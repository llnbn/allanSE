#include "tiva/BoardImplementation.hpp"
#include "inc/InterruptHandler.hpp"
#include "inc/Timer.hpp"


/*================================ define ===================================*/

/*================================ typedef ==================================*/

/*=============================== variables =================================*/

extern BoardImplementation board;

/*=============================== prototypes ================================*/

/*================================= public ==================================*/

Timer::Timer(){
	callback_ = nullptr;
	prescaler_ = 1;
}

void Timer::init(TimerConfig& config) {

  config_ = config;
  callback_ = nullptr;
  prescaler_ = 1;

  // todo: suport to sleep and deepsleep modes
  SysCtlPeripheralEnable(config_.peripheral);

  // Wait for the Timer0 module to be ready.
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0))
  {
  }

  /* Disable peripheral previous to configuring it */
  TimerDisable(config_.base, config_.source);

  /* Configure the peripheral */
  TimerConfigure(config_.base, config_.config);


}

void Timer::setPrescaler(uint32_t prescaler) {

	/* Prescaler is only available for individual timers */
	if (config_.source != TIMER_BOTH) {
		if ((1 <= prescaler) && ((prescaler <= 255) || (prescaler <= 65535))) {
			/* Remember prescaler value */
			prescaler_ = prescaler;
			/* Configure the prescaler */
			TimerPrescaleSet(config_.base, config_.source, prescaler_);
		}
	}
}

void Timer::setFrequency(uint32_t freq) {
	uint32_t clock, ticks;

	/* Get the system clock */
	clock = board.getClock();

	ticks = clock/(prescaler_ * freq);

	/* Set the timer to the given frequency */
	TimerLoadSet(config_.base, config_.source, ticks);

}

void Timer::setPeriod(uint32_t period_micros) {
	uint32_t ticks, ticks_us;

	/* Calculate the number of ticks/microsecond based on the system clock */
	ticks_us = board.getClock()/(prescaler_ * 1000000);
	/* Calculate the number of ticks for the given period */
	ticks = ticks_us * period_micros;
	/* Set the timer to the given number of ticks */
	TimerLoadSet(config_.base, config_.source, ticks);
}

void Timer::start(void) {
	  TimerEnable(config_.base, config_.source);
}

void Timer::stop(void){
	  TimerDisable(config_.base, config_.source);
}

uint32_t Timer::read(void){
	  return TimerValueGet(config_.base, config_.source);
}

void Timer::setCallback(callback_t callback) {
	callback_ = callback;
}

void Timer::clearCallback(void) {
	callback_ = nullptr;
}

void Timer::enableInterrupts(void) {

	InterruptHandler::getInstance().setInterruptHandler(*this);

	/* Clear Timer interrupts */
	TimerIntClear(config_.base, config_.interrupt_mode);

	/* Enable Timer interrupts */
	TimerIntEnable(config_.base, config_.interrupt_mode);

	/* Set the Timer interrupt priority */
	IntPrioritySet(config_.interrupt, 0xE0);

   /* Enable the Timer interrupt */
	IntEnable(config_.interrupt);
}

void Timer::disableInterrupts(void) {
	/* Disable Timer interrupts */
	TimerIntDisable(config_.base, config_.interrupt_mode);

	/* Disable the Timer interrupt */
	IntDisable(config_.interrupt);
}

/*=============================== protected =================================*/

TimerConfig Timer::getConfig(void) {
	return config_;
}

void Timer::interruptHandler(void){
	if (callback_ != nullptr) {
		callback_();
	}
}

/*================================ private ==================================*/

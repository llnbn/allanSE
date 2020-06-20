#include "inc/Gpio.hpp"

/*================================ define ===================================*/

/*================================ typedef ==================================*/

/*=============================== variables =================================*/

/*=============================== prototypes ================================*/

/*================================= public ==================================*/

GpioIn::GpioIn(){
	callback_ = nullptr;
}

bool GpioIn::read(void) {
  uint32_t state;
  
  /* Read the pin value */
  state = HWREG(config_.port + (config_.pin << 2));
  
  return (bool)state;
}

void GpioIn::enable(GpioConfig& config) {
	config_ = config;
	HWREG(config_.port + GPIO_O_AMSEL)  &= ~(config_.pin); //desabilita função analógica
	HWREG(config_.port + GPIO_O_DIR) &= ~(config_.pin); // seta direção do pino
	HWREG(config_.port + GPIO_O_AFSEL) &= ~(config_.pin); //desabilita função alternativa
	HWREG(config_.port  + GPIO_O_DEN) |= config_.pin; //habilita e/s digital

	/* Set the edge of the interrupt */
	GPIOIntTypeSet(config_.port, config_.pin, config_.edge);


}


void GpioIn::setCallback(callback_t callback) {
  /* Save the pointer to the callback function */
  callback_ = callback;

  /* Get a reference to the interruptHandler object */
  InterruptHandler& interruptHandler = InterruptHandler::getInstance();

  /* Register to the interruptHandler by passing a pointer to the object */
  interruptHandler.setInterruptHandler(*this);
}

void GpioIn::clearCallback(void) {
  /* Clear the pointer to the callback function */
  callback_ = nullptr;
}

void GpioIn::enableInterrupts(void) {
  /* Clear the interrupt */
	GPIOIntClear(config_.port, config_.pin);

  /* Enable the interrupt */
	GPIOIntEnable(config_.port, config_.pin);
}

void GpioIn::disableInterrupts(void) {
  /* Disable the interrupt */
	GPIOIntDisable(config_.port, config_.pin);
}


void GpioIn::interruptHandler(void){
  /* Call the interrupt handler if it is NOT null */
  if (callback_ != nullptr) {
      callback_();
  }
}


/*=============================== protected =================================*/

/*================================ private ==================================*/

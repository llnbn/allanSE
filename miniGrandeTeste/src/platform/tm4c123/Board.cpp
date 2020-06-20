#include "inc/Board.hpp"
#include "inc/Gpio.hpp"
#include "tiva/BoardImplementation.hpp"

/*================================ define ===================================*/

/*================================ typedef ==================================*/

/*=============================== variables =================================*/

extern GpioOut led_red;

/*=============================== prototypes ================================*/

/*================================= public ==================================*/

void Delay(void) {
	unsigned long volatile time;
	time = 1598;
	//145448;  // 0.1sec
	while(time)
	{
		time--;
	}
}

Board::Board(){
	sleepMode = nullptr;
}

void Board::init() {
	///init PLL to 80Mhz
	// 0) Use RCC2
	SYSCTL_RCC2_R |=  0x80000000;  // USERCC2
	Delay();
	Delay();
	// 1) bypass PLL while initializing
	SYSCTL_RCC2_R |=  0x00000800;  // BYPASS2, PLL bypass
	// 2) select the crystal value and oscillator source
	SYSCTL_RCC_R = (SYSCTL_RCC_R &~0x000007C0)   // clear XTAL field, bits 10-6
									 + 0x00000540;   // 10101, configure for 16 MHz crystal
	SYSCTL_RCC2_R &= ~0x00000070;  // configure for main oscillator source
	// 3) activate PLL by clearing PWRDN
	SYSCTL_RCC2_R &= ~0x00002000;
	// 4) set the desired system divider
	SYSCTL_RCC2_R |= 0x40000000;   // use 400 MHz PLL
	SYSCTL_RCC2_R = (SYSCTL_RCC2_R&~ 0x1FC00000)  // clear system clock divider
									  + (4<<22);      // configure for 80 MHz clock
	// 5) wait for the PLL to lock by polling PLLLRIS
	while((SYSCTL_RIS_R&0x00000040)==0){};  // wait for PLLRIS bit
	// 6) enable use of PLL by clearing BYPASS
	SYSCTL_RCC2_R &= ~0x00000800;
}

void Board::reset(void) {
	/* Force a software reset */
	SysCtlReset();
}

void Board::error(void) {
  while(true)
  {
    led_red.on();
    uint8_t i;
    for(i = 0; i < 200; i++) Delay();
    led_red.off();
    for(i = 0; i < 200; i++) Delay();
  }
}

uint32_t Board::getClock(void) {
	/* Return the current CPU clock */
	return SysCtlClockGet();
}

void Board::setSleepMode(int sleepMode) {
	if (sleepMode == 1) this->sleepMode = &SysCtlSleep;
	else if (sleepMode == 2) this->sleepMode = &SysCtlDeepSleep;
}

void Board::sleep(void) {
   this->sleepMode();
}

void Board::wakeup(void) {
	/* Wake-up peripherals */
    /* TODO */
}

void Board::enableInterrupts(void) {
	/* Enable global interrupts */
	IntMasterEnable();
}

void Board::disableInterrupts(void) {
	/* Disable global interrupts */
	IntMasterDisable();
}

/*=============================== protected =================================*/

/*================================ private ==================================*/

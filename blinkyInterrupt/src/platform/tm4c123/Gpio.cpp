#include "inc/Gpio.hpp"

/*================================ define ===================================*/

/*================================ typedef ==================================*/

/*=============================== variables =================================*/

/*=============================== prototypes ================================*/

/*================================= public ==================================*/

Gpio::Gpio(){
	config_.edge = 0;
	config_.invert = 0;
	config_.ioc = 0;
	config_.pin = 0;
	config_.port = 0;
}


bool Gpio::operator==(const Gpio& other) {
	return (&config_ == &other.config_);
}

GpioConfig Gpio::getGpioConfig(void) {
	return config_;
}

/*=============================== protected =================================*/

/*================================ private ==================================*/

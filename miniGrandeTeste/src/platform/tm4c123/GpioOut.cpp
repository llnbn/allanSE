/*================================ include ==================================*/

#include "inc/Gpio.hpp"

/*================================ define ===================================*/

/*================================ typedef ==================================*/

/*=============================== variables =================================*/

/*=============================== prototypes ================================*/

/*================================= public ==================================*/

GpioOut::GpioOut(){}


void GpioOut::on(void) {
  if (config_.invert) {
    /* Write a zero to set pin on */
	  HWREG(config_.port + (config_.pin << 2)) =  ~(config_.pin);
  }
  else {
    /* Write a one to set pin on */
    HWREG(config_.port + (config_.pin << 2)) =  config_.pin;
  }
}

void GpioOut::off(void) {
  if (config_.invert == 1) {
    /* Write a one to set pin off */
	  HWREG(config_.port + (config_.pin << 2)) =  config_.pin;
  }
  else {
    /* Write a zero to set pin off */
    HWREG(config_.port + (config_.pin << 2))  =  ~(config_.pin);

  }
}

void GpioOut::toggle(void) {
	uint32_t status = HWREG(config_.port + (config_.pin << 2));
	HWREG(config_.port + (config_.pin << 2)) = status^config_.pin;

}

void GpioOut::high(void){
  /* Write a one to set pin high */
  HWREG(config_.port + (config_.pin << 2)) =  config_.pin;
}

void GpioOut::low(void) {
  /* Write a zero to set pin low */
	HWREG(config_.port + (config_.pin << 2)) =  ~(config_.pin);
}

uint32_t GpioOut::status(void) {
  /* Read the pin status */
  uint32_t status = HWREG(config_.port + (config_.pin << 2));

  /* Return the pin status */
  return (status);
}

void GpioOut::enable(GpioConfig& config)  {
	config_ = config;
	HWREG(config_.port + GPIO_O_AMSEL)  &= ~(config_.pin); //desabilita função analógica
	HWREG(config_.port + GPIO_O_DIR) |= config_.pin; // seta direção do pino
	HWREG(config_.port + GPIO_O_AFSEL) &= ~(config_.pin); //desabilita função alternativa
	HWREG(config_.port + GPIO_O_DEN) |= config_.pin; //habilita e/s digital
}

/*=============================== protected =================================*/

/*================================ private ==================================*/

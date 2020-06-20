#ifndef GPIO_HPP_
#define GPIO_HPP_

#include <stdint.h>
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/InterruptHandler.hpp"
#include "inc/platform_defines.h"

class GpioConfig {
	public:
	  bool operator==(const GpioConfig& other) {return (port == other.port) && (pin == other.pin);}
	public:
	  uint32_t port;
	  uint32_t pin;
	  uint32_t ioc;
	  uint32_t edge;
	  uint32_t invert;
};

class Gpio {
	public:
	  Gpio();
	  GpioConfig getGpioConfig(void);
	public:
	  bool operator==(const Gpio& other);
	protected:
	  GpioConfig config_;
};

/*****************************************************************************/

class GpioIn : public Gpio {
	friend class InterruptHandler; //permite acessar atributos privados/protegidos em InterruptHandler
	public:
	  GpioIn();
	  bool read(void);
	  void enable(GpioConfig& config);
	  void setCallback(callback_t callback);
	  void clearCallback(void);
	  void enableInterrupts(void);
	  void disableInterrupts(void);
	protected:
	  callback_t callback_;
	  void interruptHandler(void);
};


/*****************************************************************************/

class GpioOut : public Gpio {
	public:
	  GpioOut();
	  void on(void);
	  void off(void);
	  void toggle(void);
	  void high(void);
	  void low(void);
	  uint32_t status(void);
	  void enable(GpioConfig& config);
};


#endif /* GPIO_HPP_ */

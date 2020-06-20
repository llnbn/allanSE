#ifndef BOARD_IMPLEMENTATION_HPP_
#define BOARD_IMPLEMENTATION_HPP_

/*================================ include ==================================*/

#include "inc/Board.hpp"
#include "inc/Gpio.hpp"
#include "inc/Timer.hpp"

/*================================ define ===================================*/

/*================================ typedef ==================================*/

/*=============================== variables =================================*/

/* Board */
extern BoardImplementation board;

/* LEDs */
extern GpioOut led_blue;
extern GpioOut led_red;
extern GpioOut led_green;

/* Buttons */
extern GpioIn sw1;
extern GpioIn sw2;
//extern GpioIn sw3;

/* Timers */
extern Timer timer0a;
//extern Timer timer1a;
//extern Timer timer2a;

/*=============================== prototypes ================================*/

/*================================= public ==================================*/

/*================================ private ==================================*/

#endif /* BOARD_IMPLEMENTATION_HPP_ */

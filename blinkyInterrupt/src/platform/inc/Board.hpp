#ifndef BOARD_HPP_
#define BOARD_HPP_

#include <stdint.h>
#include "driverlib/interrupt.h"
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "inc/tm4c123gh6pm.h"

class Board {
public:
  Board();
  void init();
  void reset(void);
  void error(void);
  uint32_t getClock(void);
  void setSleepMode(int);
  void sleep(void);
  void wakeup(void);
  void enableInterrupts(void);
  void disableInterrupts(void);
private:
  void (*sleepMode)();
  void setup();
};

class BoardImplementation : public Board {
public:
  BoardImplementation();
  void init(void);
};

#endif /* BOARD_HPP_ */

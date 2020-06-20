#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <stdint.h>
#include <driverlib/timer.h>
#include "inc/platform_defines.h"

class TimerConfig {
public:
  bool operator==(const TimerConfig& other) {return (peripheral == other.peripheral);}
public:
  uint32_t peripheral;
  uint32_t base;
  uint32_t source;
  uint32_t config;
  uint32_t interrupt;
  uint32_t interrupt_mode;
};

class Timer {

friend class InterruptHandler;

public:
    Timer(void);
    void init(TimerConfig& config);
    void setPrescaler(uint32_t prescaler);
    void setFrequency(uint32_t milliseconds);
    void setPeriod(uint32_t microseconds);
    void start(void);
    void stop(void);
    uint32_t read(void);
    void setCallback(callback_t callback);
    void clearCallback(void);
    void enableInterrupts(void);
    void disableInterrupts(void);
protected:
    TimerConfig getConfig(void);
    void interruptHandler(void);
protected:
    TimerConfig config_;
    
    callback_t callback_;
    
    uint32_t prescaler_;
};

#endif /* TIMER_HPP_ */

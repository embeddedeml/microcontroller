/*
 * Scheduler.h
 *
 *  Created on: Nov 2, 2014
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "BaseDef.h"
#include "Scheduler_Cfg.h"

#ifndef SCHEDULER_CFG_NROFTASKS
#error "SCHEDULER_CFG_NROFTASKS not defined"
#endif

#ifdef SCHEDULER_CFG_US_PER_TICK
#define SCHEDULER_US_IN_TICKS(us) (us/SCHEDULER_CFG_US_PER_TICK)
#endif

#ifdef SCHEDULER_CFG_MS_PER_TICK
#define SCHEDULER_MS_IN_TICKS(ms) (ms/SCHEDULER_CFG_MS_PER_TICK)
#endif

#ifdef SCHEDULER_CFG_S_PER_TICK
#define SCHEDULER_S_IN_TICKS(s) (us/SCHEDULER_CFG_S_PER_TICK)
#endif


typedef void (*FunctionPointer)();
struct Scheduler_Tasks_t
{
    uint8_t state;
    uint16_t timer;
    uint16_t timerPeriod;
    FunctionPointer callback;
};

class Scheduler
{
public:
    Scheduler();
    void init(void);
    void incrementTicks(void);
    StdReturnType enableTask(uint8_t id, uint16_t offset, uint16_t period, FunctionPointer callback);
    StdReturnType disableTask(uint8_t id);
    void run(void);

private:
    Scheduler_Tasks_t Tasks[SCHEDULER_CFG_NROFTASKS];
};

#endif /* SCHEDULER_H_ */

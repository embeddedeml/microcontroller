/*
 * Scheduler.cpp
 *
 *  Created on: Nov 2, 2014
 */

#include "Scheduler.h"

#define SCHEDULER_STATE_DISABLED	(0x00U)
#define SCHEDULER_STATE_IDLE		(0x01U)
#define SCHEDULER_STATE_SCHEDULED	(0x02U)

Scheduler::Scheduler()
{
}

/**
 * @brief Initializes internal states.
 */
void Scheduler::init(void)
{
    uint8_t task;
    for(task = 0; task < SCHEDULER_CFG_NROFTASKS; task++)
    {
        this->Tasks[task].state = SCHEDULER_STATE_DISABLED;
    }
}

/**
 * @brief Increments tick counter by one.
 */
void Scheduler::incrementTicks(void)
{
    uint8_t task;

    for(task = 0; task < SCHEDULER_CFG_NROFTASKS; task++)
    {
        if(this->Tasks[task].state != SCHEDULER_STATE_DISABLED)
        {
            if(this->Tasks[task].timer > 0x00U)
            {
                /* timer is running, decrement timer */
                this->Tasks[task].timer--;

            } else
            {
                /* timer expired, reload timer and schedule for execution */
                this->Tasks[task].timer = this->Tasks[task].timerPeriod;
                this->Tasks[task].state = SCHEDULER_STATE_SCHEDULED;
            }
        }
    }
}

/**
 * @brief Enables execution of task.
 *
 * @param id        Id of task
 * @param offset    Initial offset before first execution (ms)
 * @param period    Cycle time of task (ms)
 * @param callback  Task callback

 * @return E_OK     Task enabled
 * @return E_NOT_OK Task ID not found
 */
uint8_t Scheduler::enableTask(uint8_t id, uint16_t offset, uint16_t period, FunctionPointer callback)
{
    uint8_t retVal;

    if(id < SCHEDULER_CFG_NROFTASKS)
    {
        this->Tasks[id].state = SCHEDULER_STATE_DISABLED;

        this->Tasks[id].timerPeriod = SCHEDULER_MS_IN_TICKS(period);
        this->Tasks[id].timer = offset;
        this->Tasks[id].callback = callback;
        this->Tasks[id].state = SCHEDULER_STATE_IDLE;
        retVal = E_OK;
    } else
    {
        retVal = E_NOT_OK;
    }

    return retVal;
}

/**
 * @brief Disables execution of task
 *
 * @param id    Task ID
 *
 * @return E_OK     Task disabled
 * @return E_NOT_OK Task ID not found
 */
uint8_t Scheduler::disableTask(uint8_t id)
{
    uint8_t retVal;

    if(id < SCHEDULER_CFG_NROFTASKS)
    {
        this->Tasks[id].state = SCHEDULER_STATE_DISABLED;
        retVal = E_OK;
    } else
    {
        retVal = E_NOT_OK;
    }

    return retVal;
}

/**
 * @brief Executes all tasks with expired timer
 */
void Scheduler::run(void)
{
    uint8_t task;

    for(task = 0; task < SCHEDULER_CFG_NROFTASKS; task++)
    {
        if(this->Tasks[task].state == SCHEDULER_STATE_SCHEDULED)
        {
            this->Tasks[task].state = SCHEDULER_STATE_IDLE;
            this->Tasks[task].callback();
        }
    }
}

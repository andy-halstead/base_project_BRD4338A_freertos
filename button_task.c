/*
 * blink_task.c
 *
 *  Created on: 16 May 2025
 *      Author: anhalste
 */

#include "cmsis_os2.h"
#include "sl_constants.h"
#include "string.h"

#include "sl_si91x_button.h"
#include "sl_si91x_button_instances.h"
#include "FreeRTOS.h"
#include "task.h"

#include "button_task.h"

/******************************************************
 *                    Constants
 ******************************************************/


/******************************************************
 *               Variable Definitions
 ******************************************************/

const osThreadAttr_t button_task_attributes = {
  .name       = "button",
  .attr_bits  = 0,
  .cb_mem     = 0,
  .cb_size    = 0,
  .stack_mem  = 0,
  .stack_size = 3072,
  .priority   = osPriorityLow,
  .tz_module  = 0,
  .reserved   = 0,
};

osEventFlagsId_t button_event_id;

/******************************************************
 *               Function Declarations
 ******************************************************/
static void button_task(void *argument);

/******************************************************
 *               Function Definitions
 ******************************************************/

void button_task_init(void)
{
	osThreadNew((osThreadFunc_t)button_task, NULL, &button_task_attributes);
}

/*******************************************************************************
 * Button task.
 ******************************************************************************/
static void button_task(void *arg)
{
    printf("\r\nButton Task Started\r\n");

	(void)&arg;
    uint32_t received_flags;
    button_event_id = osEventFlagsNew(NULL);
    
    while( 1 )
	{
        received_flags = osEventFlagsWait(button_event_id, BUTTON_EVENT_ALL , osFlagsWaitAny, osWaitForever);
        
        if( received_flags & BUTTON_EVENT_BUTTON0_PRESSED ) {
            printf("BUTTON_EVENT_BUTTON0_PRESSED\r\n");

        }
        if( received_flags & BUTTON_EVENT_BUTTON1_PRESSED ) {
            printf("BUTTON_EVENT_BUTTON1_PRESSED\r\n");
        }
	}
}

void button_process_action(void)
{
  return;
}

void sl_si91x_button_isr(uint8_t pin, int8_t state)
{
    if (pin == button_btn0.pin) {
        if (state == BUTTON_PRESSED) {
            osEventFlagsSet(button_event_id, BUTTON_EVENT_BUTTON0_PRESSED);
        }
    }
    if (pin == button_btn1.pin) {
        if (state == BUTTON_PRESSED) {
            osEventFlagsSet(button_event_id, BUTTON_EVENT_BUTTON1_PRESSED);
        }
    }
}


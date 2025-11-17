/*
 * app_task.c
 *
 *  Created on: 17 Nov 2025
 *      Author: anhalste
 */

#include "cmsis_os2.h"
#include "sl_constants.h"
#include "string.h"

#include "sl_si91x_led.h"
#include "sl_si91x_led_instances.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sl_si91x_power_manager.h"

#include "app_task.h"

/******************************************************
 *                    Constants
 ******************************************************/
#define TOGGLE_DELAY_MS                    10000


/******************************************************
 *               Variable Definitions
 ******************************************************/

const osThreadAttr_t app_task_attributes = {
  .name       = "app",
  .attr_bits  = 0,
  .cb_mem     = 0,
  .cb_size    = 0,
  .stack_mem  = 0,
  .stack_size = 3072,
  .priority   = osPriorityLow,
  .tz_module  = 0,
  .reserved   = 0,
};

volatile uint32_t idleCounter;

/******************************************************
 *               Function Declarations
 ******************************************************/
static void app_task(void *argument);

/******************************************************
 *               Function Definitions
 ******************************************************/

void app_task_init(void)
{
	osThreadNew((osThreadFunc_t)app_task, NULL, &app_task_attributes);
}

/*******************************************************************************
 * App task.
 ******************************************************************************/
static void app_task(void *arg)
{
	printf("\r\nApp Task Started\r\n");

	(void)&arg;
	TickType_t xDelay = TOGGLE_DELAY_MS / portTICK_PERIOD_MS;
	
	while( 1 )
	{
		vTaskDelay(xDelay);
		printf("App Task Awake\r\n");
	}
}

void vApplicationIdleHook(void)
{
    idleCounter++;  // if this never increments, some task is Ready/running

    volatile uint8_t *requirement_table;
    requirement_table = (uint8 *)sl_si91x_power_manager_get_requirement_table();
    (void) requirement_table;
    if( requirement_table[4]!=0 ||  requirement_table[3]!=0 || requirement_table[2]!=0 || requirement_table[1]!=0 || requirement_table[0]!=0 )
    { 
       __NOP(); 
    }
}
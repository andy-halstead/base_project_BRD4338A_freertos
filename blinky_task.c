/*
 * blink_task.c
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

#include "blinky_task.h"

/******************************************************
 *                    Constants
 ******************************************************/
#define TOGGLE_DELAY_MS                    3000


/******************************************************
 *               Variable Definitions
 ******************************************************/

const osThreadAttr_t blinky_task_attributes = {
  .name       = "blinky",
  .attr_bits  = 0,
  .cb_mem     = 0,
  .cb_size    = 0,
  .stack_mem  = 0,
  .stack_size = 3072,
  .priority   = osPriorityLow,
  .tz_module  = 0,
  .reserved   = 0,
};

/******************************************************
 *               Function Declarations
 ******************************************************/
static void blinky_task(void *argument);

/******************************************************
 *               Function Definitions
 ******************************************************/

void blinky_task_init(void)
{
	osThreadNew((osThreadFunc_t)blinky_task, NULL, &blinky_task_attributes);
}

/*******************************************************************************
 * Blinky task.
 ******************************************************************************/
static void blinky_task(void *arg)
{
	printf("\r\nBlinky Task Started\r\n");

	(void)&arg;
	TickType_t xDelay = TOGGLE_DELAY_MS / portTICK_PERIOD_MS;

	sl_si91x_led_clear(SL_SI91X_LED_0_PIN);
	sl_si91x_led_clear(SL_SI91X_LED_1_PIN);
	
	while( 1 )
	{
		vTaskDelay(xDelay);
		printf("Blinky Task Awake\r\n");
		//sl_si91x_led_toggle(SL_SI91X_LED_0_PIN);
		//sl_si91x_led_toggle(SL_SI91X_LED_1_PIN);
	}
}

void preSleep(uint32_t idle_time)
{
	(void)idle_time;
	//sl_led_turn_off(&sl_led_led0);
	//printf("Idling for: 0x%08x ticks\r\n", (int)idle_time);
}

void sleptFor(uint32_t sleep_time)
{
	(void)sleep_time;
	//printf(PINK "\tWoke after %us with: 0x%08x\r\n", sleep_time, GPIO->IF);
	//sl_led_turn_on(&sl_led_led0);
	//printf("Slept for: 0x%08x ticks\r\n", (int)sleep_time);
}


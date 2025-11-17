/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#include <stdio.h>
#include "cmsis_os2.h"

#include "blinky_task.h"
#include "button_task.h"
#include "wifi_task.h"
#include "app_task.h"

#include "sl_si91x_power_manager.h"

osThreadId_t h_init_thread;

const osThreadAttr_t init_app_thread_attributes = {
  .name       = "init",
  .attr_bits  = 0,
  .cb_mem     = 0,
  .cb_size    = 0,
  .stack_mem  = 0,
  .stack_size = 3072,
  .priority   = osPriorityLow,
  .tz_module  = 0,
  .reserved   = 0,
};


static void init_thread(void *argument);

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
	//printf(WHITE "\r\n\tApp: " GREEN "Initialising...\r\n" WHITE);
	h_init_thread = osThreadNew((osThreadFunc_t)init_thread, NULL, &init_app_thread_attributes);
}

static void init_thread(void *argument)
{
	(void)argument;

  sl_si91x_power_manager_add_ps_requirement(SL_SI91X_POWER_MANAGER_PS4);
	blinky_task_init();
  button_task_init();
	wifi_task_init();
  app_task_init();
  sl_si91x_power_manager_remove_ps_requirement(SL_SI91X_POWER_MANAGER_PS4);

	osThreadTerminate(h_init_thread);
}



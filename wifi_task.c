/*
 * wifi_task.c
 *
 *  Created on: 17 Nov 2025
 *      Author: anhalste
 */

#include "cmsis_os2.h"
#include "sl_constants.h"
#include "string.h"

#include "FreeRTOS.h"
#include "task.h"

#include "sl_wifi.h"
#include "sl_net.h"
#include "sl_net_constants.h"

#include "wifi_task.h"

/******************************************************
 *                    Constants
 ******************************************************/
#define TOGGLE_DELAY_MS                    4000


/******************************************************
 *               Variable Definitions
 ******************************************************/

const osThreadAttr_t wifi_task_attributes = {
  .name       = "wifi",
  .attr_bits  = 0,
  .cb_mem     = 0,
  .cb_size    = 0,
  .stack_mem  = 0,
  .stack_size = 3072,
  .priority   = osPriorityLow,
  .tz_module  = 0,
  .reserved   = 0,
};

static const sl_wifi_device_configuration_t nwp_configuration = {
  .boot_option = LOAD_NWP_FW,
  .mac_address = NULL,
  .band        = SL_SI91X_WIFI_BAND_2_4GHZ,
  .boot_config = { .oper_mode       = SL_SI91X_CLIENT_MODE,
                   .coex_mode       = SL_SI91X_WLAN_ONLY_MODE,
                   .feature_bit_map = (SL_SI91X_FEAT_SECURITY_OPEN | SL_SI91X_FEAT_ULP_GPIO_BASED_HANDSHAKE
#ifdef SLI_SI91X_MCU_INTERFACE
                                       | SL_SI91X_FEAT_WPS_DISABLE
#endif
                                       ),
                   .tcp_ip_feature_bit_map =
                     (SL_SI91X_TCP_IP_FEAT_DHCPV4_CLIENT | SL_SI91X_TCP_IP_FEAT_EXTENSION_VALID),
                   .custom_feature_bit_map     = (SL_SI91X_CUSTOM_FEAT_EXTENTION_VALID),
                   .ext_custom_feature_bit_map = (SL_SI91X_EXT_FEAT_LOW_POWER_MODE | MEMORY_CONFIG
#if defined(SLI_SI917) || defined(SLI_SI915)
                                                  | SL_SI91X_EXT_FEAT_FRONT_END_SWITCH_PINS_ULP_GPIO_4_5_0
#endif
                                                  ),
                   .bt_feature_bit_map         = 0,
                   .ext_tcp_ip_feature_bit_map = SL_SI91X_CONFIG_FEAT_EXTENTION_VALID,
                   .ble_feature_bit_map        = 0,
                   .ble_ext_feature_bit_map    = 0,
                   .config_feature_bit_map     = SL_SI91X_FEAT_SLEEP_GPIO_SEL_BITMAP }
};

/******************************************************
 *               Function Declarations
 ******************************************************/
static void wifi_task(void *argument);

/******************************************************
 *               Function Definitions
 ******************************************************/

void wifi_task_init(void)
{
	osThreadNew((osThreadFunc_t)wifi_task, NULL, &wifi_task_attributes);
}

/*******************************************************************************
 * Wifi task.
 ******************************************************************************/
static void wifi_task(void *arg)
{
  printf("\r\nWiFi Task Started\r\n");

	(void)&arg;
	TickType_t                  xDelay = TOGGLE_DELAY_MS / portTICK_PERIOD_MS;
  sl_mac_address_t			      mac_addr	= { 0 };
	sl_wifi_firmware_version_t	version = { 0 };
	sl_status_t					        status;
  uint8_t                     xtal_enable = 1;

  // Init NWP
	status = sl_net_init(SL_NET_WIFI_CLIENT_INTERFACE, &nwp_configuration, NULL, NULL);
	if( (status != SL_STATUS_OK) && (status != SL_STATUS_ALREADY_INITIALIZED) )
	{
		printf("WiFi Init Fail 0x%1x\r\n", (int)status);
		return;
	}
	printf("WiFi Init: Success\r\n");

  // Establish a secure handshake between the M4 core and the NWP
  status = sl_si91x_m4_ta_secure_handshake(SL_SI91X_ENABLE_XTAL, 1, &xtal_enable, 0, NULL);
  if (status != SL_STATUS_OK) {
    printf("Failed to bring m4_ta_secure_handshake: 0x%lX\r\n", status);
    return;
  }
  printf("m4_ta_secure_handshake Success\r\n");

  // Read MAC address
  status = sl_wifi_get_mac_address(SL_WIFI_CLIENT_INTERFACE, &mac_addr);
	if( status == SL_STATUS_OK )
	{
		printf("MAC Address: %x:%x:%x:%x:%x:%x\r\n",
			   mac_addr.octet[0], mac_addr.octet[1],
			   mac_addr.octet[2], mac_addr.octet[3],
			   mac_addr.octet[4], mac_addr.octet[5]);
	} else
	{
		printf("MAC - Fail: 0x%lx\r\n", status);
		return;
	}

  // Read NWP FW version
  status = sl_wifi_get_firmware_version(&version);
	if( status == SL_STATUS_OK )
	{
		printf("WiFi Firmware: %x%x.%d.%d.%d.%d.%d.%d\r\n",
			 version.chip_id,
			 version.rom_id,
			 version.major,
			 version.minor,
			 version.security_version,
			 version.patch_num,
			 version.customer_id,
			 version.build_num);
	} else
	{
		printf("WiFi Firmware - Fail: 0x%lx\r\n", status);
		return;
	}

  // Place NWP in sleep without RAM reterntion
  sl_wifi_performance_profile_v2_t performance_profile = { .profile = DEEP_SLEEP_WITH_RAM_RETENTION };
  status = sl_wifi_set_performance_profile_v2(&performance_profile);
  if (status != SL_STATUS_OK) {
    printf("Power save profile with deep sleep Failed, Error Code : 0x%lX\r\n", status);
    return;
  }
  printf("Power save profile with deep sleep Success \r\n");

  while( 1 )
	{
		vTaskDelay(xDelay);
    printf("WiFi Task Awake\r\n");
	}
}



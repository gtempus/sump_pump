/**
 * \file
 *
 * \brief WINC1500 TCP Client Example.
 *
 * Copyright (c) 2014 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

/* Includes */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "interrupt.h"
#include "delay.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "status_codes.h"
#include "common/include/nm_common.h"
#include "driver/include/m2m_wifi.h"
#include "socket/include/socket.h"
#include "driver/source/nmasic.h"
#include "bsp/include/nm_bsp_mega.h"
#include "ioport.h"
#include "sysclk.h"
#include "spi.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Port to use for TCP socket */
#define TCP_PORT                       (1234)
/** Port to use for UDP socket */
#define UDP_PORT                       (6789)

/** Max size of incoming packets */
#define MAIN_WIFI_M2M_BUFFER_SIZE		20

/** Buffer to send to identify Winc + Mega to Android app */
#define KEY_BUFFER                     {0x6d, 0x61, 0x72, 0x74, 0x65}

/** Pin to toggle */
#define TOGGLE_PIN         IOPORT_CREATE_PIN(PORTC, 0)
/** LED pin to toggle */
#define LED_PIN            IOPORT_CREATE_PIN(PORTD, 2)
/** Button pin */
#define BUTTON_PIN         IOPORT_CREATE_PIN(PORTB, 7)
/** Variable to say if the timer interrupt has occurred */
volatile bool timer_int;

/** Provisioning mode definintions */
#define DEMO_WLAN_AP_NAME                  "AVR_for_IoT"
#define DEMO_WLAN_AP_CHANNEL				1
#define DEMO_WLAN_AP_WEP_INDEX				0
#define DEMO_WLAN_AP_WEP_SIZE				WEP_40_KEY_STRING_SIZE
#define DEMO_WLAN_AP_WEP_KEY				"1234567890"
#define DEMO_WLAN_AP_SECURITY				M2M_WIFI_SEC_OPEN
#define DEMO_WLAN_AP_MODE					SSID_MODE_VISIBLE
#define DEMO_WLAN_AP_DOMAIN_NAME			"atmelconfig.com"
#define DEMO_WLAN_AP_IP_ADDRESS				{192, 168, 1, 1}


/** Max length of SSID and password */
#define MAX_LEN            0x11
/** EEPROM address to store SSID */
#define EEPROM_SSID  0
/** EEPROM address to store password */
#define EEPROM_PSW   EEPROM_SSID + MAX_LEN
/** EEPROM address to store security type */
#define EEPROM_SEC   EEPROM_PSW + MAX_LEN

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H_INCLUDED */

/*
 * The Clear BSD License
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"
#include "sourcesCW/answer.h"
#include "sourcesCW/pbkdf2/mnemonic.h"

#include <stdio.h>
#include <stdlib.h>

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"
#include "usb_device_cdc_acm.h"
#include "usb_device_ch9.h"
#include "fsl_debug_console.h"

#include "usb_device_descriptor.h"
#include "virtual_com.h"
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
#include "usb_phy.h"
#endif
#if defined(FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED) && (FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED > 0U) && \
    defined(USB_DEVICE_CONFIG_KEEP_ALIVE_MODE) && (USB_DEVICE_CONFIG_KEEP_ALIVE_MODE > 0U) &&             \
    defined(FSL_FEATURE_USB_KHCI_USB_RAM) && (FSL_FEATURE_USB_KHCI_USB_RAM > 0U)
extern uint8_t USB_EnterLowpowerMode(void);
#endif
#include "pin_mux.h"
#include <stdbool.h>
#include "fsl_power.h"

#include "data.hpp"

#include "fsl_eeprom.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define PIN_ADDR				0U
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitHardware(void);
void USB_DeviceClockInit(void);
void USB_DeviceIsrEnable(void);
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle);
#endif

void BOARD_DbgConsole_Deinit(void);
void BOARD_DbgConsole_Init(void);
usb_status_t USB_DeviceCdcVcomCallback(class_handle_t handle, uint32_t event,
		void *param);
usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event,
		void *param);

static void write_flash(uint32_t *data, uint32_t size, uint32_t addr);
static void read_flash(uint32_t *data, uint32_t size, uint32_t addr);
void initWalletCMD(int walletInit, int blocked, uint32_t *pinDef);

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern usb_device_endpoint_struct_t g_UsbDeviceCdcVcomDicEndpoints[];
extern usb_device_class_struct_t g_UsbDeviceCdcVcomConfig;
/* Data structure of virtual com device */
usb_cdc_vcom_struct_t s_cdcVcom;
static char const *s_appName = "app task";

/* Line codinig of cdc device */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_lineCoding[LINE_CODING_SIZE] =
		{
		/* E.g. 0x00,0xC2,0x01,0x00 : 0x0001C200 is 115200 bits per second */
		(LINE_CODING_DTERATE >> 0U) & 0x000000FFU, (LINE_CODING_DTERATE >> 8U)
				& 0x000000FFU, (LINE_CODING_DTERATE >> 16U) & 0x000000FFU,
				(LINE_CODING_DTERATE >> 24U) & 0x000000FFU,
				LINE_CODING_CHARFORMAT,
				LINE_CODING_PARITYTYPE,
				LINE_CODING_DATABITS };

/* Abstract state of cdc device */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_abstractState[COMM_FEATURE_DATA_SIZE] =
		{ (STATUS_ABSTRACT_STATE >> 0U) & 0x00FFU, (STATUS_ABSTRACT_STATE >> 8U)
				& 0x00FFU };

/* Country code of cdc device */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_countryCode[COMM_FEATURE_DATA_SIZE] =
		{ (COUNTRY_SETTING >> 0U) & 0x00FFU, (COUNTRY_SETTING >> 8U) & 0x00FFU };

/* CDC ACM information */
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static usb_cdc_acm_info_t s_usbCdcAcmInfo;
/* Data buffer for receiving and sending*/
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_currRecvBuf[DATA_BUFF_SIZE];
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_currSendBuf[DATA_BUFF_SIZE];
volatile static uint32_t s_recvSize = 0;
volatile static uint32_t s_sendSize = 0;

/* USB device class information */
static usb_device_class_config_struct_t s_cdcAcmConfig[1] = { {
		USB_DeviceCdcVcomCallback, 0, &g_UsbDeviceCdcVcomConfig, } };

/* USB device class configuraion information */
static usb_device_class_config_list_struct_t s_cdcAcmConfigList = {
		s_cdcAcmConfig, USB_DeviceCallback, 1, };

#if defined(FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED) && (FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED > 0U) && \
    defined(USB_DEVICE_CONFIG_KEEP_ALIVE_MODE) && (USB_DEVICE_CONFIG_KEEP_ALIVE_MODE > 0U) &&             \
    defined(FSL_FEATURE_USB_KHCI_USB_RAM) && (FSL_FEATURE_USB_KHCI_USB_RAM > 0U)
volatile static uint8_t s_waitForDataReceive = 0;
volatile static uint8_t s_comOpen = 0;
#endif
/*******************************************************************************
 * Code
 ******************************************************************************/
#if (defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U))
void USB0_IRQHandler(void)
{
	USB_DeviceLpcIp3511IsrFunction(s_cdcVcom.deviceHandle);
}
#endif
#if (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
void USB1_IRQHandler(void) {
	USB_DeviceLpcIp3511IsrFunction(s_cdcVcom.deviceHandle);
}
#endif
void USB_DeviceClockInit(void) {
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
	/* enable USB IP clock */
	CLOCK_EnableUsbfs0DeviceClock(kCLOCK_UsbSrcFro, CLOCK_GetFroHfFreq());
#if defined(FSL_FEATURE_USB_USB_RAM) && (FSL_FEATURE_USB_USB_RAM)
	for (int i = 0; i < FSL_FEATURE_USB_USB_RAM; i++)
	{
		((uint8_t *)FSL_FEATURE_USB_USB_RAM_BASE_ADDRESS)[i] = 0x00U;
	}
#endif

#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
	/* enable USB IP clock */
	CLOCK_EnableUsbhs0DeviceClock(kCLOCK_UsbSrcUsbPll, 0U);
#if defined(FSL_FEATURE_USBHSD_USB_RAM) && (FSL_FEATURE_USBHSD_USB_RAM)
	for (int i = 0; i < FSL_FEATURE_USBHSD_USB_RAM; i++) {
		((uint8_t *) FSL_FEATURE_USBHSD_USB_RAM_BASE_ADDRESS)[i] = 0x00U;
	}
#endif
#endif
}
void USB_DeviceIsrEnable(void) {
	uint8_t irqNumber;
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
	uint8_t usbDeviceIP3511Irq[] = USB_IRQS;
	irqNumber = usbDeviceIP3511Irq[CONTROLLER_ID - kUSB_ControllerLpcIp3511Fs0];
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
	uint8_t usbDeviceIP3511Irq[] = USBHSD_IRQS;
	irqNumber = usbDeviceIP3511Irq[CONTROLLER_ID - kUSB_ControllerLpcIp3511Hs0];
#endif
	/* Install isr, set priority, and enable IRQ. */
#if defined(__GIC_PRIO_BITS)
	GIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
#else
	NVIC_SetPriority((IRQn_Type) irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
#endif
	EnableIRQ((IRQn_Type) irqNumber);
}
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle)
{
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
	USB_DeviceLpcIp3511TaskFunction(deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
	USB_DeviceLpcIp3511TaskFunction(deviceHandle);
#endif
}
#endif

/*!
 * @brief CDC class specific callback function.
 *
 * This function handles the CDC class specific requests.
 *
 * @param handle          The CDC ACM class handle.
 * @param event           The CDC ACM class event type.
 * @param param           The parameter of the class specific request.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCdcVcomCallback(class_handle_t handle, uint32_t event,
		void *param) {
	usb_status_t error = kStatus_USB_Error;
	uint32_t len;
	uint8_t *uartBitmap;
	usb_cdc_acm_info_t *acmInfo = &s_usbCdcAcmInfo;
	usb_device_cdc_acm_request_param_struct_t *acmReqParam;
	usb_device_endpoint_callback_message_struct_t *epCbParam;
	acmReqParam = (usb_device_cdc_acm_request_param_struct_t *) param;
	epCbParam = (usb_device_endpoint_callback_message_struct_t *) param;
	switch (event) {
	case kUSB_DeviceCdcEventSendResponse: {
		if ((epCbParam->length != 0)
				&& (!(epCbParam->length
						% g_UsbDeviceCdcVcomDicEndpoints[0].maxPacketSize))) {
			/* If the last packet is the size of endpoint, then send also zero-ended packet,
			 ** meaning that we want to inform the host that we do not have any additional
			 ** data, so it can flush the output.
			 */
			error = USB_DeviceCdcAcmSend(handle, USB_CDC_VCOM_BULK_IN_ENDPOINT,
					NULL, 0);
		} else if ((1 == s_cdcVcom.attach)
				&& (1 == s_cdcVcom.startTransactions)) {
			if ((epCbParam->buffer != NULL )
					|| ((epCbParam->buffer == NULL ) && (epCbParam->length == 0))) {
				/* User: add your own code for send complete event */
				/* Schedule buffer for next receive event */
				error = USB_DeviceCdcAcmRecv(handle,
						USB_CDC_VCOM_BULK_OUT_ENDPOINT, s_currRecvBuf,
						g_UsbDeviceCdcVcomDicEndpoints[0].maxPacketSize);
#if defined(FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED) && (FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED > 0U) && \
    defined(USB_DEVICE_CONFIG_KEEP_ALIVE_MODE) && (USB_DEVICE_CONFIG_KEEP_ALIVE_MODE > 0U) &&             \
    defined(FSL_FEATURE_USB_KHCI_USB_RAM) && (FSL_FEATURE_USB_KHCI_USB_RAM > 0U)
				s_waitForDataReceive = 1;
				USB0->INTEN &= ~USB_INTEN_SOFTOKEN_MASK;
#endif
			}
		}
	}
		break;
	case kUSB_DeviceCdcEventRecvResponse: {
		if ((1 == s_cdcVcom.attach) && (1 == s_cdcVcom.startTransactions)) {
			s_recvSize = epCbParam->length;

#if defined(FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED) && (FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED > 0U) && \
    defined(USB_DEVICE_CONFIG_KEEP_ALIVE_MODE) && (USB_DEVICE_CONFIG_KEEP_ALIVE_MODE > 0U) &&             \
    defined(FSL_FEATURE_USB_KHCI_USB_RAM) && (FSL_FEATURE_USB_KHCI_USB_RAM > 0U)
			s_waitForDataReceive = 0;
			USB0->INTEN |= USB_INTEN_SOFTOKEN_MASK;
#endif
			if (!s_recvSize) {
				/* Schedule buffer for next receive event */
				error = USB_DeviceCdcAcmRecv(handle,
						USB_CDC_VCOM_BULK_OUT_ENDPOINT, s_currRecvBuf,
						g_UsbDeviceCdcVcomDicEndpoints[0].maxPacketSize);
#if defined(FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED) && (FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED > 0U) && \
    defined(USB_DEVICE_CONFIG_KEEP_ALIVE_MODE) && (USB_DEVICE_CONFIG_KEEP_ALIVE_MODE > 0U) &&             \
    defined(FSL_FEATURE_USB_KHCI_USB_RAM) && (FSL_FEATURE_USB_KHCI_USB_RAM > 0U)
				s_waitForDataReceive = 1;
				USB0->INTEN &= ~USB_INTEN_SOFTOKEN_MASK;
#endif
			}
		}
	}
		break;
	case kUSB_DeviceCdcEventSerialStateNotif:
		((usb_device_cdc_acm_struct_t *) handle)->hasSentState = 0;
		error = kStatus_USB_Success;
		break;
	case kUSB_DeviceCdcEventSendEncapsulatedCommand:
		break;
	case kUSB_DeviceCdcEventGetEncapsulatedResponse:
		break;
	case kUSB_DeviceCdcEventSetCommFeature:
		if (USB_DEVICE_CDC_FEATURE_ABSTRACT_STATE == acmReqParam->setupValue) {
			if (1 == acmReqParam->isSetup) {
				*(acmReqParam->buffer) = s_abstractState;
			} else {
				*(acmReqParam->length) = 0;
			}
		} else if (USB_DEVICE_CDC_FEATURE_COUNTRY_SETTING
				== acmReqParam->setupValue) {
			if (1 == acmReqParam->isSetup) {
				*(acmReqParam->buffer) = s_countryCode;
			} else {
				*(acmReqParam->length) = 0;
			}
		}
		error = kStatus_USB_Success;
		break;
	case kUSB_DeviceCdcEventGetCommFeature:
		if (USB_DEVICE_CDC_FEATURE_ABSTRACT_STATE == acmReqParam->setupValue) {
			*(acmReqParam->buffer) = s_abstractState;
			*(acmReqParam->length) = COMM_FEATURE_DATA_SIZE;
		} else if (USB_DEVICE_CDC_FEATURE_COUNTRY_SETTING
				== acmReqParam->setupValue) {
			*(acmReqParam->buffer) = s_countryCode;
			*(acmReqParam->length) = COMM_FEATURE_DATA_SIZE;
		}
		error = kStatus_USB_Success;
		break;
	case kUSB_DeviceCdcEventClearCommFeature:
		break;
	case kUSB_DeviceCdcEventGetLineCoding:
		*(acmReqParam->buffer) = s_lineCoding;
		*(acmReqParam->length) = LINE_CODING_SIZE;
		error = kStatus_USB_Success;
		break;
	case kUSB_DeviceCdcEventSetLineCoding: {
		if (1 == acmReqParam->isSetup) {
			*(acmReqParam->buffer) = s_lineCoding;
		} else {
			*(acmReqParam->length) = 0;
		}
	}
		error = kStatus_USB_Success;
		break;
	case kUSB_DeviceCdcEventSetControlLineState: {
		s_usbCdcAcmInfo.dteStatus = acmReqParam->setupValue;
		/* activate/deactivate Tx carrier */
		if (acmInfo->dteStatus
				& USB_DEVICE_CDC_CONTROL_SIG_BITMAP_CARRIER_ACTIVATION) {
			acmInfo->uartState |= USB_DEVICE_CDC_UART_STATE_TX_CARRIER;
		} else {
			acmInfo->uartState &=
					(uint16_t) ~USB_DEVICE_CDC_UART_STATE_TX_CARRIER;
		}

		/* activate carrier and DTE */
		if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_DTE_PRESENCE) {
			acmInfo->uartState |= USB_DEVICE_CDC_UART_STATE_RX_CARRIER;
		} else {
			acmInfo->uartState &=
					(uint16_t) ~USB_DEVICE_CDC_UART_STATE_RX_CARRIER;
		}

		/* Indicates to DCE if DTE is present or not */
		acmInfo->dtePresent =
				(acmInfo->dteStatus
						& USB_DEVICE_CDC_CONTROL_SIG_BITMAP_DTE_PRESENCE) ?
						true : false;

		/* Initialize the serial state buffer */
		acmInfo->serialStateBuf[0] = NOTIF_REQUEST_TYPE; /* bmRequestType */
		acmInfo->serialStateBuf[1] = USB_DEVICE_CDC_NOTIF_SERIAL_STATE; /* bNotification */
		acmInfo->serialStateBuf[2] = 0x00; /* wValue */
		acmInfo->serialStateBuf[3] = 0x00;
		acmInfo->serialStateBuf[4] = 0x00; /* wIndex */
		acmInfo->serialStateBuf[5] = 0x00;
		acmInfo->serialStateBuf[6] = UART_BITMAP_SIZE; /* wLength */
		acmInfo->serialStateBuf[7] = 0x00;
		/* Notifiy to host the line state */
		acmInfo->serialStateBuf[4] = acmReqParam->interfaceIndex;
		/* Lower byte of UART BITMAP */
		uartBitmap = (uint8_t *) &acmInfo->serialStateBuf[NOTIF_PACKET_SIZE
				+ UART_BITMAP_SIZE - 2];
		uartBitmap[0] = acmInfo->uartState & 0xFFu;
		uartBitmap[1] = (acmInfo->uartState >> 8) & 0xFFu;
		len = (uint32_t)(NOTIF_PACKET_SIZE + UART_BITMAP_SIZE);
		if (0 == ((usb_device_cdc_acm_struct_t *) handle)->hasSentState) {
			error = USB_DeviceCdcAcmSend(handle,
					USB_CDC_VCOM_INTERRUPT_IN_ENDPOINT, acmInfo->serialStateBuf,
					len);
			if (kStatus_USB_Success != error) {
				usb_echo("kUSB_DeviceCdcEventSetControlLineState error!");
			}
			((usb_device_cdc_acm_struct_t *) handle)->hasSentState = 1;
		}

		/* Update status */
		if (acmInfo->dteStatus
				& USB_DEVICE_CDC_CONTROL_SIG_BITMAP_CARRIER_ACTIVATION) {
			/*  To do: CARRIER_ACTIVATED */
		} else {
			/* To do: CARRIER_DEACTIVATED */
		}
		if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_DTE_PRESENCE) {
			/* DTE_ACTIVATED */
			if (1 == s_cdcVcom.attach) {
				s_cdcVcom.startTransactions = 1;
#if defined(FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED) && (FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED > 0U) && \
    defined(USB_DEVICE_CONFIG_KEEP_ALIVE_MODE) && (USB_DEVICE_CONFIG_KEEP_ALIVE_MODE > 0U) &&             \
    defined(FSL_FEATURE_USB_KHCI_USB_RAM) && (FSL_FEATURE_USB_KHCI_USB_RAM > 0U)
				s_waitForDataReceive = 1;
				USB0->INTEN &= ~USB_INTEN_SOFTOKEN_MASK;
				s_comOpen = 1;
				usb_echo("USB_APP_CDC_DTE_ACTIVATED\r\n");
#endif
			}
		} else {
			/* DTE_DEACTIVATED */
			if (1 == s_cdcVcom.attach) {
				s_cdcVcom.startTransactions = 0;
			}
		}
	}
		break;
	case kUSB_DeviceCdcEventSendBreak:
		break;
	default:
		break;
	}

	return error;
}

/*!
 * @brief USB device callback function.
 *
 * This function handles the usb device specific requests.
 *
 * @param handle          The USB device handle.
 * @param event           The USB device event type.
 * @param param           The parameter of the device specific request.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event,
		void *param) {
	usb_status_t error = kStatus_USB_Error;
	uint16_t *temp16 = (uint16_t *) param;
	uint8_t *temp8 = (uint8_t *) param;

	switch (event) {
	case kUSB_DeviceEventBusReset: {
		s_cdcVcom.attach = 0;
#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)) || \
    (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
		/* Get USB speed to configure the device, including max packet size and interval of the endpoints. */
		if (kStatus_USB_Success
				== USB_DeviceClassGetSpeed(CONTROLLER_ID, &s_cdcVcom.speed)) {
			USB_DeviceSetSpeed(handle, s_cdcVcom.speed);
		}
#endif
	}
		break;
	case kUSB_DeviceEventSetConfiguration:
		if (param) {
			s_cdcVcom.attach = 1;
			s_cdcVcom.currentConfiguration = *temp8;
			if (USB_CDC_VCOM_CONFIGURE_INDEX == (*temp8)) {
				/* Schedule buffer for receive */
				USB_DeviceCdcAcmRecv(s_cdcVcom.cdcAcmHandle,
						USB_CDC_VCOM_BULK_OUT_ENDPOINT, s_currRecvBuf,
						g_UsbDeviceCdcVcomDicEndpoints[0].maxPacketSize);
			}
		}
		break;
	case kUSB_DeviceEventSetInterface:
		if (s_cdcVcom.attach) {
			uint8_t interface = (uint8_t)((*temp16 & 0xFF00U) >> 0x08U);
			uint8_t alternateSetting = (uint8_t)(*temp16 & 0x00FFU);
			if (interface < USB_CDC_VCOM_INTERFACE_COUNT) {
				s_cdcVcom.currentInterfaceAlternateSetting[interface] =
						alternateSetting;
			}
		}
		break;
	case kUSB_DeviceEventGetConfiguration:
		break;
	case kUSB_DeviceEventGetInterface:
		break;
	case kUSB_DeviceEventGetDeviceDescriptor:
		if (param) {
			error = USB_DeviceGetDeviceDescriptor(handle,
					(usb_device_get_device_descriptor_struct_t *) param);
		}
		break;
	case kUSB_DeviceEventGetConfigurationDescriptor:
		if (param) {
			error = USB_DeviceGetConfigurationDescriptor(handle,
					(usb_device_get_configuration_descriptor_struct_t *) param);
		}
		break;
	case kUSB_DeviceEventGetStringDescriptor:
		if (param) {
			/* Get device string descriptor request */
			error = USB_DeviceGetStringDescriptor(handle,
					(usb_device_get_string_descriptor_struct_t *) param);
		}
		break;
	default:
		break;
	}

	return error;
}

/*!
 * @brief Application initialization function.
 *
 * This function initializes the application.
 *
 * @return None.
 */
void USB_DeviceApplicationInit(void) {
	USB_DeviceClockInit();
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
	SYSMPU_Enable(SYSMPU, 0);
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

	s_cdcVcom.speed = USB_SPEED_FULL;
	s_cdcVcom.attach = 0;
	s_cdcVcom.cdcAcmHandle = (class_handle_t) NULL;
	s_cdcVcom.deviceHandle = NULL;

	if (kStatus_USB_Success
			!= USB_DeviceClassInit(CONTROLLER_ID, &s_cdcAcmConfigList,
					&s_cdcVcom.deviceHandle)) {
		usb_echo("USB device init failed\r\n");
	} else {
		//usb_echo("USB device CDC virtual com demo\r\n");
		s_cdcVcom.cdcAcmHandle = s_cdcAcmConfigList.config->classHandle;
	}

	USB_DeviceIsrEnable();

	USB_DeviceRun(s_cdcVcom.deviceHandle);
}

/*!
 * @brief USB task function.
 *
 * This function runs the task for USB device.
 *
 * @return None.
 */
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTask(void *handle)
{
	while (1U)
	{
		USB_DeviceTaskFn(handle);
	}
}
#endif

/*!
 * @brief Application task function.
 *
 * This function runs the task for application.
 *
 * @return None.
 */
void APPTask(void *handle) {
	usb_status_t error = kStatus_USB_Error;

	USB_DeviceApplicationInit();

#if USB_DEVICE_CONFIG_USE_TASK
	if (s_cdcVcom.deviceHandle)
	{
		if (xTaskCreate(USB_DeviceTask, /* pointer to the task                      */
						(char const *)"usb device task", /* task name for kernel awareness debugging */
						5000L / sizeof(portSTACK_TYPE), /* task stack size                          */
						s_cdcVcom.deviceHandle, /* optional task startup argument           */
						5, /* initial priority                         */
						&s_cdcVcom.deviceTaskHandle /* optional task handle to create           */
				) != pdPASS)
		{
			usb_echo("usb device task create failed!\r\n");
			return;
		}
	}
#endif
	int cmd;
	struct message mess;
	static struct wallet_status statusW;
	uint8_t buffer[128] = { 0 };
	uint32_t lenBuf = 0;
	uint32_t send = 0;
	int numCheckPin = 0;
	int pinInit = -1;
	int walletInit = 0x01;
	int receivePntoLcp = 0;
	uint32_t pinDef[34];
	/*buffer[0] = 0x9c;
	buffer[1] = 0x9c;
	buffer[2] = 0x10;
	buffer[3] = 0x9a;
	buffer[4] = 0x9a;

	xQueueSend(lpc_to_pn, buffer, 0);
	for (int i = 0; i < 128; i++) {
		buffer[i] = 0x00;
	}*/
	while (!receivePntoLcp) {


		vTaskDelay(100 / portTICK_PERIOD_MS);
		if(xQueueReceive(pn_to_lpc, buffer, 0)) {
			if ((buffer[0] != 0x9c) || (buffer[1] != 0x9c)
					|| (buffer[2] != 0x10) || (buffer[3 + 35] != 0x9a)
					|| (buffer[4 + 35] != 0x9a)) {/*
				buffer[0] = 0x9c;
				buffer[1] = 0x9c;
				buffer[2] = 0x10;
				buffer[3] = 0x9a;
				buffer[4] = 0x9a;
				xQueueSend(lpc_to_pn, buffer, 0);*/
				for (int i = 0; i < 128; i++) {
					buffer[i] = 0x00;
				}
			} else {

				switch (buffer[3]) {
				case 0: {	//not init
					initWalletCMD(walletInit, 0, pinDef);
					receivePntoLcp = 0;
					break;
				}
				case 1: {	//init
					pinDef[0] = buffer[3];
					pinDef[1] = buffer[4] * 256 + buffer[5];
					for (int i = 0; i < 32; i++) {
						pinDef[2 + i] = buffer[6 + i];
					}
					receivePntoLcp = 1;
					break;
				}
				case 2: {	//block
					receivePntoLcp = 1;
					break;
				}
				}
			}
		}
	}

	for (int i = 0; i < 128; i++) {
		buffer[i] = 0x00;
	}

//read_flash(pinDef, 34, PIN_ADDR);
//initWalletCMD(walletInit, 0, pinDef);
	mess.cmd = WALLET_ENTER_PIN;
	xQueueSend(card_to_lcd, (void* )&mess, 0);

	while (1) {
		if (xQueueReceive(lcd_to_card, (void* )&mess, 0)) {
			cmd = mess.cmd;
			switch (cmd) {
			case WALLET_PINCODE:
				if (pinInit == -1) {
					if (pinDef[1] == *(uint32_t*) mess.data) {
						buffer[0] = 0x9c;
						buffer[1] = 0x9c;
						buffer[2] = 0x30;
						buffer[3] = 0x9a;
						buffer[4] = 0x9a;
						xQueueSend(lpc_to_pn, buffer, 0);
						buffer[0] = 0x00;
						buffer[1] = 0x00;
						buffer[2] = 0x00;
						buffer[3] = 0x00;
						buffer[4] = 0x00;
						numCheckPin = 0;
						pinInit = *(uint32_t*) mess.data;
						char amo[16] = { "0.00000000" };
						char amoD[16] = { "0.00" };
						amoD[4] = 0x00;
						amoD[5] = '\0';
						amo[10] = '\0';
						for (int i = 0; i < 16; i++) {
							statusW.curr[0].amount[i] = amo[i];
							statusW.curr[0].amount_dollars[i] = amoD[i];
							statusW.curr[1].amount[i] = amo[i];
							statusW.curr[1].amount_dollars[i] = amoD[i];
							statusW.curr[2].amount[i] = amo[i];
							statusW.curr[2].amount_dollars[i] = amoD[i];
						}
						statusW.curr[0].curr_name = BTC;
						statusW.curr[1].curr_name = ETH;
						statusW.curr[2].curr_name = LTC;

						statusW.num = 3;
						mess.data = (void*) &statusW;
						mess.cmd = WALLET_STATUS;
						xQueueSend(card_to_lcd, (void* )&mess, 0);
					} else {
						numCheckPin = numCheckPin + 1;
						if (numCheckPin > 2) {
							mess.cmd = WALLET_BLOCKED;
							xQueueSend(card_to_lcd, (void* )&mess, 0);
							vTaskDelay(5000);
							initWalletCMD(walletInit, 1, pinDef);
							mess.cmd = WALLET_ENTER_PIN;
							xQueueSend(card_to_lcd, (void* )&mess, 0);
						} else {
							int ostNum = 3 - numCheckPin;
							mess.data = (void*) &ostNum;
							mess.cmd = WALLET_WRONG_PINCODE;
							xQueueSend(card_to_lcd, (void* )&mess, 0);
						}
					}
				}
				break;
			case WALLET_CMD_CLEAR:
				pinInit = -1;
				for (int i = 0; i < 34; i++) {
					pinDef[i] = '\0';
				}
				for (int i = 0; i < 128; i++) {
					buffer[i] = '\0';
				}
				/*buffer[0] = 0x9c;
				buffer[1] = 0x9c;
				buffer[2] = 0x20;
				buffer[38] = 0x9a;
				buffer[39] = 0x9a;
				xQueueSend(lpc_to_pn, buffer, 0);
				for (int i = 0; i < 128; i++) {
					buffer[i] = 0x00;
				}*/
				/*while (!xQueueReceive(pn_to_lpc, buffer, 0)) {
				}
				for (int i = 0; i < 128; i++) {
					buffer[i] = 0x00;
				}*/
				//write_flash(pinDef, 34, PIN_ADDR);
				initWalletCMD(walletInit, 0, pinDef);
				mess.cmd = WALLET_ENTER_PIN;
				xQueueSend(card_to_lcd, (void* )&mess, 0);
				break;
			}
		}
		if (xQueueReceive(pn_to_lpc, buffer, 0) && pinInit != -1) {
			uint8_t sendBuffer[128] = { 0 };
			dataInPN(buffer, sendBuffer, &lenBuf, &send, &pinInit, pinDef);

			if (send > 0 && sendBuffer[0] != 0) {
				//portENTER_CRITICAL();
				vTaskDelay(100 / portTICK_PERIOD_MS);
				xQueueSend(lpc_to_pn, sendBuffer, 0);
				//portEXIT_CRITICAL();
				send = 0;
				lenBuf = 0;
				for (int i = 0; i < 128; i++) {
					buffer[i] = 0x00;
					sendBuffer[i] = 0x00;
				}

			}

		}
#ifdef usbConnect
		if ((1 == s_cdcVcom.attach) && (1 == s_cdcVcom.startTransactions))
		{
			/* User Code */
			if ((0 != s_recvSize) && (0xFFFFFFFF != s_recvSize))
			{
				int32_t i;
				/* Copy Buffer to Send Buff */
				for (i = 0; i < s_recvSize; i++)
				{
					s_currSendBuf[s_sendSize++] = s_currRecvBuf[i];
				}
				s_recvSize = 0;
			}
			if (s_sendSize)
			{
				uint32_t size = s_sendSize;
				s_sendSize = 0;
				if(numCheckPin<3)
				{
					dataToBuffer(s_currSendBuf,&size, buffer, &lenBuf, &send, &pinInit, pinDef);
				}
				if(send>0)
				{
					//portENTER_CRITICAL();
					error = USB_DeviceCdcAcmSend(s_cdcVcom.cdcAcmHandle, USB_CDC_VCOM_BULK_IN_ENDPOINT, buffer, lenBuf);
					//portEXIT_CRITICAL();
					send=0;
					lenBuf = 0;
					for(int i =0; i<1800; i++)
					{
						buffer[i] = 0x00;
					}
					if (error != kStatus_USB_Success)
					{
						// Failure to send Data Handling code here
					}
				} else {USB_DeviceCdcAcmSend(s_cdcVcom.cdcAcmHandle, USB_CDC_VCOM_BULK_IN_ENDPOINT, buffer, 0);}
			}
#if defined(FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED) && (FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED > 0U) && \
    defined(USB_DEVICE_CONFIG_KEEP_ALIVE_MODE) && (USB_DEVICE_CONFIG_KEEP_ALIVE_MODE > 0U) &&             \
    defined(FSL_FEATURE_USB_KHCI_USB_RAM) && (FSL_FEATURE_USB_KHCI_USB_RAM > 0U)
			if ((s_waitForDataReceive))
			{
				if (s_comOpen == 1)
				{
					/* Wait for all the packets been sent during opening the com port. Otherwise these packets may
					 * wake up the system.
					 */
					usb_echo("Waiting to enter lowpower ...\r\n");
					for (uint32_t i = 0U; i < 16000000U; ++i)
					{
						__ASM("NOP"); /* delay */
					}

					s_comOpen = 0;
				}
				usb_echo("Enter lowpower\r\n");
				BOARD_DbgConsole_Deinit();
				USB0->INTEN &= ~USB_INTEN_TOKDNEEN_MASK;
				if (SysTick->CTRL & SysTick_CTRL_ENABLE_Msk)
				{
					SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
				}
				USB_EnterLowpowerMode();

				if (SysTick->CTRL & SysTick_CTRL_ENABLE_Msk)
				{
					SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
				}
				s_waitForDataReceive = 0;
				USB0->INTEN |= USB_INTEN_TOKDNEEN_MASK;
				BOARD_DbgConsole_Init();
				usb_echo("Exit  lowpower\r\n");
			}
#endif
		}
#endif
	}
}

#if defined(__CC_ARM) || defined(__GNUC__)
int usb_init(void)
#else
void usb_init(void)
#endif
{
#if (defined USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS)
	POWER_DisablePD(kPDRUNCFG_PD_USB1_PHY);
	/* enable usb1 host clock */
	CLOCK_EnableClock(kCLOCK_Usbh1);
	*((uint32_t *) (USBHSH_BASE + 0x50)) |= USBHSH_PORTMODE_DEV_ENABLE_MASK;
	/* enable usb1 host clock */
	CLOCK_DisableClock(kCLOCK_Usbh1);
#endif
#if (defined USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS)
	POWER_DisablePD(kPDRUNCFG_PD_USB0_PHY); /*< Turn on USB Phy */
	CLOCK_SetClkDiv(kCLOCK_DivUsb0Clk, 1, false);
	CLOCK_AttachClk(kFRO_HF_to_USB0_CLK);
	/* enable usb0 host clock */
	CLOCK_EnableClock(kCLOCK_Usbhsl0);
	*((uint32_t *)(USBFSH_BASE + 0x5C)) |= USBFSH_PORTMODE_DEV_ENABLE_MASK;
	/* disable usb0 host clock */
	CLOCK_DisableClock(kCLOCK_Usbhsl0);
#endif

	if (xTaskCreate(APPTask, /* pointer to the task                      */
	s_appName, /* task name for kernel awareness debugging */
	15000L / sizeof(portSTACK_TYPE), /* task stack size                          */
	&s_cdcVcom, /* optional task startup argument           */
	3, /* initial priority                         */
	&s_cdcVcom.applicationTaskHandle /* optional task handle to create           */
	) != pdPASS) {
		usb_echo("app task create failed!\r\n");
#if (defined(__CC_ARM) || defined(__GNUC__))
		return 1;
#else
		return;
#endif
	}

#if (defined(__CC_ARM) || defined(__GNUC__))
	return 1;
#endif
}

static void write_flash(uint32_t *data, uint32_t size, uint32_t addr) {
	if (size > FSL_FEATURE_EEPROM_SIZE / 4)
		size = FSL_FEATURE_EEPROM_SIZE / 4;

	if ((addr + size) > (FSL_FEATURE_EEPROM_SIZE / 4))
		return;

	for (int i = 0; i < size; i++) {
		EEPROM_WriteWord(EEPROM, addr + i * 4, data[i]);
	}
}

static void read_flash(uint32_t *data, uint32_t size, uint32_t addr) {
	if (size > FSL_FEATURE_EEPROM_SIZE / 4)
		size = FSL_FEATURE_EEPROM_SIZE / 4;

	if ((addr + size) > (FSL_FEATURE_EEPROM_SIZE / 4))
		return;

	for (int i = 0; i < size; i++) {
		data[i] = *(uint32_t*) (FSL_FEATURE_EEPROM_BASE_ADDRESS + addr + i * 4);
	}
}

void initWalletCMD(int walletInit, int blocked, uint32_t *pinDef) {
	uint8_t buffer[40] = { 0 };
	uint8_t seed[32] = { 0 };
	struct message messRes;
	int initWallet = 0;
	int restoreWalletPin = 0;
	int restoreWalletMS = 0;
	while (pinDef[0] != walletInit || blocked == 1) {
		blocked = 0;
		BYTE pin[5] = { 0 };
		pin[4] = '\0';
		int lenMnem = 0;
		BYTE mnemonic[120] = { 0 };
		initWallet = 0;
		restoreWalletPin = 0;
		restoreWalletMS = 0;
		struct message messInit;
		vTaskDelay(500 / portTICK_PERIOD_MS);
		messInit.cmd = WALLET_INIT;
		xQueueSend(card_to_lcd, (void* )&messInit, 0);
		while (!initWallet) {
			if (xQueueReceive(lcd_to_card, (void* )&messInit, 0)) {
				int cmd = messInit.cmd;
				switch (cmd) {
				case WALLET_CMD_RESTORE:
					messRes.cmd = WALLET_ENTER_PIN;
					xQueueSend(card_to_lcd, (void* )&messRes, 0);
					while (!restoreWalletPin) {
						if (xQueueReceive(lcd_to_card, (void* )&messRes, 0)) {
							cmd = messRes.cmd;
							switch (cmd) {
							case WALLET_PINCODE:
								pinDef[1] = *(uint32_t*) messRes.data;
								pin[0] = (pinDef[1] - pinDef[1] % 1000) / 1000
										+ 48;
								pin[1] = ((pinDef[1] - pinDef[1] % 100) / 100)
										% 10 + 48;
								pin[2] = ((pinDef[1] - pinDef[1] % 10) / 10)
										% 10 + 48;
								pin[3] = pinDef[1] % 10 + 48;
								mnemonicGenerate(pin, mnemonic, &lenMnem, 128);
								restoreWalletPin = 1;
								messRes.cmd = WALLET_ENTER_MS;
								xQueueSend(card_to_lcd, (void* )&messRes, 0);
								while (!restoreWalletMS) {
									if (xQueueReceive(lcd_to_card,
											(void* )&messRes, 0)) {
										int pinToSeed = 1;
										cmd = messRes.cmd;
										switch (cmd) {
										case WALLET_MNEMONIC:
											if (strncmp((char*) mnemonic,
													(char*) messRes.data,
													lenMnem)) {
												pinToSeed = 0;
											}
											if (pinToSeed == 1) {
												pinDef[0] = walletInit;
												seedFromMnemonic(mnemonic,
														lenMnem, pin, 4, seed);
												for (int i = 0; i < 32; i++) {
													pinDef[i + 2] = seed[i];
												}
												buffer[0] = 0x9c;
												buffer[1] = 0x9c;
												buffer[2] = 0x20;
												buffer[3] = 0x01;
												buffer[4] = (pinDef[1]
														- pinDef[1] % 256)
														/ 256;
												buffer[5] = pinDef[1] % 256;
												for (int i = 0; i < 32; i++) {
													buffer[i + 6] =
															pinDef[i + 2];
												}
												buffer[38] = 0x9a;
												buffer[39] = 0x9a;
												xQueueSend(lpc_to_pn, buffer, 0);
												vTaskDelay(150 / portTICK_PERIOD_MS);
												xQueueSend(lpc_to_pn, buffer, 0);
												vTaskDelay(150 / portTICK_PERIOD_MS);
												xQueueSend(lpc_to_pn, buffer, 0);
												vTaskDelay(150 / portTICK_PERIOD_MS);
												xQueueSend(lpc_to_pn, buffer, 0);
												for (int i = 0; i < 39;
														i++) {
													buffer[i] = 0x00;
												}
												/*while (!xQueueReceive(pn_to_lpc,
														buffer, 0)) {
													for (int i = 0; i < 128;
															i++) {
														buffer[i] = 0x00;
													}
												}*/
											}
											restoreWalletMS = 1;
											initWallet = 1;
											break;
										case WALLET_CANCEL_PRESSED:
											initWallet = 1;
											restoreWalletPin = 1;
											restoreWalletMS = 1;
											break;
										}
									}
								}
								break;
							case WALLET_CANCEL_PRESSED:
								initWallet = 1;
								restoreWalletPin = 1;
								break;
							}
						}
					}
					break;
				case WALLET_CMD_INIT:
					messInit.cmd = WALLET_SET_PIN;
					xQueueSend(card_to_lcd, (void* )&messInit, 0);
					while (!initWallet) {
						if (xQueueReceive(lcd_to_card, (void* )&messInit, 0)) {
							int cmd = messInit.cmd;
							switch (cmd) {
							case WALLET_PINCODE:
								pinDef[1] = *(uint32_t*) messInit.data;
								pin[0] = (pinDef[1] - pinDef[1] % 1000) / 1000
										+ 48;
								pin[1] = ((pinDef[1] - pinDef[1] % 100) / 100)
										% 10 + 48;
								pin[2] = ((pinDef[1] - pinDef[1] % 10) / 10)
										% 10 + 48;
								pin[3] = pinDef[1] % 10 + 48;
								mnemonicGenerate(pin, mnemonic, &lenMnem, 128);
								seedFromMnemonic(mnemonic, lenMnem, pin, 4,
										seed);
								pinDef[0] = walletInit;
								for (int i = 0; i < 32; i++) {
									pinDef[i + 2] = seed[i];
								}
								buffer[0] = 0x9c;
								buffer[1] = 0x9c;
								buffer[2] = 0x20;
								buffer[3] = 0x01;
								buffer[4] = (pinDef[1] - pinDef[1] % 256) / 256;
								buffer[5] = pinDef[1] % 256;
								for (int i = 0; i < 32; i++) {
									buffer[i + 6] = pinDef[i + 2];
								}
								buffer[38] = 0x9a;
								buffer[39] = 0x9a;
								xQueueSend(lpc_to_pn, buffer, 0);
								vTaskDelay(150 / portTICK_PERIOD_MS);
								xQueueSend(lpc_to_pn, buffer, 0);
								vTaskDelay(150 / portTICK_PERIOD_MS);
								xQueueSend(lpc_to_pn, buffer, 0);
								vTaskDelay(150 / portTICK_PERIOD_MS);
								xQueueSend(lpc_to_pn, buffer, 0);

								for (int i = 0; i < 39; i++) {
									buffer[i] = 0x00;
								}

								messInit.cmd = WALLET_SET_MS;
								mnemonic[lenMnem] = '\0';
								messInit.data = (void*) mnemonic;
								initWallet = 1;
								xQueueSend(card_to_lcd, (void* )&messInit, 0);
								int temp = 0;
								while (!temp) {
									if (xQueueReceive(lcd_to_card,
											(void* )&messInit, 0)) {
										temp = 1;
									}
								}
								break;
							case WALLET_CANCEL_PRESSED:
								initWallet = 1;
								break;
							}
						}
					}
					break;
				}
			}
		}
	}
}


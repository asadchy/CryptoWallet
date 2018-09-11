/******************************************************************************
 *
 * @brief     This file is part of the TouchGFX 4.8.0 evaluation distribution.
 *
 * @author    Draupner Graphics A/S <http://www.touchgfx.com>
 *
 ******************************************************************************
 *
 * @section Copyright
 *
 * This file is free software and is provided for example purposes. You may
 * use, copy, and modify within the terms and conditions of the license
 * agreement.
 *
 * This is licensed software for evaluation use, any use must strictly comply
 * with the evaluation license agreement provided with delivery of the
 * TouchGFX software.
 *
 * The evaluation license agreement can be seen on www.touchgfx.com
 *
 * @section Disclaimer
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Draupner Graphics A/S has
 * no obligation to support this software. Draupner Graphics A/S is providing
 * the software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Draupner Graphics A/S can not be held liable for any consequential,
 * incidental, or special damages, or any other relief, or for any claim by
 * any third party, arising from your use of this software.
 *
 *****************************************************************************/
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/hal/BoardConfiguration.hpp>
#include <touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp>

using namespace touchgfx;

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "data.hpp"

extern "C" {
	extern void usb_init(void);
	#include "communication.h"
}

/**
 * Define the FreeRTOS task priorities and stack sizes
 */
#define configGUI_TASK_PRIORITY                 ( tskIDLE_PRIORITY + 4 )

#define configGUI_TASK_STK_SIZE                 ( 950 )

//#define CANVAS_BUFFER_SIZE (3600)

static void GUITask(void* params)
{
    touchgfx::HAL::getInstance()->taskEntry();
}

int main(void)
{
    hw_init();
    touchgfx_init();

    lcd_to_card = xQueueCreate(QUEUE_SIZE, sizeof(struct message));
    if(lcd_to_card == 0)
    {
    	while(1);
    }

    card_to_lcd = xQueueCreate(QUEUE_SIZE, sizeof(struct message));
    if(card_to_lcd == 0)
    {
    	while(1);
    }

    xTaskCreate(GUITask, (TASKCREATE_NAME_TYPE)"GUITask",
                configGUI_TASK_STK_SIZE,
                NULL,
                configGUI_TASK_PRIORITY,
                NULL);

    //usb_init();

    xTaskCreate(Communication_Task, (TASKCREATE_NAME_TYPE)"I2CTask",
				configGUI_TASK_STK_SIZE,
				NULL,
				configGUI_TASK_PRIORITY - 1,
				NULL);


    vTaskStartScheduler();

    for (;;);

}


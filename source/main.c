/* Copyright 2025, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
 * Copyright 2023, Guido Cicconi
 * Copyright 2025, Juan Ignacio Biener
 * All rights reserved.
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
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */


/*==================[inclusions]=============================================*/

#include "SD2_board.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "oled.h"

/*==================[macros and definitions]=================================*/

#define TIEMPO_OLED 2000

typedef enum
{
    OLED_DISPLAY_1 = 0,
	OLED_DISPLAY_2,
}estDisplay_enum;

/*==================[internal data declaration]==============================*/

static int32_t Timer_OLED; // Temporizador
static estDisplay_enum Display_Act = OLED_DISPLAY_1; // Temporizador

/*==================[internal functions declaration]=========================*/

void Oled_Display_1(void)
{
	oled_clearScreen(OLED_COLOR_BLACK);
	/* Drawing */
	oled_fillRect(32, 16, 32+64, 16+32, OLED_COLOR_WHITE);
	oled_fillRect(32+8, 16+8, 32+64-8, 16+32-8, OLED_COLOR_BLACK);
	oled_putString(41, 29, (uint8_t *)"SD2-2025", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
	oled_circle(64, 32, 31, OLED_COLOR_WHITE);
}

void Oled_Display_2(void)
{
	oled_clearScreen(OLED_COLOR_WHITE);
	/* Drawing */
	oled_fillRect(32, 16, 32+64, 16+32, OLED_COLOR_BLACK);
	oled_fillRect(32+8, 16+8, 32+64-8, 16+32-8, OLED_COLOR_WHITE);
	oled_putString(41, 29, (uint8_t *)"SD2-2025", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
	oled_circle(64, 32, 31, OLED_COLOR_BLACK);

}

int main(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    board_init();
    board_configSPI0();
    SysTick_Config(SystemCoreClock / 1000U);
    Timer_OLED = TIEMPO_OLED;

	oled_init();
	oled_setContrast(16);
	Oled_Display_2();

    while(1) {
    	if (Timer_OLED == 0)
    	{
    		Timer_OLED = TIEMPO_OLED;
    		switch (Display_Act)
			{
				case OLED_DISPLAY_1:
					Oled_Display_1();
					Display_Act = OLED_DISPLAY_2;
				break;

				case OLED_DISPLAY_2:
					Oled_Display_2();
					Display_Act = OLED_DISPLAY_1;
				break;

				default:
					Display_Act = OLED_DISPLAY_1;
				break;
			};
    	}
    }

    return 0;
}

void SysTick_Handler(void)
{
	if (Timer_OLED > 0) {
		Timer_OLED--;
	}
}

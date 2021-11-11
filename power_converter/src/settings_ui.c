#include <stdio.h>
#include <xuartps_hw.h>

#include <zynq_registers.h>

#include "settings_ui.h"

uint8_t operating_mode = SETTING_MODE;

void init_settings_ui() {

	// Set LEDs as output
	AXI_LED_TRI &= ~0xF;
	// Set leds
	AXI_LED_DATA = operating_mode;

	xil_printf("--==[ PowerConverter ][ version 1.0 ]==--\r\n");

}

// switch operating mode
void switch_mode() {

	xil_printf("Operating mode ");

	switch (operating_mode) {
	case SETTING_MODE:
		operating_mode = IDLE_MODE;
		xil_printf("IDLE");
		break;
	case IDLE_MODE:
		operating_mode = MODULATING_MODE;
		xil_printf("MODULATING");
		break;
	case MODULATING_MODE:
		operating_mode = SETTING_MODE;
		xil_printf("SETTING");
		break;
	}

	xil_printf(" selected\r\n");

	// Set leds
	AXI_LED_DATA = operating_mode;
}


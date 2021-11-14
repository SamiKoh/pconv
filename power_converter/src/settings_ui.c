#include <stdio.h>
#include <xuartps_hw.h>

#include <zynq_registers.h>

#include "settings_ui.h"

uint8_t operating_mode = IDLE_MODE; // default operating mode
uint8_t selected_parameter = KI_PARAM; // default parameter

void init_settings_ui() {

	// Set LEDs as output
	AXI_LED_TRI &= ~0xF;
	// Set leds
	AXI_LED_DATA = operating_mode;

	xil_printf("\r\n--==[ PowerConverter ][ version 1.0 ]==--\r\n");

}

// switch operating mode
void switch_mode() {

	switch (operating_mode) {
	case SETTING_MODE:
		operating_mode = IDLE_MODE;
		break;
	case IDLE_MODE:
		operating_mode = MODULATING_MODE;
		break;
	case MODULATING_MODE:
		operating_mode = SETTING_MODE;
		break;
	}

	// Set leds
	AXI_LED_DATA = operating_mode;

	display_mode_status();

}

void display_mode_status() {
	xil_printf("\r\n-> Operating mode\r\n");
	for (uint8_t i = 0; i < 25; i++)
		xil_printf("=");
	xil_printf("\r\n");
	txt_list((operating_mode == MODULATING_MODE), "MODULATING");
	txt_list((operating_mode == IDLE_MODE), "IDLE");
	txt_list((operating_mode == SETTING_MODE), "SETTING");
	xil_printf("\r\n");
}

// switch parameter
void switch_parameter() {

	if (operating_mode != SETTING_MODE)
		return; // return if not in setting mode

	switch (selected_parameter) {
	case KP_PARAM:
		selected_parameter = KI_PARAM;
		break;
	case KI_PARAM:
		selected_parameter = KP_PARAM;
		break;
	}
	display_parameter_status();
}

void display_parameter_status() {
	xil_printf("\r\n-> Switch parameter\r\n");
	for (uint8_t i = 0; i < 25; i++)
		xil_printf("=");
	xil_printf("\r\n");
	txt_list((selected_parameter == KP_PARAM), "KP");
	txt_list((selected_parameter == KI_PARAM), "KI");
	xil_printf("\r\n");
}

void change_parameter_value(unsigned int kp, unsigned int ki) {
	(selected_parameter == KP_PARAM) ?
			xil_printf("KP value: %d", kp) : xil_printf("KI value: %d", ki);
}

void txt_list(uint8_t is_selected, char *txt) {
	is_selected ? xil_printf(" [*%s ] ", txt) : xil_printf("   %s   ", txt);
}

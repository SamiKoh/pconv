#include <stdio.h>
#include <zynq_registers.h>
#include <xuartps_hw.h>
#include "buttons.h"

void init_buttons() {
	// set all buttons to input state
	AXI_BTN_TRI |= 0xF;
}

// is button pressed and released?
uint8_t is_button_pressed() {

	// only one button is pressed at once
	if (AXI_BTN_DATA == 0b0000 || AXI_BTN_DATA == 0b0001
			|| AXI_BTN_DATA == 0b0010 || AXI_BTN_DATA == 0b0100
			|| AXI_BTN_DATA == 0b1000) {

		//static unsigned char btn_mode_old = 0;
		static reg_btns button_status_old = BTN_DEFAULTS;

		if ((button_status_old.btn_mode == 1) && (buttons.btn_mode == 0)) {
			button_status_old = buttons;
			return B_MODE;
		} else if ((button_status_old.btn_parameter == 1)
				&& (buttons.btn_parameter == 0)) {
			button_status_old = buttons;
			return B_PARAMETER;
		} else if ((button_status_old.btn_dec == 1) && (buttons.btn_dec == 0)) {
			button_status_old = buttons;
			return B_DECREASE;
		} else if ((button_status_old.btn_inc == 1) && (buttons.btn_inc == 0)) {
			button_status_old = buttons;
			return B_INCREASE;
		}

		button_status_old = buttons;
	}
	return B_NONE;
}

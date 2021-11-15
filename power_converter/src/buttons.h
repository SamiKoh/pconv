#ifndef BUTTONS_UI_H
#define BUTTONS_UI_H

typedef struct {

	unsigned int btn_mode :1;
	unsigned int btn_parameter :1;
	unsigned int btn_dec :1;
	unsigned int btn_inc :1;

} reg_btns;

#define buttons (*(volatile reg_btns *)(AXI_BTN_DATA_ADDRESS))
#define BTN_DEFAULTS { 0, 0, 0, 0 }

enum {
	B_NONE = -1, B_MODE = 10, B_PARAMETER, B_DECREASE, B_INCREASE
};

void init_buttons();
uint8_t is_button_pressed();

#endif

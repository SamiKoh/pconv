#include <zynq_registers.h>

typedef struct {

	unsigned int btn_mode :1;
	unsigned int btn_parameter :1;
	unsigned int btn_dec :1;
	unsigned int btn_inc :1;

} reg_btns;

#define buttons (*(volatile reg_btns *)(ADDRESS))

void init_buttons() {

	// set all buttons to input state
	AXI_BTN_TRI |= 0xF;

}


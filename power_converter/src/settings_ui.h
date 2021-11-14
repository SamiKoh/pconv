#ifndef SETTINGS_UI_H
#define SETTINGS_UI_H


/* Different operating modes */
enum {
	SETTING_MODE = 0b0001, IDLE_MODE = 0b0010, MODULATING_MODE = 0b0100
} op_modes;

enum {
	KP_PARAM = 1, KI_PARAM
} pid_params;

#define OPERATION_INC 0
#define OPERATION_DEC 1

extern void init_settings_ui();
extern void switch_mode();
extern void switch_parameter();

extern void change_parameter_value(uint8_t operation, unsigned int *kp, unsigned int *ki);

static void display_mode_status();
static void display_parameter_status();
static void txt_list(uint8_t is_selected, char *txt);

#endif

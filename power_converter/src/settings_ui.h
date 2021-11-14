#ifndef SETTINGS_UI_H
#define SETTINGS_UI_H


/* Different operating modes */
enum {
	SETTING_MODE = 0b0001, IDLE_MODE = 0b0010, MODULATING_MODE = 0b0100
} op_modes;

enum {
	KP_PARAM = 1, KI_PARAM
} pid_params;


extern void init_settings_ui();
extern void switch_mode();
static void display_mode_status();
static void display_parameter_status();
static void txt_list(uint8_t is_selected, char *txt);

#endif

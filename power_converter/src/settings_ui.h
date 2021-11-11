#ifndef SETTINGS_UI_H
#define SETTINGS_UI_H


/* Different operating modes */
enum {
	SETTING_MODE = 0b0001, IDLE_MODE = 0b0010, MODULATING_MODE = 0b0100
} op_modes;

void init_settings_ui();
void switch_mode();

#endif

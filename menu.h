#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "controller_a.h"

void show_menu(ALLEGRO_BITMAP* menu_background, ALLEGRO_FONT* font);
void menu_keydown(unsigned char *key, CONTROLLER_A* controller_a, ALLEGRO_SAMPLE *sound_selection, ALLEGRO_SAMPLE_ID* sound_selection_id, ALLEGRO_SAMPLE *sound_start, ALLEGRO_SAMPLE_ID* sound_start_id);


#endif // MENU_H_INCLUDED

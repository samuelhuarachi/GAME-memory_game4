#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <iostream>
#include <vector>
#include "controller_a.h"
#include "turn.h"

int axis_x = 325;
int axis_y = 200;

void show_menu_player(ALLEGRO_BITMAP* menu_background, ALLEGRO_FONT* font, CONTROLLER_A *controller_a) {

    al_draw_tinted_bitmap(menu_background, al_map_rgba_f(1, 1, 1, 1), 0, 0, 0);
    al_draw_filled_rectangle(300,210, 500, 277, al_map_rgb(0, 0, 0));
    controller_a->total_players;

    if (controller_a->total_players == 1) {
        al_draw_text(font, al_map_rgb(234, 234, 28), axis_x, axis_y + 30, 0, "1 player");
        al_draw_text(font, al_map_rgb(255, 255, 255), axis_x, axis_y + 50, 0, "2 players");
    } else {
        al_draw_text(font, al_map_rgb(255, 255, 255), axis_x, axis_y + 30, 0, "1 player");
        al_draw_text(font, al_map_rgb(234, 234, 28), axis_x, axis_y + 50, 0, "2 players");
    }
}

void menu_player_keydown(unsigned char *key, CONTROLLER_A *controller_a, ALLEGRO_SAMPLE *sound_selection, ALLEGRO_SAMPLE_ID *sound_selection_id) {
    if(key[ALLEGRO_KEY_UP]) {
         al_play_sample(sound_selection, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, sound_selection_id);
         controller_a->total_players = 1;
         return;
    }

    if(key[ALLEGRO_KEY_DOWN]) {
        al_play_sample(sound_selection, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, sound_selection_id);
        controller_a->total_players = 2;
        return;
    }

    if(key[ALLEGRO_KEY_ENTER]) {
        controller_a->turn = TURN::ENDGAME;
        controller_a->cutscene = CUTSCENE::MENU;
    }
}

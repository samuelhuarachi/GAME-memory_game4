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


using namespace std;

int MENU_OPTION_SELECTED = CUTSCENE::INGAME;

void previous() {
    if (MENU_OPTION_SELECTED == CUTSCENE::EXIT) {
        MENU_OPTION_SELECTED = CUTSCENE::TOTAL_PLAYERS;
    } else if (MENU_OPTION_SELECTED == CUTSCENE::TOTAL_PLAYERS) {
        MENU_OPTION_SELECTED = CUTSCENE::INGAME;
    }
}

void next() {
    if (MENU_OPTION_SELECTED == CUTSCENE::INGAME) {
        MENU_OPTION_SELECTED = CUTSCENE::TOTAL_PLAYERS;
    } else if (MENU_OPTION_SELECTED == CUTSCENE::TOTAL_PLAYERS) {
        MENU_OPTION_SELECTED = CUTSCENE::EXIT;
    }
}

void show_menu(ALLEGRO_BITMAP* menu_background, ALLEGRO_FONT* font) {

    al_draw_tinted_bitmap(menu_background, al_map_rgba_f(1, 1, 1, 1), 0, 0, 0);
    al_draw_filled_rectangle(300,200, 500, 287, al_map_rgb(0, 0, 0));

    int axis_x = 325;
    int axis_y = 200;

    if (MENU_OPTION_SELECTED == CUTSCENE::INGAME) {
        al_draw_text(font, al_map_rgb(230, 255, 0), axis_x, axis_y + 20, 0, "Jogar");
    } else {
        al_draw_text(font, al_map_rgb(255, 255, 255), axis_x, axis_y + 20, 0, "Jogar");
    }

    if (MENU_OPTION_SELECTED == CUTSCENE::TOTAL_PLAYERS) {
        al_draw_text(font, al_map_rgb(234, 234, 28), axis_x, axis_y + 40, 0, "Total players");
    } else {
        al_draw_text(font, al_map_rgb(255, 255, 255), axis_x, axis_y + 40, 0, "Total players");
    }

    if (MENU_OPTION_SELECTED == CUTSCENE::EXIT) {
        al_draw_text(font, al_map_rgb(234, 234, 28), axis_x, axis_y + 60, 0, "Sair");
    } else {
        al_draw_text(font, al_map_rgb(255, 255, 255), axis_x, axis_y + 60, 0, "Sair");
    }
}

void menu_keydown(unsigned char *key, CONTROLLER_A *controller_a, ALLEGRO_SAMPLE *sound_selection, ALLEGRO_SAMPLE_ID *sound_selection_id, ALLEGRO_SAMPLE *sound_start, ALLEGRO_SAMPLE_ID* sound_start_id) {
    if(key[ALLEGRO_KEY_UP]) {
        al_play_sample(sound_selection, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, sound_selection_id);
        previous();
    } else if(key[ALLEGRO_KEY_DOWN]) {
        al_play_sample(sound_selection, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, sound_selection_id);
        next();
    } else if(key[ALLEGRO_KEY_ENTER]) {
        if (MENU_OPTION_SELECTED == CUTSCENE::INGAME) {
            al_play_sample(sound_start, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, sound_start_id);
            controller_a->cutscene = CUTSCENE::INGAME;
        } else if (MENU_OPTION_SELECTED == CUTSCENE::EXIT) {
            controller_a->cutscene = CUTSCENE::EXIT;
        } else if (MENU_OPTION_SELECTED == CUTSCENE::TOTAL_PLAYERS) {
            controller_a->cutscene = CUTSCENE::TOTAL_PLAYERS;
        }
    }
}






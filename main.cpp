#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include <stdlib.h>
#include <typeinfo>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <math.h>
#include <array>
#include <iterator>
#include <cmath>
#include <tuple>

using namespace std;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::ifstream;
using std::vector;

#define KEY_SEEN     1
#define KEY_RELEASED 2
#include "Cards.h"
#include "Card.h"
#include "menu.h"
#include "controller_a.h"
#include "menu_player.h"


int CARD_DIMENSION_WIDTH = 100;
int CARD_DIMENSION_HEIGHT = 100;
bool ALL_CARDS_ARE_OPENED = true;

ALLEGRO_BITMAP* background;
ALLEGRO_BITMAP* selection;
ALLEGRO_FONT* font;
ALLEGRO_BITMAP* menu_background;
ALLEGRO_BITMAP* game_background;

bool is_pressing_the_key(int key) {
    if (key) {
        return true;
    }
    return false;
}

CONTROLLER_A controller_a;


void controller_a_restart(CONTROLLER_A* controller_a, Cards* cards_nivel) {
    cards_nivel->shuffle_cards();
    controller_a->turn = TURN::PRESENTATION;

    int score = 0;
    controller_a->player1->setScore(score);
    controller_a->player2->setScore(score);
}

void controller_a_set_line_column_by_key_direction_pressed(unsigned char *key, CONTROLLER_A* controller_a) {
    if(key[ALLEGRO_KEY_UP]) {
        controller_a->line = controller_a->line - 1;
    }

    if(key[ALLEGRO_KEY_DOWN]) {
        controller_a->line = controller_a->line + 1;
    }

    if(key[ALLEGRO_KEY_LEFT]) {
        controller_a->column = controller_a->column - 1;
    }

    if(key[ALLEGRO_KEY_RIGHT]) {
        controller_a->column = controller_a->column + 1;
    }

    if (controller_a->line < 0) {
        controller_a->line = 0;
    }

    if (controller_a->line > 3) {
        controller_a->line = 3;
    }

    if (controller_a->column < 0) {
        controller_a->column = 0;
    }

    if (controller_a->column > 6) {
        controller_a->column = 6;
    }
}

bool is_card_got_it_right(CONTROLLER_A* controller_a) {
    if (controller_a->card1->getIdentifier() == controller_a->card2->getIdentifier()) {
        return true;
    }
    return false;
}

void show_cards(Cards* cards_nivel) {
    int column_x = 0;
    int card_position_x = 2;
    int card_position_y = 90;
    int card_position_deslocation = 103;
    int margin_left = 37;
    int jump_line = 103;
    int card_position_x_initial = card_position_x + margin_left;
    int card_position_y_initial = card_position_y;
    card_position_x = card_position_x + margin_left;
    int period = 1;
    Card *some_card;

    ALL_CARDS_ARE_OPENED = true;
    for (unsigned int i = 0; i < cards_nivel->cards.size(); i++) {
        some_card = cards_nivel->cards[i];

        if (some_card->isFaceDown) {
            ALL_CARDS_ARE_OPENED = false;
            al_draw_bitmap_region(background, 0, 0, CARD_DIMENSION_WIDTH, CARD_DIMENSION_HEIGHT, card_position_x, card_position_y, 0);
        } else {
            al_draw_bitmap_region(some_card->getImg(), 0, 0, CARD_DIMENSION_WIDTH, CARD_DIMENSION_HEIGHT, card_position_x, card_position_y, 0);
        }


        card_position_x = card_position_x + card_position_deslocation;
        if (period == 7) {
            card_position_y = card_position_y + jump_line;
            card_position_x = card_position_x_initial;
            period = 0;
        }
        column_x = column_x + 10;
        period = period + 1;
    } // for

     // Selection
     al_draw_bitmap_region(selection, 0, 0, 100, 100, card_position_x_initial + (controller_a.column * card_position_deslocation) + 60 , card_position_y_initial + (controller_a.line * card_position_deslocation) + 40, 0);

}

void controller_a_selection_by_space_pressed(CONTROLLER_A* controller_a, Cards* cards_nivel1) {

    int card_selected = controller_a->line * 7 + controller_a->column;
    Card* some_card = cards_nivel1->cards[card_selected];

    if (controller_a->turn == TURN::OPEN_FIRST && some_card->isFaceDown == true) {

        some_card->isFaceDown = false;
        controller_a->card1 = some_card;
        controller_a->turn = TURN::OPEN_SECOND;
        return;
    }

    if (controller_a->turn == TURN::OPEN_SECOND && some_card->isFaceDown == true) {
        some_card->isFaceDown = false;
        controller_a->card2 = some_card;

        show_cards(cards_nivel1);
        al_flip_display();

        if (!is_card_got_it_right(controller_a)) {
            controller_a->card1->isFaceDown = true;
            controller_a->card2->isFaceDown = true;

            if (controller_a->player_turn == 1) {

                if (controller_a->total_players == 2)
                    controller_a->player_turn  = 2;

                int score_calculate = controller_a->player1->getScore() - 15;
                controller_a->player1->setScore(score_calculate);
            }
            else {
                if (controller_a->total_players == 2)
                    controller_a->player_turn  = 1;

                int score_calculate = controller_a->player2->getScore() - 15;
                controller_a->player2->setScore(score_calculate);
            }

            al_rest(1.5);
        } else {
            if (controller_a->player_turn == 1) {
                int score_calculate = controller_a->player1->getScore() + 25;
                controller_a->player1->setScore(score_calculate);
            }
            else {
                int score_calculate = controller_a->player2->getScore() + 25;
                controller_a->player2->setScore(score_calculate);
            }
        }

        controller_a->turn = TURN::OPEN_FIRST;
    }
}

void show_all_cards(Cards* cards_nivel) {
    Card *some_card;
    for (unsigned int i = 0; i < cards_nivel->cards.size(); i++) {
        some_card = cards_nivel->cards[i];
        some_card = cards_nivel->cards[i];
        some_card->isFaceDown = false;
    }
}

void hidden_all_cards(Cards* cards_nivel) {
    Card *some_card;
    for (unsigned int i = 0; i < cards_nivel->cards.size(); i++) {
        some_card = cards_nivel->cards[i];
        some_card = cards_nivel->cards[i];
        some_card->isFaceDown = true;
    }
}

void in_game(Cards* cards_nivel1, CONTROLLER_A* controller_a) {
    //al_draw_bitmap_region(background, 0, 0, CARD_DIMENSION_WIDTH, CARD_DIMENSION_HEIGHT, card_position_x, card_position_y, 0);
    al_draw_bitmap(game_background, 0, 0, 0);

    if (controller_a->turn == TURN::PRESENTATION) {
        show_all_cards(cards_nivel1);
        show_cards(cards_nivel1);

        al_flip_display();
        al_rest(3.5);

        hidden_all_cards(cards_nivel1);
        controller_a->turn = TURN::OPEN_FIRST;
    }

    show_cards(cards_nivel1);

    if (ALL_CARDS_ARE_OPENED) {
        al_draw_textf(font, al_map_rgb(255, 255, 255), 320, 550, 0, "%s", "press ENTER to restart");
        controller_a->turn = TURN::ENDGAME;
    }

    if (controller_a->player_turn == 1) {
        al_draw_textf(font, al_map_rgb(255, 255, 255), 37, 40, 0, "%s", "Player 1 turn");
    } else {
        al_draw_textf(font, al_map_rgb(255, 255, 255), 37, 40, 0, "%s", "Player 2 turn");
    }

    al_draw_textf(font, al_map_rgb(255, 255, 255), 255, 520, 0, "%s", "Controls: SPACE and ARROWS, ESC to menu");
    al_draw_textf(font, al_map_rgb(255, 255, 255), 37, 520, 0, "%s %d", "Player 1 score:", controller_a->player1->getScore());

    if (controller_a->total_players == 2) {
        al_draw_textf(font, al_map_rgb(255, 255, 255), 37, 535, 0, "%s %d", "Player 2 score:", controller_a->player2->getScore());
    }
}


int main()
{

    ALLEGRO_DISPLAY* display;

    if(!al_init())
        al_show_native_message_box(NULL,NULL,NULL,"Couldnt initialize",NULL,NULL);

    if(!al_install_keyboard())
        printf("Couldn't initialize keyboard\n");

    /*if(!al_install_mouse())
    {
        printf("couldn't initialize mouse\n");
    }*/
    al_init_primitives_addon();
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
    al_install_audio();
    al_init_acodec_addon();


    //al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    display = al_create_display(800,600);
    if(!display)
        al_show_native_message_box(NULL,NULL,NULL,"Couldnt create Screen",NULL,NULL);

    ALLEGRO_EVENT_QUEUE* queue;

    ALLEGRO_TIMER* timer;
    timer = al_create_timer(1.0 / 60.0);
    queue = al_create_event_queue();
    font = al_create_builtin_font();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    //al_register_event_source(queue, al_get_mouse_event_source());

    ALLEGRO_EVENT event;

    al_start_timer(timer);
    unsigned char key[ALLEGRO_KEY_MAX];

    if(!al_init_image_addon())
        al_show_native_message_box(NULL,NULL,NULL,"Allegro image addon couldnt initialize",NULL,NULL);


    bool abc = false;
    bool exit_game = false;

    al_reserve_samples(5);
    ALLEGRO_SAMPLE *menu_sound;
    ALLEGRO_SAMPLE_ID menu_sound_id;
    menu_sound = al_load_sample("./sounds/menu_sound.ogg");

    ALLEGRO_SAMPLE *ingame_sound;
    ALLEGRO_SAMPLE_ID ingame_sound_id;
    ingame_sound = al_load_sample("./sounds/ingame.ogg");



    ALLEGRO_SAMPLE *sound_click;
    ALLEGRO_SAMPLE_ID sound_click_id;
    sound_click = al_load_sample("./sounds/click.ogg");

    ALLEGRO_SAMPLE *sound_selection;
    ALLEGRO_SAMPLE_ID sound_selection_id;
    sound_selection = al_load_sample("./sounds/selection.ogg");

    ALLEGRO_SAMPLE *sound_start;
    ALLEGRO_SAMPLE_ID sound_start_id;
    sound_start = al_load_sample("./sounds/start.ogg");

    ALLEGRO_BITMAP* title; title= al_load_bitmap("./images/title.png");
    selection= al_load_bitmap("./images/pointer.png");
    background= al_load_bitmap("./images/background.jpg");
    menu_background = al_load_bitmap("./images/menu_background.jpg");
    game_background = al_load_bitmap("./images/game_background.jpg");

    Player player1;
    player1.setName("Player 1");

    Player player2;
    player2.setName("Player 2");

    controller_a.player1 = &player1;
    controller_a.player2 = &player2;

    Cards cards_nivel1;
    cards_nivel1.cards.push_back(new Card("abacaxi", 1));
    cards_nivel1.cards.push_back(new Card("pineapple", 1));
    cards_nivel1.cards.push_back(new Card("amanhecer", 2));
    cards_nivel1.cards.push_back(new Card("dawn", 2));
    cards_nivel1.cards.push_back(new Card("barn", 3));
    cards_nivel1.cards.push_back(new Card("celeiro", 3));
    cards_nivel1.cards.push_back(new Card("boeiro", 4));
    cards_nivel1.cards.push_back(new Card("drain", 4));
    cards_nivel1.cards.push_back(new Card("calcada", 5));
    cards_nivel1.cards.push_back(new Card("sidewalk", 5));
    cards_nivel1.cards.push_back(new Card("caneleira", 6));
    cards_nivel1.cards.push_back(new Card("shin_guard", 6));
    cards_nivel1.cards.push_back(new Card("escova_de_dente", 7));
    cards_nivel1.cards.push_back(new Card("toothbrush", 7));
    cards_nivel1.cards.push_back(new Card("esgoto", 8));
    cards_nivel1.cards.push_back(new Card("sewage", 8));
    cards_nivel1.cards.push_back(new Card("grama", 9));
    cards_nivel1.cards.push_back(new Card("grass", 9));
    cards_nivel1.cards.push_back(new Card("grumpy", 10));
    cards_nivel1.cards.push_back(new Card("ranzinza", 10));
    cards_nivel1.cards.push_back(new Card("hammer", 11));
    cards_nivel1.cards.push_back(new Card("martelo", 11));
    cards_nivel1.cards.push_back(new Card("manga", 12));
    cards_nivel1.cards.push_back(new Card("mango", 12));
    cards_nivel1.cards.push_back(new Card("oil", 13));
    cards_nivel1.cards.push_back(new Card("petroleo", 13));
    cards_nivel1.cards.push_back(new Card("pata", 14));
    cards_nivel1.cards.push_back(new Card("paw", 14));

    cards_nivel1.shuffle_cards();

    memset(key, 0, sizeof(key));

    // al_play_sample(background_sound, 0.3, 0, 1, ALLEGRO_PLAYMODE_LOOP, &background_sound_id);
    //al_stop_sample(id);

    while(!exit_game) {
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER: // Holding key
                abc = is_pressing_the_key(key[ALLEGRO_KEY_S]);
                if (abc) {
                        printf("%d\n", abc);
                }

                for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= KEY_SEEN;

                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;

                if (controller_a.cutscene == CUTSCENE::INGAME) {
                    if(key[ALLEGRO_KEY_SPACE]) {
                        al_play_sample(sound_click, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, &sound_click_id);
                        controller_a_selection_by_space_pressed(&controller_a, &cards_nivel1);
                    }

                    if((key[ALLEGRO_KEY_UP] || key[ALLEGRO_KEY_DOWN] || key[ALLEGRO_KEY_LEFT] || key[ALLEGRO_KEY_RIGHT])) {
                        controller_a_set_line_column_by_key_direction_pressed(key, &controller_a);
                    }

                    if(key[ALLEGRO_KEY_ENTER] && controller_a.turn == TURN::ENDGAME) {
                        controller_a_restart(&controller_a, &cards_nivel1);
                    }
                } else if (controller_a.cutscene == CUTSCENE::MENU) {
                    menu_keydown(key, &controller_a, sound_selection, &sound_selection_id, sound_start, &sound_start_id);

                    if (controller_a.cutscene == CUTSCENE::EXIT) {
                        exit_game = true;
                    }
                } else if (controller_a.cutscene == CUTSCENE::TOTAL_PLAYERS) {
                    menu_player_keydown(key, &controller_a, sound_selection, &sound_selection_id);

                    if(key[ALLEGRO_KEY_ENTER] && controller_a.turn == TURN::ENDGAME) {
                        controller_a_restart(&controller_a, &cards_nivel1);
                    }
                }

                break;
            case ALLEGRO_EVENT_KEY_UP:
                if(key[ALLEGRO_KEY_A]) {
                }
                if(key[ALLEGRO_KEY_ESCAPE]) {
                    if (controller_a.cutscene == CUTSCENE::INGAME) {
                        controller_a.cutscene = CUTSCENE::MENU;
                    }
                }
                key[event.keyboard.keycode] &= KEY_RELEASED;
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                exit_game = true;
                break;
            /*case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                pos_x = event.mouse.x;
                pos_y = event.mouse.y;
                break;*/

        }

        if (al_is_event_queue_empty(queue)) {
            al_clear_to_color(al_map_rgb(0, 0, 0));

            switch(controller_a.cutscene)
            {
                case CUTSCENE::MENU:

                    if (controller_a.ingame_sound_was_started) {
                        al_stop_sample(&ingame_sound_id);
                        controller_a.ingame_sound_was_started = false;
                    }
                    if (!controller_a.menu_sound_was_started) {
                        al_play_sample(menu_sound, 0.6, 0, 1, ALLEGRO_PLAYMODE_LOOP, &menu_sound_id);
                        controller_a.menu_sound_was_started = true;
                    }

                    show_menu(menu_background, font);
                    break;
                case CUTSCENE::TOTAL_PLAYERS:
                    show_menu_player(menu_background, font, &controller_a);
                    break;
                case CUTSCENE::INGAME:
                    if (controller_a.menu_sound_was_started) {
                        al_stop_sample(&menu_sound_id);
                        controller_a.menu_sound_was_started = false;
                    }

                    if (!controller_a.ingame_sound_was_started) {
                        al_play_sample(ingame_sound, 0.6, 0, 1, ALLEGRO_PLAYMODE_LOOP, &ingame_sound_id);
                        controller_a.ingame_sound_was_started = true;
                    }

                    in_game(&cards_nivel1, &controller_a);
                    break;
            }
        }

        al_flip_display();
    } // game loop

    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_sample(menu_sound);
    al_destroy_sample(sound_click);
    al_destroy_sample(sound_selection);
    al_destroy_sample(sound_start);
    al_destroy_sample(ingame_sound);
    al_destroy_bitmap(menu_background);

    return 0;
}

/**

speech to text free
https://ttsmp3.com/

1 - retrovisor - side mirror
martelo - hammer
caneleira - shin guard
calçada - sidewalk
escova de dente - toothbrush
pata - paw
esgoto - sewage
boeiro -drain
Abacaxi - Pineapple
petroleo - oil
celeiro - barn
Amanhecer - Dawn - 11
Ranzinza - Grumpy
manga - mango
grama - grass


tijolo - brick
colher - spoon
garfo - fork
caderno - notebook paper
geladeira - fridge
cana de açucar - sugarcane
chuteira - soccer cleats
manga - t-shirt sleeve
pano de mesa - tablecloth
caixa de areia de gato -litter box
dever de casa - homework
Cavalo-marinho - Seahorse
Piquenique - Picnic
Eclético - Eclectic
Deslumbrante - Stunning
Ferroviário - Railway worker
Cavaleiro - Knight
Sussurro - Whisper
Esplêndido - Splendid
Labirinto - Labyrinth
Alma - Soul
Mendigo - Beggar
Penumbra - Twilight
Peregrino - Pilgrim
Vagalume - Firefly
Inóspito - Inhospitable
Nefasto - Nefarious
Pálido - Pale
Voraz - Voracious


PARTS OF THE CAR
Volante - Steering wheel
Freio - Brake
Motor - Engine
Capô - Hood
Porta-malas - Trunk
Rodas - Wheels
Faróis - Headlights
Pára-choques - Bumper
Para-brisa - Windshield
Retrovisor - Rearview mirror
Banco - Seat
Câmbio - Gear shift
Pneu - Tire
Suspensão - Suspension


sound effects free: https://www.chosic.com/free-music/piano/, https://pixabay.com/sound-effects/search/background%20piano/?pagi=2
acesso arquivos, virutal diretorios (exemplo: doom.wad): https://icculus.org/physfs/docs/html/

Cão - Dog
Gato - Cat
Casa - House
Árvore - Tree
Bola - Ball
Livro - Book
Carro - Car
Avião - Airplane
Escola - School
Sol - Sun <---
Lua - Moon
Pássaro - Bird
Floresta - Forest
Praia - Beach
Cidade - City
Montanha - Mountain
Rio - River
Lago - Lake
Oceano - Ocean
Estrela - Star
Fruta - Fruit
Verdura - Vegetable
Leite - Milk
Água - Water
Suco - Juice
Pão - Bread
Queijo - Cheese
Ovo - Egg
Arroz - Rice
Feijão - Beans - 30
Carne - Meat
Peixe - Fish
Macaco - Monkey
Elefante - Elephant
Leão - Lion
Girafa - Giraffe
Zebra - Zebra
Tigre - Tiger
Urso - Bear
Cavalo - Horse - 40
Galinha - Chicken
Porco - Pig
Cabra - Goat
Sapo - Frog
Cobra - Snake
Tartaruga - Turtle
Camisa - Shirt
Calça - Pants
Sapato - Shoe
Chapéu - Hat - 50

**/

#ifndef SDL_H
#define SDL_H

typedef enum menu_state {MAIN_MENU, PLAY, ADD_IMAGE, ADD_LANG, QUIT} MenuState;
typedef enum play_mode {CLASSIC, RANDOM, TRAINING} Mode;
    
/*==== Function prototype ====*/
/**
 * Setup window and renderer. Raise the window.
 * \return 1 on success, 0 if error.
*/
int setup_window_renderer(SDL_Window **window, SDL_Renderer **renderer, const char *title);
//Set the background color of the window.
void set_back_color(SDL_Renderer *renderer, SDL_Color color);
//Set background image
void set_back_image(SDL_Renderer *renderer, const char *image_path);
//Popup window, you can print whatever you want on it
void window_popup(SDL_Renderer *renderer, SDL_Window *window, char *text, SDL_Color font_color,  SDL_Color back_color, int delay_ms);
//Win or lose marker for play_game_window
void win_lose_popup(SDL_Renderer *renderer, SDL_Window *window, int win, int delay_ms);
//Print score of player 1 and player 2 on the window
void score_popup(SDL_Renderer *renderer, SDL_Window *window, int p1_score, int p2_score, int delay_ms);
//Admin password window in the MAIN MENU
int password_window(SDL_Renderer *renderer, SDL_Window *window, MenuState *menu);
//Main menu window
int main_menu_window(SDL_Renderer *renderer, SDL_Window *window, MenuState *menu);
//Menu selection (Play)
void play_menu_window(SDL_Renderer *renderer, SDL_Window *window, int *one_player_bool, Mode *play_menu_choice, int *nb_image, int nb_image_max, MenuState *menu);
//Language selection for certain mode
char *select_language_window(SDL_Renderer *renderer, SDL_Window *window, char **language_array, int array_size, MenuState *menu);
//Image selection (2 players mode)
AVL_Image *select_image_window(SDL_Renderer *renderer, SDL_Window *window, AVL_Image *image_avl, char **french_array, int french_array_size, MenuState *menu);
//Window player turn
void player_turn_window(SDL_Renderer *renderer, SDL_Window *window, int player_turn, int p1_score, int p2_score);
//Basic game outpup
void game_window(SDL_Renderer *renderer, SDL_Window *window, AVL_Image *image, const char *language);
//Images debug
void test_image(AVL_Image *image_avl, SDL_Renderer *rend, SDL_Window *win);
//Étape 3
void random_image(AVL_Image *image_avl, SDL_Renderer *rend, SDL_Window *win, char **const struct1, int struct1_size);
//Game window
int play_game_window(AVL_Image *image, AVL_Trans *trans, SDL_Renderer *renderer, SDL_Window *window, MenuState *menu);
//End game window with score, winner...
void end_game_window(SDL_Renderer *renderer, SDL_Window *window, int one_player_bool, int p1_score, int p2_score, int score_max, MenuState *menu);
//Main game function
void play_game(SDL_Renderer *renderer, SDL_Window *window, AVL_Image *image, char **french_array, int french_array_size, char **language_array, int lang_array_size, int one_player, int nb_images, Mode mode, MenuState *menu);
//Training mode
void training_mode(SDL_Renderer *renderer, SDL_Window *window, AVL_Image *image_avl, char **french_array, int french_array_size, char *language, MenuState *menu);
/*==== Add Image ====*/
void add_trans_window(SDL_Renderer *renderer, SDL_Window *window, char *french_word, char *language, char *input, int input_size);
void add_image_menu(SDL_Renderer *renderer, SDL_Window *window, AVL_Image **image_avl, char ***french_array, int *french_array_size, char **language_array, int lang_array_size, MenuState *menu);

/*==== Add language ====*/
void add_language_menu(SDL_Renderer *renderer, SDL_Window *window, AVL_Image *image_avl, char **french_array, int french_array_size, char ***language_array, int *lang_array_size, MenuState *menu);
#endif
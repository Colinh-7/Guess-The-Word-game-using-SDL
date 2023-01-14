#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "data_struct.h"
#include "strmanip.h"
#include "sdl.h"

int main(int argc, char *argv[])
{
    /*==== Variables ====*/
    int status, nb_words, nb_language, debug = 0;
    char **french = NULL, **language = NULL;
    AVL_Image *image = NULL;
    MenuState state = MAIN_MENU;

    int one_player, back_play_menu, play_nb_image = 1;
    Mode play_menu_choice;

    SDL_Window *win = NULL;
    SDL_Renderer *render = NULL;
    SDL_bool play = SDL_TRUE;
    SDL_Event event;
    Mix_Music *main_theme = NULL;
    Mix_Chunk *enter = NULL;

    /*==== Program initialization ====*/
    //Check if debug mode is activated.
    if (argc > 1 && (!strcmp(argv[1], "-debug"))) debug = 1;

    //Define how many words the array of french words and language will contain
    FILE *temp = fopen("data/loading-files/data.csv", "r");
    if (temp) {
        nb_words = nb_linesCSV(temp)-1;
        nb_language = nb_columnsCSV(temp) -1;
    }
    fclose(temp);

    //Load data
    load_from_CSV(&french, &language, &image);

    /*==== Start SDL and TTF ====*/
    if (!SDL_Init(SDL_INIT_VIDEO) && !TTF_Init() && (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) != -1)) {
        if (debug) printf("SDL_Init : successful.\nTTF_Init : successful.\n");

        if (image && debug) printf("Image AVL : successful.\n");
        else if (debug) printf("Image AVL : error\n");
        if (french && debug) printf("French array : successful.\n");
        else if (debug) printf("French array : error\n");
        if (language && debug) printf("Language array : successful.\n");
        else if (debug) printf("Language array : error\n");

        //Renderer and window initialization
        if (setup_window_renderer(&win, &render, "Devine l'image") && image && french && language) {
            if (debug) printf("Window and renderer : successful.\n");

            //Étape 3
            if (argc > 1 && (!strcmp(argv[1], "-rand"))) random_image(image, render, win, french, nb_words);

            //Load music and sound
            Mix_AllocateChannels(10);
            main_theme = Mix_LoadMUS("data/music/main_theme.mp3");
            enter = Mix_LoadWAV("data/music/enter.wav");

            //Start music
            Mix_PlayMusic(main_theme, -1);

            //Main loop
            while (play) {
                switch (state) {
                    case MAIN_MENU:
                        main_menu_window(render, win, &state);
                        //Play the sound if enter is pressed and valid
                        Mix_PlayChannel(-1, enter, 0);
                    break;
                    case PLAY:
                        play_menu_window(render, win, &one_player, &play_menu_choice, &play_nb_image, nb_words, &state);
                        Mix_PlayChannel(-1, enter, 0);
                        if (state != MAIN_MENU && state != QUIT) {
                            //Stop main theme
                            Mix_HaltMusic();
                            //Launch the game
                            play_game(render, win, image, french, nb_words, language, nb_language, one_player, play_nb_image, play_menu_choice, &state);
                            //Restart music
                            Mix_PlayChannel(-1, enter, 0);
                            Mix_PlayMusic(main_theme, -1);
                            //Return to MAIN_MENU if the player did not quit.
                            if (state != QUIT) state = MAIN_MENU;
                        }
                    break;
                    case ADD_IMAGE:
                        add_image_menu(render, win, &image, &french, &nb_words, language, nb_language, &state);
                        if (state != QUIT) state = MAIN_MENU;
                    break;
                    case ADD_LANG:
                        add_language_menu(render, win, image, french, nb_words, &language, &nb_language, &state);
                        if (state != QUIT) state = MAIN_MENU;
                    break;
                    case QUIT:
                        play = SDL_FALSE;
                        status = EXIT_SUCCESS;
                    break;
                }
            }
            SDL_RenderClear(render);
            SDL_DestroyRenderer(render);
            SDL_DestroyWindow(win);
            Mix_FreeMusic(main_theme);
            Mix_FreeChunk(enter);
            TTF_Quit();
            SDL_Quit();
            Mix_CloseAudio();
        }
        else {
            status = EXIT_FAILURE;
        }
    }
    else {
        if (debug) printf("Error SDL_Init : %s\n", SDL_GetError());
        if (debug) printf("SDL_mixer error : %s\n", Mix_GetError());
        status = EXIT_FAILURE;
    }
    /*==== Free ====*/
    free_array(french, nb_words);
    free_array(language, nb_language);
    free_AVL(image);
    if (debug) printf("All structures have been freed.\n");

    /*==== End ====*/
    return status;
}

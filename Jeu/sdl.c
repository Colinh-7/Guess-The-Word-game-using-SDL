#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <time.h>
#include "data_struct.h"
#include "strmanip.h"
#include "sdl.h"

SDL_Color 
    black = {0,0,0,255},
    white = {255, 255, 255, 255},
    red = {255, 0, 0, 255},
    lime = {0, 255, 0, 255},
    green = {0, 128, 0, 255},
    blue = {0, 0, 255, 255},
    cyan = {0, 255, 255, 255},
    yellow = {255, 255, 0, 255},
    grey = {128, 128, 128, 255},
    silver = {192, 192, 192, 255}
;

int setup_window_renderer(SDL_Window **window, SDL_Renderer **renderer, const char *title)
{
    SDL_CreateWindowAndRenderer(1920, 1080, SDL_WINDOW_RESIZABLE, window, renderer);
    if (*window && *renderer) {
        SDL_SetWindowTitle(*window, title);
        SDL_RaiseWindow(*window);
        return 1;
    }
    else return 0;
}

void set_back_color(SDL_Renderer *renderer, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

void set_back_image(SDL_Renderer *renderer, const char *image_path)
{
    SDL_Texture *text = NULL;
    text = IMG_LoadTexture(renderer, image_path);
    if (text) SDL_RenderCopy(renderer, text, NULL, NULL);
    SDL_DestroyTexture(text);
}

void window_popup(SDL_Renderer *renderer, SDL_Window *window, char *text, SDL_Color font_color,  SDL_Color back_color, int delay_ms) 
{
    TTF_Font *font = NULL;
    SDL_Rect popup_rect, text_rect;
    SDL_Surface *text_surf = NULL;
    SDL_Texture *text_text = NULL;
    int w, h;
    char *myfont = "data/fonts/retro.ttf";

    //Get window height and width
    SDL_GetWindowSize(window, &w, &h);
    //Setup rect
    text_rect.w = strlen(text)*(w/64);
    text_rect.h = h/9;
    text_rect.x = w/2 - text_rect.w/2;
    text_rect.y = h/2 - text_rect.h/2;

    if (text_rect.w >= w/2) popup_rect.w = text_rect.w + 60;
    else popup_rect.w = w/2;
    popup_rect.h = h/3;
    popup_rect.x = w/2 - popup_rect.w/2;
    popup_rect.y = h/2 - popup_rect.h/2;

    //Font
    font = TTF_OpenFont(myfont, 50);
    
    if (font) {
        text_surf = TTF_RenderText_Solid(font, text, font_color);
        text_text = SDL_CreateTextureFromSurface(renderer, text_surf);
        SDL_SetRenderDrawColor(renderer, back_color.r, back_color.g, back_color.b, back_color.a);
        SDL_RenderFillRect(renderer, &popup_rect);
        SDL_RenderCopy(renderer, text_text, NULL, &text_rect);
        SDL_RenderPresent(renderer);
        SDL_Delay(delay_ms);
        //Reset back color
        SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
    }
    TTF_CloseFont(font);
    SDL_FreeSurface(text_surf);
    SDL_DestroyTexture(text_text);
}

void win_lose_popup(SDL_Renderer *renderer, SDL_Window *window, int win, int delay_ms)
{
    SDL_Rect image_rect;
    SDL_Texture *image = NULL;
    int w, h;

    SDL_GetWindowSize(window, &w, &h);

    image_rect.w = w/9;
    image_rect.h = h/7;
    image_rect.x = w*0.75;
    image_rect.y = h*0.75;

    if (win) image = IMG_LoadTexture(renderer, "data/utils/win.png"); 
    else image = IMG_LoadTexture(renderer, "data/utils/lose.png");
    if (image) {
        SDL_RenderCopy(renderer, image, NULL, &image_rect);
        SDL_RenderPresent(renderer);
        SDL_Delay(delay_ms);
    }
    SDL_DestroyTexture(image);
}

void score_popup(SDL_Renderer *renderer, SDL_Window *window, int p1_score, int p2_score, int delay_ms)
{
    SDL_Rect p1_title_rect, p2_title_rect, p1_score_rect, p2_score_rect;
    SDL_Surface *p1_title_surf = NULL, *p2_title_surf = NULL, *p1_score_surf = NULL, *p2_score_surf = NULL;
    SDL_Texture *p1_title_text = NULL, *p2_title_text = NULL, *p1_score_text = NULL, *p2_score_text = NULL;
    TTF_Font *title_font = NULL, *font = NULL;
    char
        *p1_score_title = "Score J1 :",
        *p2_score_title = "Score J2 :",
        p1_score_char[6],
        p2_score_char[6],
        *myfont = "data/fonts/retro.ttf"
    ;
    int w, h;

    SDL_GetWindowSize(window, &w, &h);
    //Score_char init
    sprintf(p1_score_char, "%d", p1_score);
    sprintf(p2_score_char, "%d", p2_score);
    //Rect init
    p1_title_rect.w = 10*(w/64); //10 -> nb char
    p1_title_rect.h = h/10;
    p1_title_rect.x = w*0.05;
    p1_title_rect.y = h*0.75;

    p2_title_rect.w = 10*(w/64);
    p2_title_rect.h = h/10;
    p2_title_rect.x = w*0.95 - p2_title_rect.w;
    p2_title_rect.y = h*0.75;

    p1_score_rect.w = strlen(p1_score_char)*(w/64);
    p1_score_rect.h = h/10;
    p1_score_rect.x = (p1_title_rect.x + p1_title_rect.w/2) - p1_score_rect.w/2;
    p1_score_rect.y = h*0.85;

    p2_score_rect.w = strlen(p2_score_char)*(w/64);
    p2_score_rect.h = h/10;
    p2_score_rect.x = (p2_title_rect.x + p2_title_rect.w/2) - p2_score_rect.w/2;
    p2_score_rect.y = h*0.85;

    //Open font
    title_font = TTF_OpenFont(myfont, 50);
    TTF_SetFontStyle(title_font, TTF_STYLE_UNDERLINE);
    font = TTF_OpenFont(myfont, 50);

    if (title_font && font) {
        p1_title_surf = TTF_RenderText_Solid(title_font, p1_score_title, white);
        p2_title_surf = TTF_RenderText_Solid(title_font, p2_score_title, white);
        p1_score_surf = TTF_RenderText_Solid(font, p1_score_char, white);
        p2_score_surf = TTF_RenderText_Solid(font, p2_score_char, white);

        p1_title_text = SDL_CreateTextureFromSurface(renderer, p1_title_surf);
        p2_title_text = SDL_CreateTextureFromSurface(renderer, p2_title_surf);
        p1_score_text = SDL_CreateTextureFromSurface(renderer, p1_score_surf);
        p2_score_text = SDL_CreateTextureFromSurface(renderer, p2_score_surf);

        SDL_RenderCopy(renderer, p1_title_text, NULL, &p1_title_rect);
        SDL_RenderCopy(renderer, p2_title_text, NULL, &p2_title_rect);
        SDL_RenderCopy(renderer, p1_score_text, NULL, &p1_score_rect);
        SDL_RenderCopy(renderer, p2_score_text, NULL, &p2_score_rect);
        SDL_RenderPresent(renderer);
        SDL_Delay(delay_ms);
    }

    SDL_FreeSurface(p1_title_surf);
    SDL_FreeSurface(p2_title_surf);
    SDL_FreeSurface(p1_score_surf);
    SDL_FreeSurface(p2_score_surf);
    SDL_DestroyTexture(p1_title_text);
    SDL_DestroyTexture(p2_title_text);
    SDL_DestroyTexture(p1_score_text);
    SDL_DestroyTexture(p2_score_text);
    TTF_CloseFont(title_font);
    TTF_CloseFont(font);
}

int password_window(SDL_Renderer *renderer, SDL_Window *window, MenuState *menu) 
{
    TTF_Font *font = NULL, *title_font = NULL, *hidden_font = NULL;
    SDL_Rect title_rect, input_rect, hidden_rect;
    SDL_Surface *title_surf = NULL, *input_surf = NULL;
    SDL_Texture *title_text = NULL, *input_text = NULL, *image_text = NULL, *image2_text = NULL, *temp_text = NULL;
    SDL_bool play = SDL_TRUE;
    SDL_Event evt;
    char
        *password = "LKIM:9::", 
        *title = "Entrez le mot de passe administrateur :",
        *no_input = "Entrez le mot de passe...",
        input[20] = "",
        hidden_input[20] = "",
        *myfont = "data/fonts/retro.ttf"
    ;
    int w, h, i = 0, return_value = 0, hidden_mode=1;

    SDL_GetWindowSize(window, &w, &h);

    title_rect.w = w*0.8;
    title_rect.h = h/7;
    title_rect.x = w/2-title_rect.w/2;
    title_rect.y = h*0.1;

    input_rect.w = w/3;
    input_rect.h = h/9;
    input_rect.x = w/2-input_rect.w/2;
    input_rect.y = h*0.4;

    hidden_rect.w = w*0.05;
    hidden_rect.h = h*0.05;
    hidden_rect.x = w*0.8 - hidden_rect.w/2;
    hidden_rect.y = input_rect.y;

    title_font = TTF_OpenFont(myfont, 50);
    TTF_SetFontStyle(title_font, TTF_STYLE_UNDERLINE);
    font = TTF_OpenFont(myfont, 50);
    hidden_font = TTF_OpenFont("data/fonts/FreeSansBold.ttf", 50);

    if (font && title_font) {
        SDL_RenderClear(renderer);
        //Textures and surfaces
        title_surf = TTF_RenderText_Solid(title_font, title, white);
        input_surf = TTF_RenderText_Solid(font, no_input, white);
        title_text = SDL_CreateTextureFromSurface(renderer, title_surf);
        input_text = SDL_CreateTextureFromSurface(renderer, input_surf);

        //Open both images
        image_text = IMG_LoadTexture(renderer, "data/utils/openeye.png");
        image2_text = IMG_LoadTexture(renderer, "data/utils/closeeye.png");

        SDL_RenderCopy(renderer, title_text, NULL, &title_rect);
        SDL_RenderCopy(renderer, input_text, NULL, &input_rect);
        SDL_RenderCopy(renderer, image_text, NULL, &hidden_rect);

        SDL_RenderPresent(renderer);

        while(play) {
            SDL_WaitEvent(&evt);
            switch(evt.type) {
                case SDL_QUIT:
                    play = SDL_FALSE;
                    *menu = QUIT;
                break;
                case SDL_WINDOWEVENT:
                    if (evt.window.event = SDL_WINDOWEVENT_RESIZED) {
                        SDL_GetWindowSize(window, &w, &h);
                        title_rect.w = w*0.8;
                        title_rect.h = h/7;
                        title_rect.x = w/2-title_rect.w/2;
                        title_rect.y = h*0.1;
                        
                        input_rect.h = h/9;
                        input_rect.y = h*0.4;

                        hidden_rect.w = w*0.05;
                        hidden_rect.h = h*0.05;
                        hidden_rect.x = w*0.8 - hidden_rect.w/2;
                        hidden_rect.y = input_rect.y;
                    }
                break;
                case SDL_KEYUP:
                    if (evt.key.keysym.sym == SDLK_BACKSPACE) {
                        if (i) {
                            input[i-1] = '\0';
                            hidden_input[--i] = '\0';
                        }
                    }
                    if (evt.key.keysym.sym == SDLK_RETURN) {
                        if (i) {
                            return_value = (strcmp(password, hash_password(input)) == 0) ? 1 : 0;
                            play = SDL_FALSE;
                        }
                        else {
                            window_popup(renderer, window, "Vous devez entrer un mot de passe.", black, silver, 1200);
                        }
                    }
                break;
                case SDL_MOUSEBUTTONUP:
                    //If click on hidden_rect
                    if ((evt.button.x >= hidden_rect.x && evt.button.x <= (hidden_rect.x+hidden_rect.w)) && (evt.button.y >= hidden_rect.y && evt.button.y <= (hidden_rect.y+hidden_rect.h))) {
                        hidden_mode = !hidden_mode;
                    }
                break;
                case SDL_TEXTINPUT:
                if (i<sizeof(input)-2) {
                    strcat(input, evt.text.text);
                    hidden_input[i++] = '*';
                }
                break;
            }
            if (evt.type == SDL_KEYUP || evt.type == SDL_MOUSEBUTTONUP || evt.window.event == SDL_WINDOWEVENT_RESIZED) {
                SDL_RenderClear(renderer);
                if (strlen(input)) {
                    if (!hidden_mode) {
                        input_surf = TTF_RenderText_Solid(font, input, white);
                        input_rect.w = strlen(input)*(w/48);
                    }
                    else {
                        input_surf = TTF_RenderText_Solid(hidden_font, hidden_input, white);
                        input_rect.w = strlen(input)*(w/48);
                    }
                }
                else {
                    input_surf = TTF_RenderText_Solid(font, no_input, white);
                    input_rect.w = w/3;
                }
                temp_text = (hidden_mode) ? image_text : image2_text;

                input_text = SDL_CreateTextureFromSurface(renderer, input_surf);
                input_rect.x = w/2-input_rect.w/2;

                SDL_RenderCopy(renderer, title_text, NULL, &title_rect);
                SDL_RenderCopy(renderer, input_text, NULL, &input_rect);
                SDL_RenderCopy(renderer, temp_text, NULL, &hidden_rect);

                SDL_RenderPresent(renderer);
            }
        }

    }
    SDL_RenderClear(renderer);

    TTF_CloseFont(hidden_font);
    TTF_CloseFont(font);
    TTF_CloseFont(title_font);
    SDL_FreeSurface(title_surf);
    SDL_FreeSurface(input_surf);
    SDL_DestroyTexture(image_text);
    SDL_DestroyTexture(image2_text);
    SDL_DestroyTexture(title_text);
    SDL_DestroyTexture(input_text);

    return return_value;
}

int main_menu_window(SDL_Renderer *renderer, SDL_Window *window, MenuState *menu)
{
    TTF_Font *pfont = NULL, *ifont = NULL, *lfont = NULL, *title_font = NULL;
    SDL_Texture *ttitle = NULL, *tplay = NULL, *timage = NULL, *tlang = NULL, *admin_text = NULL;
    SDL_Surface *stitle = NULL, *splay = NULL, *sadd_image = NULL, *sadd_lang = NULL, *admin_surf = NULL;
    SDL_Rect rtitle, rplay, rimage, rlang, admin_rect;
    SDL_Color tcolor = white, pcolor = lime, icolor = white, lcolor = white;
    SDL_bool play = SDL_TRUE;
    SDL_Event evt;
    int w, h, ctrl = 0;
    static int admin_mode = 0;
    char *admin = "Mode administrateur";

    //Get window width and height
    SDL_GetWindowSize(window, &w, &h);

    //Init all rect
    rtitle.w = w*0.6;
    rtitle.h = h/7;
    rtitle.x = w/2 - rtitle.w/2;
    rtitle.y = 10;

    rplay.w = w/9;
    rplay.h = h/9;
    rplay.x = w/2-rplay.w/2;
    rplay.y = h*0.25;

    rimage.w = w/5;
    rimage.h = h/9;
    rimage.x = w/2-rimage.w/2;
    rimage.y = h*0.375;

    rlang.w = w/5;
    rlang.h = h/9;
    rlang.x = w/2-rlang.w/2;
    rlang.y = h*0.5;

    admin_rect.w = w/7;
    admin_rect.h = h/14;
    admin_rect.x = w*0.025;
    admin_rect.y = h*0.9;

    //Init all fonts
    pfont = TTF_OpenFont("data/fonts/retro.ttf", 50);
    ifont = TTF_OpenFont("data/fonts/retro.ttf", 50);
    lfont = TTF_OpenFont("data/fonts/retro.ttf", 50);
    title_font = TTF_OpenFont("data/fonts/FreeSansBold.ttf", 50);
    TTF_SetFontStyle(title_font, TTF_STYLE_UNDERLINE);
    TTF_SetFontStyle(pfont, TTF_STYLE_UNDERLINE);

    //Default value
    *menu = PLAY;

    //Switching colors depending menu state
    if (pfont && ifont && lfont && title_font) {

        //Init all surfaces
        stitle = TTF_RenderText_Solid(title_font, "DEVINE L'IMAGE ?", tcolor);
        splay = TTF_RenderText_Solid(pfont, "Jouer", pcolor);
        sadd_image = TTF_RenderText_Solid(ifont, "Ajouter image", icolor);
        sadd_lang = TTF_RenderText_Solid(lfont, "Ajouter langue", lcolor);

        if (admin_mode) {
            admin_surf = TTF_RenderText_Solid(title_font, admin, white);
            admin_text = SDL_CreateTextureFromSurface(renderer, admin_surf);
            SDL_RenderCopy(renderer, admin_text, NULL, &admin_rect);
        }

        ttitle = SDL_CreateTextureFromSurface(renderer, stitle);
        tplay = SDL_CreateTextureFromSurface(renderer, splay);
        timage = SDL_CreateTextureFromSurface(renderer, sadd_image);
        tlang = SDL_CreateTextureFromSurface(renderer, sadd_lang);

        SDL_RenderCopy(renderer, ttitle, NULL, &rtitle);
        SDL_RenderCopy(renderer, tplay, NULL, &rplay);
        SDL_RenderCopy(renderer, timage, NULL, &rimage);
        SDL_RenderCopy(renderer, tlang, NULL, &rlang);

        SDL_RenderPresent(renderer);

        while (play) {
            SDL_WaitEvent(&evt);
            switch(evt.type) {
                case SDL_QUIT:
                    *menu = QUIT;
                    play = SDL_FALSE;
                break;
                case SDL_WINDOWEVENT:
                    if (evt.window.event == SDL_WINDOWEVENT_RESIZED) {
                        SDL_GetWindowSize(window, &w, &h);
                        rtitle.w = w*0.6;
                        rtitle.h = h/7;
                        rtitle.x = w/2 - rtitle.w/2;
                        rtitle.y = 10;

                        rplay.w = w/9;
                        rplay.h = h/9;
                        rplay.x = w/2-rplay.w/2;
                        rplay.y = h*0.25;

                        rimage.w = w/5;
                        rimage.h = h/9;
                        rimage.x = w/2-rimage.w/2;
                        rimage.y = h*0.375;

                        rlang.w = w/5;
                        rlang.h = h/9;
                        rlang.x = w/2-rlang.w/2;
                        rlang.y = h*0.5;

                        admin_rect.w = w/7;
                        admin_rect.h = h/14;
                        admin_rect.x = w*0.025;
                        admin_rect.y = h*0.9;
                    }
                break;
                case SDL_KEYUP:
                    if (evt.key.keysym.sym == SDLK_DOWN) *menu = (++(*menu) == QUIT) ? PLAY : *menu; //If menu == QUIT -> set it back to play
                    if (evt.key.keysym.sym == SDLK_UP) *menu = (--(*menu) == MAIN_MENU) ? ADD_LANG: *menu; //if menu == MAIN_MENU ->set it back to add_lang
                    if (evt.key.keysym.sym == SDLK_RETURN) {
                        if (admin_mode) {
                            play = SDL_FALSE;
                        }
                        else {
                            if (*menu == ADD_LANG || *menu == ADD_IMAGE) {
                                window_popup(renderer, window, "Droits administrateurs requis.", black, silver, 1800);
                            }
                            else {
                                play = SDL_FALSE;
                            }
                        }
                    }
                    if (evt.key.keysym.sym == SDLK_LCTRL || evt.key.keysym.sym == SDLK_RCTRL) ctrl = 0;
                break;
                case SDL_KEYDOWN:
                    if (evt.key.keysym.sym == SDLK_LCTRL || evt.key.keysym.sym == SDLK_RCTRL) ctrl = 1;
                    //If CTRL+a
                    if (ctrl && evt.key.keysym.sym == SDLK_a) {
                        if (!admin_mode) {
                            admin_mode = password_window(renderer, window, menu);
                            if (*menu == QUIT) play = SDL_FALSE;   
                        }
                        else {
                            window_popup(renderer, window, "Droits administrateurs deja actifs.", black, silver, 1200);
                        }
                    }
                break;
            }
            if (evt.type == SDL_WINDOWEVENT || evt.type == SDL_KEYUP || evt.type == SDL_KEYDOWN) {
                SDL_RenderClear(renderer);
                switch(*menu) {
                    case PLAY:
                        pcolor = lime;
                        icolor = white;
                        lcolor = white;
                        TTF_SetFontStyle(pfont, TTF_STYLE_UNDERLINE);
                        TTF_SetFontStyle(ifont, TTF_STYLE_NORMAL);
                        TTF_SetFontStyle(lfont, TTF_STYLE_NORMAL);
                    break;
                    case ADD_IMAGE:
                        pcolor = white;
                        icolor = lime;
                        lcolor = white;
                        TTF_SetFontStyle(pfont, TTF_STYLE_NORMAL);
                        TTF_SetFontStyle(ifont, TTF_STYLE_UNDERLINE);
                        TTF_SetFontStyle(lfont, TTF_STYLE_NORMAL);
                    break;
                    case ADD_LANG:
                        pcolor = white;
                        icolor = white;
                        lcolor = lime;
                        TTF_SetFontStyle(pfont, TTF_STYLE_NORMAL);
                        TTF_SetFontStyle(ifont, TTF_STYLE_NORMAL);
                        TTF_SetFontStyle(lfont, TTF_STYLE_UNDERLINE);
                    break;
                }
                splay = TTF_RenderText_Solid(pfont, "Jouer", pcolor);
                sadd_image = TTF_RenderText_Solid(ifont, "Ajouter image", icolor);
                sadd_lang = TTF_RenderText_Solid(lfont, "Ajouter langue", lcolor);

                ttitle = SDL_CreateTextureFromSurface(renderer, stitle);
                tplay = SDL_CreateTextureFromSurface(renderer, splay);
                timage = SDL_CreateTextureFromSurface(renderer, sadd_image);
                tlang = SDL_CreateTextureFromSurface(renderer, sadd_lang);

                if (admin_mode) {
                    admin_surf = TTF_RenderText_Solid(title_font, admin, white);
                    admin_text = SDL_CreateTextureFromSurface(renderer, admin_surf);
                    SDL_RenderCopy(renderer, admin_text, NULL, &admin_rect);
                }

                SDL_RenderCopy(renderer, ttitle, NULL, &rtitle);
                SDL_RenderCopy(renderer, tplay, NULL, &rplay);
                SDL_RenderCopy(renderer, timage, NULL, &rimage);
                SDL_RenderCopy(renderer, tlang, NULL, &rlang);

                SDL_RenderPresent(renderer);
            } 
        }
    }
    SDL_RenderClear(renderer);

    SDL_DestroyTexture(ttitle);
    SDL_FreeSurface(stitle);
    SDL_DestroyTexture(tplay);
    SDL_FreeSurface(splay);    
    SDL_DestroyTexture(timage);
    SDL_FreeSurface(sadd_image);
    SDL_DestroyTexture(tlang);
    SDL_FreeSurface(sadd_lang);
    SDL_FreeSurface(admin_surf);
    SDL_DestroyTexture(admin_text);
    TTF_CloseFont(pfont);
    TTF_CloseFont(ifont);
    TTF_CloseFont(lfont);
    TTF_CloseFont(title_font);
}

void play_menu_window(SDL_Renderer *renderer, SDL_Window *window, int *one_player_bool, Mode *play_menu_choice, int *nb_image, int nb_image_max, MenuState *menu)
{
    TTF_Font *title_font = NULL, *one_font = NULL, *two_font = NULL, *classic_font = NULL, *rand_font = NULL, *training_font = NULL, *nb_input_font = NULL;
    SDL_Surface *title_surf = NULL, *one_player_surf = NULL, *two_player_surf = NULL, *classic_surf = NULL, *random_surf = NULL, *training_surf = NULL, *nb_image_surf = NULL;
    SDL_Texture *title_text = NULL, *one_player_text = NULL, *two_player_text = NULL, *classic_text = NULL, *random_text = NULL, *training_text = NULL, *nb_image_text = NULL;
    SDL_Rect title_rect, one_rect, two_rect, classic_rect, rand_rect, train_rect, nb_image_rect;
    SDL_Color one_color, two_color, classic_color, rand_color, training_color, training_color_invisible;
    SDL_Event evt;
    SDL_bool play = SDL_TRUE;
    char 
        *title = "Choix de la partie",
        *one_player = "1 joueur",
        *two_player = "2 joueurs",
        *classic = "Mode classique",
        *random = "Mode aleatoire",
        *training = "Mode entrainement",
        *myfont = "data/fonts/retro.ttf",
        nb_image_output[25] = "Nombre d'image(s) : ",
        nb_image_input[6] = ""
    ;
    int w, h, i;
    
    //Get window size
    SDL_GetWindowSize(window, &w, &h);
    //Rect init
    title_rect.w = w/3;
    title_rect.h = h/7;
    title_rect.x = w/2 - title_rect.w/2;
    title_rect.y = 10;

    one_rect.w = w/7;
    one_rect.h = h/10;
    one_rect.x = w/3 - one_rect.w/2;
    one_rect.y = h/5;

    two_rect.w = one_rect.w;
    two_rect.h = one_rect.h;
    two_rect.x = w * 0.66 - two_rect.w/2;
    two_rect.y = one_rect.y;

    classic_rect.w = w/5;
    classic_rect.h = h/10;
    classic_rect.x = w/2 - classic_rect.w/2;
    classic_rect.y = h*0.38;

    rand_rect.w = w/5;
    rand_rect.h = h/10;
    rand_rect.x = w/2 - rand_rect.w/2;
    rand_rect.y = h/2;

    train_rect.w = w/4;
    train_rect.h = h/10;
    train_rect.x = w/2 - train_rect.w/2;
    train_rect.y = h*0.62;

    nb_image_rect.w = w/3;
    nb_image_rect.h = h/10;
    nb_image_rect.x = w/2 - nb_image_rect.w/2;
    nb_image_rect.y = h*0.8;

    //Default values
    *one_player_bool = 1;
    *play_menu_choice = CLASSIC;
    sprintf(nb_image_input, "%d", *nb_image);
    i = strlen(nb_image_input);
    strcat(nb_image_output, nb_image_input);

    //Open fonts
    title_font = TTF_OpenFont(myfont, 50);
    TTF_SetFontStyle(title_font, TTF_STYLE_UNDERLINE);
    one_font = TTF_OpenFont(myfont, 50);
    TTF_SetFontStyle(one_font, TTF_STYLE_UNDERLINE);
    two_font = TTF_OpenFont(myfont, 50);
    classic_font = TTF_OpenFont(myfont, 50);
    TTF_SetFontStyle(classic_font, TTF_STYLE_UNDERLINE);
    rand_font = TTF_OpenFont(myfont, 50);
    training_font = TTF_OpenFont(myfont, 50);
    nb_input_font = TTF_OpenFont(myfont, 50);

    //Color init
    one_color = lime;
    two_color = white;
    classic_color = lime;
    rand_color = white;
    training_color = white;
    training_color_invisible = white;
    training_color_invisible.a = 75;
    
    if (one_font && two_font && classic_font) {
        SDL_RenderClear(renderer);
        //Surfaces
        title_surf = TTF_RenderText_Solid(title_font, title, white);
        one_player_surf = TTF_RenderText_Solid(one_font, one_player, one_color);
        two_player_surf = TTF_RenderText_Solid(two_font, two_player, two_color);
        classic_surf = TTF_RenderText_Solid(classic_font, classic, classic_color);
        random_surf = TTF_RenderText_Solid(rand_font, random, rand_color);
        training_surf = TTF_RenderText_Solid(training_font, training, training_color);
        nb_image_surf = TTF_RenderText_Solid(nb_input_font, nb_image_output, white);
        //Textures
        title_text = SDL_CreateTextureFromSurface(renderer, title_surf);
        one_player_text = SDL_CreateTextureFromSurface(renderer, one_player_surf);
        two_player_text = SDL_CreateTextureFromSurface(renderer, two_player_surf);
        classic_text = SDL_CreateTextureFromSurface(renderer, classic_surf);
        random_text = SDL_CreateTextureFromSurface(renderer, random_surf);
        training_text = SDL_CreateTextureFromSurface(renderer, training_surf);
        nb_image_text = SDL_CreateTextureFromSurface(renderer, nb_image_surf);
        //Renderer
        SDL_RenderCopy(renderer, title_text, NULL, &title_rect);
        SDL_RenderCopy(renderer, one_player_text, NULL, &one_rect);
        SDL_RenderCopy(renderer, two_player_text, NULL, &two_rect);
        SDL_RenderCopy(renderer, classic_text, NULL, &classic_rect);
        SDL_RenderCopy(renderer, random_text, NULL, &rand_rect);
        SDL_RenderCopy(renderer, training_text, NULL, &train_rect);
        SDL_RenderCopy(renderer, nb_image_text, NULL, &nb_image_rect);
        SDL_RenderPresent(renderer);

        while (play) {

            SDL_WaitEvent(&evt);

            switch(evt.type) {
                case SDL_QUIT:
                    *menu = QUIT;
                    play = SDL_FALSE;
                break;
                case SDL_WINDOWEVENT:
                    if(evt.window.event == SDL_WINDOWEVENT_RESIZED) {
                        SDL_GetWindowSize(window, &w, &h);
                        title_rect.w = w/3;
                        title_rect.h = h/7;
                        title_rect.x = w/2 - title_rect.w/2;
                        title_rect.y = 10;

                        one_rect.w = w/7;
                        one_rect.h = h/10;
                        one_rect.x = w/3 - one_rect.w/2;
                        one_rect.y = h/5;

                        two_rect.w = one_rect.w;
                        two_rect.h = one_rect.h;
                        two_rect.x = w * 0.66 - two_rect.w/2;
                        two_rect.y = one_rect.y;

                        classic_rect.w = w/5;
                        classic_rect.h = h/10;
                        classic_rect.x = w/2 - classic_rect.w/2;
                        classic_rect.y = h*0.38;

                        rand_rect.w = w/5;
                        rand_rect.h = h/10;
                        rand_rect.x = w/2 - rand_rect.w/2;
                        rand_rect.y = h/2;

                        train_rect.w = w/4;
                        train_rect.h = h/10;
                        train_rect.x = w/2 - train_rect.w/2;
                        train_rect.y = h*0.62;

                        nb_image_rect.w = w/3;
                        nb_image_rect.h = h/10;
                        nb_image_rect.x = w/2 - nb_image_rect.w/2;
                        nb_image_rect.y = h*0.8;
                    }
                break;
                case SDL_KEYUP:
                    switch(evt.key.keysym.sym) {
                        case SDLK_RIGHT:
                            *one_player_bool = 0;
                            if (*play_menu_choice == TRAINING) {
                                *play_menu_choice = RANDOM;
                            }
                        break;
                        case SDLK_LEFT:
                            *one_player_bool = 1;                        
                        break;
                        case SDLK_UP:
                            if (*play_menu_choice != CLASSIC) (*play_menu_choice)--;
                        break;
                        case SDLK_DOWN:
                            if ((*one_player_bool && *play_menu_choice != TRAINING) || (!*one_player_bool && *play_menu_choice < RANDOM)) (*play_menu_choice)++;
                        break;
                        case SDLK_RETURN:
                            if (is_number(nb_image_input)) {
                                *nb_image = atoi(nb_image_input);
                                if (*nb_image <= nb_image_max) {
                                    /*if (*nb_image % 2) {        
                                        window_popup(renderer, window, "Le nombre doit etre pair.", white, black, 1000);
                                        sprintf(nb_image_input, "%d", *nb_image);
                                        sprintf(nb_image_output, "%s", "Nombre d'images : ");
                                        strcat(nb_image_output, nb_image_input);
                                        i = 2;
                                    }
                                    else {*/
                                        play = SDL_FALSE;
                                    //}
                                }
                                else {
                                    window_popup(renderer, window, "Pas assez d'image(s).", black, silver, 1200);
                                }
                            }
                            else {
                                window_popup(renderer, window, "Erreur nombre invalide.", white, black, 1000);
                            }
                        break;
                        case SDLK_BACKSPACE:
                            if (i) {
                                nb_image_input[--i] = '\0';
                                nb_image_output[strlen(nb_image_output)-1] = '\0';
                            }
                            else {
                                *menu = MAIN_MENU;
                                play = SDL_FALSE;
                            }
                        break;
                    }
                break;
                case SDL_TEXTINPUT:
                    if (i<5) {
                        strcat(nb_image_input, evt.text.text);
                        strcat(nb_image_output, evt.text.text);
                        i++;
                    }
                break;
            }
            if (evt.type == SDL_KEYUP || evt.type == SDL_WINDOWEVENT) {

                SDL_RenderClear(renderer);
                if (*one_player_bool) {
                    one_color = lime;
                    TTF_SetFontStyle(one_font, TTF_STYLE_UNDERLINE);
                    two_color = white;
                    TTF_SetFontStyle(two_font, TTF_STYLE_NORMAL);
                }
                else {
                    two_color = lime;
                    TTF_SetFontStyle(two_font, TTF_STYLE_UNDERLINE);
                    one_color = white;
                    TTF_SetFontStyle(one_font, TTF_STYLE_NORMAL);
                }
                switch (*play_menu_choice) {
                    case CLASSIC:
                        classic_color = lime;
                        rand_color = white;
                        training_color = white;
                        TTF_SetFontStyle(classic_font, TTF_STYLE_UNDERLINE);
                        TTF_SetFontStyle(rand_font, TTF_STYLE_NORMAL);
                        TTF_SetFontStyle(training_font, TTF_STYLE_NORMAL);
                    break;
                    case RANDOM:
                        classic_color = white;
                        rand_color = lime;
                        training_color = white;
                        TTF_SetFontStyle(classic_font, TTF_STYLE_NORMAL);
                        TTF_SetFontStyle(rand_font, TTF_STYLE_UNDERLINE);
                        TTF_SetFontStyle(training_font, TTF_STYLE_NORMAL);
                    break;
                    case TRAINING:
                        classic_color = white;
                        rand_color = white;
                        training_color = lime;
                        TTF_SetFontStyle(classic_font, TTF_STYLE_NORMAL);
                        TTF_SetFontStyle(rand_font, TTF_STYLE_NORMAL);
                        TTF_SetFontStyle(training_font, TTF_STYLE_UNDERLINE);
                    break;
                }
                //Surfaces
                title_surf = TTF_RenderText_Solid(title_font, title, white);
                one_player_surf = TTF_RenderText_Solid(one_font, one_player, one_color);
                two_player_surf = TTF_RenderText_Solid(two_font, two_player, two_color);
                classic_surf = TTF_RenderText_Solid(classic_font, classic, classic_color);
                random_surf = TTF_RenderText_Solid(rand_font, random, rand_color);
                if (*one_player_bool) training_surf = TTF_RenderText_Solid(training_font, training, training_color);
                else training_surf = TTF_RenderText_Solid(training_font, training, training_color_invisible);
                nb_image_surf = TTF_RenderText_Solid(nb_input_font, nb_image_output, white);
                //Textures
                title_text = SDL_CreateTextureFromSurface(renderer, title_surf);
                one_player_text = SDL_CreateTextureFromSurface(renderer, one_player_surf);
                two_player_text = SDL_CreateTextureFromSurface(renderer, two_player_surf);
                classic_text = SDL_CreateTextureFromSurface(renderer, classic_surf);
                random_text = SDL_CreateTextureFromSurface(renderer, random_surf);
                training_text = SDL_CreateTextureFromSurface(renderer, training_surf);
                nb_image_text = SDL_CreateTextureFromSurface(renderer, nb_image_surf);
                //Renderer
                SDL_RenderCopy(renderer, title_text, NULL, &title_rect);
                SDL_RenderCopy(renderer, one_player_text, NULL, &one_rect);
                SDL_RenderCopy(renderer, two_player_text, NULL, &two_rect);
                SDL_RenderCopy(renderer, classic_text, NULL, &classic_rect);
                SDL_RenderCopy(renderer, random_text, NULL, &rand_rect);
                SDL_RenderCopy(renderer, training_text, NULL, &train_rect);
                SDL_RenderCopy(renderer, nb_image_text, NULL, &nb_image_rect);
                SDL_RenderPresent(renderer);

            }
        }
    }
    SDL_RenderClear(renderer);
    //Free
    SDL_FreeSurface(title_surf);
    SDL_DestroyTexture(title_text);
    SDL_FreeSurface(one_player_surf);
    SDL_DestroyTexture(one_player_text);
    SDL_FreeSurface(two_player_surf);
    SDL_DestroyTexture(two_player_text);
    SDL_FreeSurface(classic_surf);
    SDL_DestroyTexture(classic_text);
    SDL_FreeSurface(random_surf);
    SDL_DestroyTexture(random_text);
    SDL_FreeSurface(training_surf);
    SDL_DestroyTexture(training_text);
    TTF_CloseFont(title_font);
    TTF_CloseFont(one_font);
    TTF_CloseFont(two_font);
    TTF_CloseFont(classic_font);
    TTF_CloseFont(rand_font);
    TTF_CloseFont(training_font);
    TTF_CloseFont(nb_input_font);
}

void game_window(SDL_Renderer *renderer, SDL_Window *window, AVL_Image *image_avl, const char *language)
{
    SDL_Rect rect_image, rect_txt, rect_input;
    TTF_Font *font = NULL;
    SDL_Texture *image = NULL, *lang_txt = NULL;
    SDL_Surface *surf = NULL;
    int w, h;
    char *myfont = "data/fonts/retro.ttf", txt[100] = "Langue : ";

    strcat(txt, language);

    SDL_GetWindowSize(window, &w, &h);
    rect_image.x = 50;
    rect_image.y = 15;
    rect_image.w = w-rect_image.x*2;
    rect_image.h = h-(rect_image.y+h*0.33);

    rect_txt.x = w/3;
    rect_txt.y = rect_image.y + rect_image.h + 10;
    rect_txt.w = w/3;
    rect_txt.h = h/9;

    if (image_avl) image = IMG_LoadTexture(renderer, image_avl->image);
    font = TTF_OpenFont(myfont, 50);
    if (font) surf = TTF_RenderText_Solid(font, txt, white);

    if (image && surf) {
        //Print the image from image_avl on screen + language required
        lang_txt = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_RenderCopy(renderer, image, NULL, &rect_image);
        SDL_RenderCopy(renderer, lang_txt, NULL, &rect_txt);
        SDL_RenderPresent(renderer);
    }
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(image);
    SDL_DestroyTexture(lang_txt);
    TTF_CloseFont(font);
}

char *select_language_window(SDL_Renderer *renderer, SDL_Window *window, char **language_array, int array_size, MenuState *menu)
{
    char *title = "Selection de la langue", *myfont = "data/fonts/retro.ttf", flag_path[20];
    int w, h, choice = 0, i=0;
    SDL_Texture *title_text = NULL, *lang_text = NULL, *flag_text = NULL, *rightArrow_text = NULL, *leftArrow_text = NULL;
    SDL_Surface *title_surf = NULL, *lang_surf = NULL;
    TTF_Font *font = NULL, *title_font = NULL;
    SDL_Rect rect_title, rect_lang, rect_flag, rightArrow_rect, leftArrow_rect;
    SDL_bool play = SDL_TRUE;
    SDL_Event evt;

    //Capitalize all languages for better looking on screen
    for (;i<array_size; ++i) {
        capitalize(language_array[i]);
    }

    //Get window size
    SDL_GetWindowSize(window, &w, &h);

    //Set all rectangles
    rect_title.w = w/2;
    rect_title.h = h/7;
    rect_title.x = w/2-rect_title.w/2;
    rect_title.y = 30;

    rect_lang.w = w/4;
    rect_lang.h = h/6;
    rect_lang.x = w/2 - rect_lang.w/2;
    rect_lang.y = h/3;

    rect_flag.w = w/10;
    rect_flag.h = h/10;
    rect_flag.x = w/2 - rect_flag.w/2;
    rect_flag.y = rect_lang.y+rect_lang.h + 20;

    rightArrow_rect.w = w/12; 
    rightArrow_rect.h = h/8;
    rightArrow_rect.x = w*0.75 - rightArrow_rect.w/2;
    rightArrow_rect.y = h/2-rightArrow_rect.h/2;

    leftArrow_rect.w = w/12; 
    leftArrow_rect.h = h/8;
    leftArrow_rect.x = w*0.25 - leftArrow_rect.w/2;
    leftArrow_rect.y = h/2-leftArrow_rect.h/2;

    //Set fonts
    font = TTF_OpenFont(myfont, 50);
    title_font = TTF_OpenFont(myfont, 50);
    TTF_SetFontStyle(title_font, TTF_STYLE_UNDERLINE);

    //Set the flag image path
    sprintf(flag_path, "data/flags/%s.png", language_array[choice]);
    lower(flag_path);

    //Clear old window
    SDL_RenderClear(renderer);

    //Set all surfaces and textures
    title_surf = TTF_RenderText_Solid(title_font, title, white);
    lang_surf = TTF_RenderText_Solid(font,language_array[choice], white);
    title_text = SDL_CreateTextureFromSurface(renderer, title_surf);
    lang_text = SDL_CreateTextureFromSurface(renderer, lang_surf);
    flag_text = IMG_LoadTexture(renderer, flag_path);

    // Open arrows png
    rightArrow_text = IMG_LoadTexture(renderer, "data/utils/rightarrow.png");
    leftArrow_text = IMG_LoadTexture(renderer, "data/utils/leftarrow.png");

    //Copy textures on renderer
    if (choice != 0) SDL_RenderCopy(renderer, leftArrow_text, NULL, &leftArrow_rect);
    if (choice != array_size-1) SDL_RenderCopy(renderer, rightArrow_text, NULL, &rightArrow_rect);
    SDL_RenderCopy(renderer, title_text, NULL, &rect_title);
    SDL_RenderCopy(renderer, lang_text, NULL, &rect_lang);
    SDL_RenderCopy(renderer, flag_text, NULL, &rect_flag);
    SDL_RenderPresent(renderer);

    //Selection window
    while (play && font) {

        SDL_WaitEvent(&evt);

        switch (evt.type) {
            case SDL_QUIT:
                *menu = QUIT;
                play = SDL_FALSE;
            break;
            case SDL_WINDOWEVENT:
                SDL_GetWindowSize(window, &w, &h);
                rect_title.w = w/2;
                rect_title.h = h/7;
                rect_title.x = w/2-rect_title.w/2;
                rect_title.y = 30;

                rect_lang.w = w/4;
                rect_lang.h = h/6;
                rect_lang.x = w/2 - rect_lang.w/2;
                rect_lang.y = h/3;

                rect_flag.w = w/10;
                rect_flag.h = h/10;
                rect_flag.x = w/2 - rect_flag.w/2;
                rect_flag.y = rect_lang.y+rect_lang.h + 20;

                rightArrow_rect.w = w/12; 
                rightArrow_rect.h = h/8;
                rightArrow_rect.x = w*0.75 - rightArrow_rect.w/2;
                rightArrow_rect.y = h/2-rightArrow_rect.h/2;

                leftArrow_rect.w = w/12; 
                leftArrow_rect.h = h/8;
                leftArrow_rect.x = w*0.25 - leftArrow_rect.w/2;
                leftArrow_rect.y = h/2-leftArrow_rect.h/2;
            break;
            case SDL_KEYUP:
                if (evt.key.keysym.sym == SDLK_RETURN) {
                    play = SDL_FALSE;
                }
                else if (evt.key.keysym.sym == SDLK_LEFT) {
                    choice = (!choice) ? 0 : choice-1;
                }
                else if (evt.key.keysym.sym == SDLK_RIGHT) {
                    choice = (choice == array_size-1) ? choice : choice+1;
                }
            break;
        }
        if (evt.window.event == SDL_WINDOWEVENT_RESIZED || evt.type == SDL_KEYUP) {
            //Set the flag image path
            sprintf(flag_path, "data/flags/%s.png", language_array[choice]);
            lower(flag_path);
            //Clear old window
            SDL_RenderClear(renderer);
            //Set all surfaces and textures
            title_surf = TTF_RenderText_Solid(title_font, title, white);
            lang_surf = TTF_RenderText_Solid(font,language_array[choice], white);
            title_text = SDL_CreateTextureFromSurface(renderer, title_surf);
            lang_text = SDL_CreateTextureFromSurface(renderer, lang_surf);
            flag_text = IMG_LoadTexture(renderer, flag_path);
            //Copy textures on renderer
            if (choice != 0) SDL_RenderCopy(renderer, leftArrow_text, NULL, &leftArrow_rect);
            if (choice != array_size-1) SDL_RenderCopy(renderer, rightArrow_text, NULL, &rightArrow_rect);
            SDL_RenderCopy(renderer, title_text, NULL, &rect_title);
            SDL_RenderCopy(renderer, lang_text, NULL, &rect_lang);
            SDL_RenderCopy(renderer, flag_text, NULL, &rect_flag);
            SDL_RenderPresent(renderer);
        }
    }
    for (i=0;i<array_size; ++i) {
        lower(language_array[i]);
    }
    //Free
    SDL_FreeSurface(title_surf);
    SDL_FreeSurface(lang_surf);
    SDL_DestroyTexture(title_text);
    SDL_DestroyTexture(lang_text);
    SDL_DestroyTexture(flag_text);
    SDL_DestroyTexture(rightArrow_text);
    SDL_DestroyTexture(leftArrow_text);
    TTF_CloseFont(font);
    TTF_CloseFont(title_font);
    SDL_RenderClear(renderer);


    return language_array[choice];
}

AVL_Image *select_image_window(SDL_Renderer *renderer, SDL_Window *window, AVL_Image *image_avl, char **french_array, int french_array_size, MenuState *menu)
{
    SDL_Rect input_rect, image_rect, title_rect;
    TTF_Font *font = NULL, *title_font = NULL;
    SDL_Surface *title_surf = NULL, *input_surf = NULL, *image_surf = NULL;
    SDL_Texture *title_text = NULL, *image_text = NULL, *input_text = NULL;
    SDL_bool play = SDL_TRUE;
    SDL_Event evt;
    AVL_Image *temp_image = NULL;
    char
        *title = "Choix de l'image",
        *defaultOutput = "Entrez le nom de l'image...",
        txtInput[28] = "",
        txtInput_cpy[28],
        *no_result = "Aucun resultat.",
        *myfont = "data/fonts/retro.ttf",
        *str = NULL     //The result of the search in the french array
    ;
    int w, h, i = 0;

    //Get Window Size
    SDL_GetWindowSize(window, &w, &h);

    //Rect init 
    title_rect.w = w/2;
    title_rect.h = h/6;
    title_rect.x = w/2 - title_rect.w/2;
    title_rect.y = 10;

    input_rect.w = w/3;
    input_rect.h = h/8;
    input_rect.x = w/2 - input_rect.w/2;
    input_rect.y = h*0.3;

    image_rect.w = w/3;
    image_rect.h = h/3;
    image_rect.x = w/2 - image_rect.w/2;
    image_rect.y = h*0.55;

    //Open font
    font = TTF_OpenFont(myfont, 50);
    title_font = TTF_OpenFont(myfont, 50);
    TTF_SetFontStyle(title_font, TTF_STYLE_UNDERLINE);

    if (font) {
        title_surf = TTF_RenderText_Solid(title_font, title, white);
        input_surf = TTF_RenderText_Solid(font, defaultOutput, white);
        
        title_text = SDL_CreateTextureFromSurface(renderer, title_surf);
        input_text = SDL_CreateTextureFromSurface(renderer, input_surf);

        SDL_RenderCopy(renderer, title_text, NULL, &title_rect);
        SDL_RenderCopy(renderer, input_text, NULL, &input_rect);

        SDL_RenderPresent(renderer);

        while (play) {
            SDL_WaitEvent(&evt);
            switch (evt.type) {
                case SDL_QUIT:
                    *menu = QUIT;
                    play = SDL_FALSE;
                    temp_image = NULL;
                break;
                case SDL_WINDOWEVENT:
                    if (evt.window.event == SDL_WINDOWEVENT_RESIZED) {
                        SDL_GetWindowSize(window, &w, &h);
                        title_rect.w = w/2;
                        title_rect.h = h/6;
                        title_rect.x = w/2 - title_rect.w/2;
                        title_rect.y = 10;

                        input_rect.w = w/3;
                        input_rect.h = h/8;
                        input_rect.x = w/2 - input_rect.w/2;
                        input_rect.y = h*0.3;

                        image_rect.w = w/3;
                        image_rect.h = h/3;
                        image_rect.x = w/2 - image_rect.w/2;
                        image_rect.y = h*0.55;
                    }
                break;
                case SDL_KEYUP:
                    if (evt.key.keysym.sym == SDLK_BACKSPACE) {
                        if (i>0) txtInput[--i] = '\0';
                    }
                    else if (evt.key.keysym.sym == SDLK_RETURN) {
                        if (temp_image) {
                            play = SDL_FALSE;
                        }
                        else {
                            window_popup(renderer, window, "Aucune image. Recommencez.", black, silver, 1200);
                        }
                    }

                break;
                case SDL_TEXTINPUT:
                    if (i<27) {
                        if (is_valid(evt.text.text, 0)) {
                            strcat(txtInput, evt.text.text);
                            i++;
                        }
                        else window_popup(renderer, window, "Caractere invalide.", black, silver, 1000);
                    }
                break;
            }
            if (evt.type == SDL_TEXTINPUT || evt.type == SDL_KEYUP || evt.window.event == SDL_WINDOWEVENT_RESIZED) {
                SDL_RenderClear(renderer);
                if (strlen(txtInput) != 0) {    //If there is a text input
                    //Save user input (save uppercase for exemple)
                    strcpy(txtInput_cpy, txtInput);
                    //Search in the array
                    str = find_in_array(french_array, french_array_size, lower(txtInput));
                    if (str) {       //If there is a result
                        temp_image = find_AVLi(image_avl, str);
                        image_text = IMG_LoadTexture(renderer, temp_image->image);
                    }
                    else {
                        //Surface and texture init for "Aucun resultat."
                        image_surf = TTF_RenderText_Solid(font, no_result, white);
                        image_text = SDL_CreateTextureFromSurface(renderer, image_surf);
                    }
                    //Update rect size
                    input_rect.w = strlen(txtInput)*(w/64);
                    input_rect.x = w/2 - input_rect.w/2;
                    //Update text input texture
                    strcpy(txtInput, txtInput_cpy); //Reset the original input
                    input_surf = TTF_RenderText_Solid(font, txtInput, white);
                    input_text = SDL_CreateTextureFromSurface(renderer, input_surf);
                }
                else {
                    //Reset last search
                    str = NULL;
                    temp_image = NULL;
                    //Surface and texture for "Entrez le nom de l'image..."
                    input_surf = TTF_RenderText_Solid(font, defaultOutput, white);
                    input_text = SDL_CreateTextureFromSurface(renderer, input_surf);
                    input_rect.w = w/4;
                    input_rect.x = w/2 - input_rect.w/2;
                    //Surface and texture init for "Aucun resultat."
                    image_surf = TTF_RenderText_Solid(font, no_result, white);
                    image_text = SDL_CreateTextureFromSurface(renderer, image_surf);
                }

                SDL_RenderCopy(renderer, title_text, NULL, &title_rect);
                SDL_RenderCopy(renderer, input_text, NULL, &input_rect);
                SDL_RenderCopy(renderer, image_text, NULL, &image_rect);
                SDL_RenderPresent(renderer);
            }
        }
    }
    SDL_RenderClear(renderer);
    //Free
    TTF_CloseFont(font);
    TTF_CloseFont(title_font);
    SDL_FreeSurface(title_surf);
    SDL_FreeSurface(input_surf);
    SDL_FreeSurface(image_surf);
    SDL_DestroyTexture(title_text);
    SDL_DestroyTexture(image_text);
    SDL_DestroyTexture(input_text);

    return temp_image;
}

void player_turn_window(SDL_Renderer *renderer, SDL_Window *window, int player_turn, int p1_score, int p2_score)
{
    SDL_Surface *turn_surf = NULL, *player_surf = NULL;
    SDL_Texture *turn_text = NULL, *player_text = NULL;
    TTF_Font *turn_font = NULL, *player_font = NULL;
    SDL_Rect turn_rect, player_rect;
    char 
        *turn = "A votre tour :",
        player[9] = "Joueur ",
        *myfont = "data/fonts/retro.ttf",
        player_nb
    ;
    int w, h;

    //Get window size
    SDL_GetWindowSize(window, &w, &h);

    //Rect init
    turn_rect.w = w/3;
    turn_rect.h = h/4;
    turn_rect.x = w/2 - turn_rect.w/2;
    turn_rect.y = 10;

    player_rect.w = w/4;
    player_rect.h = h/4;
    player_rect.x = w/2 - player_rect.w/2;
    player_rect.y = h * 0.4;
    
    //Setup player
    player_nb = '0' + player_turn;
    player[7] = player_nb;

    //Font init
    turn_font = TTF_OpenFont(myfont, 50);
    TTF_SetFontStyle(turn_font, TTF_STYLE_UNDERLINE);
    player_font = TTF_OpenFont(myfont, 50);

    if (turn_font && player_font) {
        turn_surf = TTF_RenderText_Solid(turn_font, turn, white);
        player_surf = TTF_RenderText_Solid(player_font, player, white);

        turn_text = SDL_CreateTextureFromSurface(renderer, turn_surf);
        player_text = SDL_CreateTextureFromSurface(renderer, player_surf);

        SDL_RenderCopy(renderer, turn_text, NULL, &turn_rect);
        SDL_RenderCopy(renderer, player_text, NULL, &player_rect);
        score_popup(renderer, window, p1_score, p2_score, 1500);
        SDL_RenderClear(renderer);
    }
    //Free
    SDL_FreeSurface(turn_surf);
    SDL_DestroyTexture(turn_text);
    SDL_FreeSurface(player_surf);
    SDL_DestroyTexture(player_text);
    TTF_CloseFont(turn_font);
    TTF_CloseFont(player_font);
}

void test_image(AVL_Image *image_avl, SDL_Renderer *rend, SDL_Window *win)
{
    SDL_Event evt;
    SDL_bool test = SDL_TRUE;
    if (image_avl) {
        game_window(rend, win, image_avl, "test");
        while (test) {
            SDL_WaitEvent(&evt);

            switch (evt.type) {
                case SDL_KEYUP :
                    if (evt.key.keysym.sym == SDLK_RETURN) {
                        test = SDL_FALSE;
                    }
                break;
            }   
        }
        SDL_RenderClear(rend);
        test_image(image_avl->left, rend, win);
        test_image(image_avl->right, rend, win);
    }
}

void random_image(AVL_Image *image_avl, SDL_Renderer *rend, SDL_Window *win, char **const struct1, int struct1_size)
{
    AVL_Image *temp = NULL;
    SDL_bool play = SDL_TRUE;
    SDL_Event evt;
    char *array[1];
    //Charge a array of 1 cell of a random image
    rand_image_array(struct1, struct1_size, array, 1);
    //Find that image in the AVL
    temp = find_AVLi(image_avl, array[0]);
    //Print the window with the image
    game_window(rend, win, temp, "random test");
    
    while (play) {
        SDL_WaitEvent(&evt);
        if (evt.type == SDL_QUIT) {
            play = SDL_FALSE;
        }
    }
    SDL_RenderClear(rend);
}

int play_game_window(AVL_Image *image, AVL_Trans *trans, SDL_Renderer *renderer, SDL_Window *window, MenuState *menu)
{
    SDL_bool play = SDL_TRUE;
    TTF_Font *font = NULL, *title_timer_font = NULL;
    SDL_Surface *txtSurf = NULL, *timer_surf = NULL, *title_timer_surf = NULL;
    SDL_Texture *txtText = NULL, *timer_text = NULL, *title_timer_text = NULL;
    SDL_Event evt;
    SDL_Rect rect_input, timer_rect, title_timer_rect;
    time_t end_timet;
    Mix_Chunk *win_sound = NULL, *lose_sound = NULL, *end_sound = NULL, *boom_sound = NULL;;
    struct tm *end_time = NULL;
    int w, h, i=0, return_value = 0, noSound = 1;
    float time_left;
    char txtInput[50] = "", *txtOutput = "Entrez votre reponse...", word_plural[50] = "tttt", answer[90] = "Bonne reponse : ", timer_char[3] = "10", *time_left_str = "Temps restant :" ;

    //Plural is only available for french language 
    if (!strcmp(trans->language, "francais")) plural(image, trans, word_plural);

    if (image && trans && renderer && window) {

        SDL_GetWindowSize(window, &w, &h);

        //Sound init
        win_sound = Mix_LoadWAV("data/music/win.wav");
        lose_sound = Mix_LoadWAV("data/music/lose.wav");
        end_sound = Mix_LoadWAV("data/music/end_timer.wav");
        boom_sound = Mix_LoadWAV("data/music/boom.wav");

        //Setup input text box
        font = TTF_OpenFont("data/fonts/retro.ttf", 50);
        title_timer_font = TTF_OpenFont("data/fonts/retro.ttf", 50);
        TTF_SetFontStyle(title_timer_font, TTF_STYLE_UNDERLINE);
        rect_input.w = w/4;
        rect_input.h = h/10;
        rect_input.x = w/2 - rect_input.w/2;
        rect_input.y = (h-(15+h*0.33)) + h/9 + 30 ;

        //Timer rect
        timer_rect.w = w/13;
        timer_rect.h = h/7;
        timer_rect.x = w*0.1 - timer_rect.w/2;
        timer_rect.y = h*0.8;

        title_timer_rect.w = w/6;
        title_timer_rect.h = h/11;
        title_timer_rect.x = w*0.1- title_timer_rect.w/2;
        title_timer_rect.y = h*0.7;

        //Start renderer
        if (font && title_timer_font) {
            title_timer_surf = TTF_RenderText_Solid(title_timer_font, time_left_str, white);
            title_timer_text = SDL_CreateTextureFromSurface(renderer, title_timer_surf);
            txtSurf = TTF_RenderText_Solid(font, txtOutput, white);
            txtText = SDL_CreateTextureFromSurface(renderer, txtSurf);
            timer_surf = TTF_RenderText_Solid(font, timer_char, white);
            timer_text = SDL_CreateTextureFromSurface(renderer, timer_surf);
            SDL_RenderCopy(renderer, title_timer_text, NULL, &title_timer_rect);
            SDL_RenderCopy(renderer, timer_text, NULL, &timer_rect);
            SDL_RenderCopy(renderer, txtText, NULL, &rect_input);
            game_window(renderer, window, image, trans->language);
        }

        //Begin timer
        end_timet = time(&end_timet);
        //Set end_time
        if (end_time = localtime(&end_timet)) {
            end_time->tm_sec += 10;
        }
    
        while (play && ((time_left = difftime(mktime(end_time), time(NULL))) > 0)) { //time(NULL) = current time

            if ((int)time_left < atoi(timer_char)) {
                if (time_left == 9) {
                    timer_rect.w /= 2;
                    timer_rect.x = w*0.1 - timer_rect.w/2;
                }
                if (((int)time_left < 6) && noSound)  {
                    Mix_PlayChannel(5, end_sound, -1);
                    noSound = 0;
                }
                SDL_RenderClear(renderer);
                sprintf(timer_char, "%.0f", time_left);
                timer_surf = TTF_RenderText_Solid(font, timer_char, white);
                timer_text = SDL_CreateTextureFromSurface(renderer, timer_surf);
                SDL_RenderCopy(renderer, title_timer_text, NULL, &title_timer_rect);
                SDL_RenderCopy(renderer, timer_text, NULL, &timer_rect);
                SDL_RenderCopy(renderer, txtText, NULL, &rect_input);
                game_window(renderer, window, image, trans->language);
            }

            while (SDL_PollEvent(&evt)) {

                switch (evt.type) {
                    case SDL_QUIT:
                        *menu = QUIT;
                        play = SDL_FALSE;
                    break;
                    case SDL_WINDOWEVENT:
                        if (evt.window.event == SDL_WINDOWEVENT_RESIZED) {
                            SDL_GetWindowSize(window, &w, &h);

                            timer_rect.w = w/13;
                            if (time_left<=9) timer_rect.w /= 2;
                            timer_rect.h = h/7;
                            timer_rect.x = w*0.1 - timer_rect.w/2;
                            timer_rect.y = h*0.8;

                            title_timer_rect.w = w/6;
                            title_timer_rect.h = h/11;
                            title_timer_rect.x = w*0.1- title_timer_rect.w/2;
                            title_timer_rect.y = h*0.7;
                        
                            rect_input.w = w/4;
                            rect_input.h = h/10;
                            rect_input.x = w/2 - rect_input.w/2;
                            rect_input.y = (h-(15+h*0.33)) + h/9 + 30;
                        }
                    break;
                    case SDL_KEYUP:
                        if (evt.key.keysym.sym == SDLK_RETURN) {
                            if (strlen(txtInput)) {
                                if (is_valid(lower(txtInput), 0)) {
                                    //printf("txtInput = %s\nword_plural = %s\n", txtInput, word_plural);
                                    //Stop end_sound
                                    Mix_HaltChannel(5);
                                    if (!strcmp(trans->word, txtInput) || !strcmp(word_plural, txtInput)) {
                                        Mix_PlayChannel(-1, win_sound, 0);
                                        return_value = 1;
                                        win_lose_popup(renderer, window, 1, 1000);
                                    }
                                    else {
                                        Mix_PlayChannel(-1, lose_sound, 0);
                                        return_value = 0;
                                        strcat(answer, trans->word);
                                        win_lose_popup(renderer, window, 0, 0);
                                        window_popup(renderer, window, answer, white, black, 1500);
                                    }
                                    play = SDL_FALSE;
                                }
                                else {
                                    window_popup(renderer, window, "Invalide. Recommencer", white, black, 1000);
                                    end_time->tm_sec += 1;
                                    strcpy(txtInput, "");
                                    i=0;
                                }
                            }
                            else {
                                window_popup(renderer, window, "Pas de mot.", black, silver, 1000);
                                end_time->tm_sec += 1;
                            }

                        }
                        else if (evt.key.keysym.sym == SDLK_BACKSPACE) {
                            if (i) txtInput[--i] = '\0';
                        }
                    break;
                    case SDL_TEXTINPUT:
                        if (i<49) {
                            strcat(txtInput, evt.text.text);
                            i++;
                        }
                    break;
                }
                if (evt.type == SDL_TEXTINPUT || evt.type == SDL_KEYUP || evt.window.event == SDL_WINDOWEVENT_RESIZED) {
                    SDL_RenderClear(renderer);
                    if (strlen(txtInput) != 0) {    //Print text input
                        //Resized if char is added
                        rect_input.w = (w/64)*strlen(txtInput);
                        rect_input.x = w/2 - rect_input.w/2;
                        //Update txtInput on screen
                        txtSurf = TTF_RenderText_Solid(font, txtInput, white);
                        txtText = SDL_CreateTextureFromSurface(renderer, txtSurf);
                    }
                    else {                          //Print "Entrer votre reponse..." 
                        txtSurf = TTF_RenderText_Solid(font, txtOutput, white);
                        txtText = SDL_CreateTextureFromSurface(renderer, txtSurf);
                        rect_input.w = w/4;
                        rect_input.h = h/10;
                        rect_input.x = w/2 - rect_input.w/2;
                        rect_input.y = (h-(15+h*0.33)) + h/9 + 30 ;
                    }
                    SDL_RenderCopy(renderer, title_timer_text, NULL, &title_timer_rect);
                    SDL_RenderCopy(renderer, timer_text, NULL, &timer_rect);
                    SDL_RenderCopy(renderer, txtText, NULL, &rect_input);
                    game_window(renderer, window, image, trans->language);
                }
            }
        }
        //Stop the end_timer sounds
        Mix_HaltChannel(5);
        if (time_left<1) {
            //Play sound
            Mix_PlayChannel(-1, boom_sound, 0);
            //Print 0 + popup on screen
            SDL_RenderClear(renderer);
            sprintf(timer_char, "%.0f", time_left);
            timer_surf = TTF_RenderText_Solid(font, timer_char, white);
            timer_text = SDL_CreateTextureFromSurface(renderer, timer_surf);
            SDL_RenderCopy(renderer, title_timer_text, NULL, &title_timer_rect);
            SDL_RenderCopy(renderer, timer_text, NULL, &timer_rect);
            SDL_RenderCopy(renderer, txtText, NULL, &rect_input);
            game_window(renderer, window, image, trans->language);
            window_popup(renderer, window, "Temps ecoule.", black, silver, 1200);
        }
    }
    SDL_RenderClear(renderer);
    //Free
    SDL_FreeSurface(timer_surf);
    SDL_FreeSurface(txtSurf);
    SDL_FreeSurface(title_timer_surf);
    SDL_DestroyTexture(timer_text);
    SDL_DestroyTexture(txtText);
    SDL_DestroyTexture(title_timer_text);
    TTF_CloseFont(font);
    TTF_CloseFont(title_timer_font);
    Mix_FreeChunk(win_sound);
    Mix_FreeChunk(lose_sound);
    Mix_FreeChunk(end_sound);
    Mix_FreeChunk(boom_sound);
    return return_value;
}

void end_game_window(SDL_Renderer *renderer, SDL_Window *window, int one_player_bool, int p1_score, int p2_score, int score_max, MenuState *menu)
{
    TTF_Font *title_font = NULL, *font = NULL;
    SDL_Surface *title_surf = NULL, *text_surf = NULL, *score_surf = NULL, *pressEnter_surf = NULL;
    SDL_Texture *title_text = NULL, *image_text = NULL, *score_text = NULL, *pressEnter_text = NULL;
    SDL_Rect title_rect, score_rect, image_rect, pressEnter_rect;
    SDL_bool play = SDL_TRUE;
    SDL_Event evt;
    SDL_Color white_alpha = white;
    int  w, h, alpha_down = 1;
    float score_ratio, alpha_float = 0;
    char 
        *myfont = "data/fonts/retro.ttf",
        *title_one = "Votre score :",
        *title_winner = "Score final :",
        winner[25],
        *gold_medal = "data/medals/gold.png",
        *silver_medal = "data/medals/silver.png",
        *bronze_medal = "data/medals/bronze.png",
        *pressEnter ="Appuyez sur ENTRER",
        score[15]
    ;

    SDL_GetWindowSize(window, &w, &h);

    title_rect.w = w/3;
    title_rect.h = h/5;
    title_rect.x = w/2 - title_rect.w/2;
    title_rect.y = 10;

    image_rect.w = w/9;
    image_rect.h = h/4;
    image_rect.y = h*0.35;

    score_rect.h = h/6;
    score_rect.y = h*0.4;

    pressEnter_rect.w = w/3;
    pressEnter_rect.h = h/6;
    pressEnter_rect.x = w/2 - pressEnter_rect.w/2;
    pressEnter_rect.y = h*0.7;

    title_font = TTF_OpenFont(myfont, 50);
    TTF_SetFontStyle(title_font, TTF_STYLE_UNDERLINE);
    font = TTF_OpenFont(myfont, 50);

    if (font && title_font) {
        //Press enter surf and text
        pressEnter_surf = TTF_RenderText_Solid(title_font, pressEnter, white);
        pressEnter_text = SDL_CreateTextureFromSurface(renderer, pressEnter_surf);
        SDL_RenderCopy(renderer, pressEnter_text, NULL, &pressEnter_rect);

        if (one_player_bool) {
            //Score output
            sprintf(score, "%d sur %d", p1_score, score_max);
            score_surf = TTF_RenderText_Solid(font, score, white);
            score_text = SDL_CreateTextureFromSurface(renderer, score_surf);

            //Score ratio
            score_ratio = (float)p1_score/score_max;
            //Debug
            //printf("score :%f\np1_score:%d\nscore_max:%d\n", score_ratio, p1_score, score_max);

            //Switching medals
            if (score_ratio >= 0.84) {    //84 to 100% -> gold medal
                image_text = IMG_LoadTexture(renderer, gold_medal);
            }
            else if (score_ratio >= 0.66) {     //66 to 84 % -> silver medal
                image_text = IMG_LoadTexture(renderer, silver_medal);
            }
            else if (score_ratio >= 0.5) {   //50 to 66 % -> bronze medal
                image_text = IMG_LoadTexture(renderer, bronze_medal);
            }

            //Update rect
            score_rect.w = strlen(score)*(w/38);
            if (image_text) score_rect.x = w/2-(score_rect.w+image_rect.w+20)/2;
            else score_rect.x = w/2-score_rect.w/2;
            image_rect.x = score_rect.x + score_rect.w + 20;

            //Title surface and texture
            title_surf = TTF_RenderText_Solid(title_font, title_one, white);
            title_text = SDL_CreateTextureFromSurface(renderer, title_surf);

            SDL_RenderCopy(renderer, title_text, NULL, &title_rect);
            SDL_RenderCopy(renderer, score_text, NULL, &score_rect);
            if (image_text) SDL_RenderCopy(renderer, image_text, NULL, &image_rect);
            SDL_RenderPresent(renderer);
            //SDL_Delay(2500);
        }
        else {
            if (p1_score > p2_score) {
                sprintf(winner, "Le joueur %d gagne !", 1);
            }
            else if (p2_score > p1_score) {
                sprintf(winner, "Le joueur %d gagne !", 2);
            }
            else strcpy(winner, "Egalite !");

            title_surf = TTF_RenderText_Solid(title_font, title_winner, white);
            title_text = SDL_CreateTextureFromSurface(renderer, title_surf);

            score_surf = TTF_RenderText_Solid(font, winner, white);
            score_text = SDL_CreateTextureFromSurface(renderer, score_surf);
            //Update rect
            score_rect.w = strlen(winner) * (w/48);
            score_rect.x = w/2-score_rect.w/2;

            SDL_RenderCopy(renderer, title_text, NULL, &title_rect);
            SDL_RenderCopy(renderer, score_text, NULL, &score_rect);
            score_popup(renderer, window, p1_score, p2_score, 0);
        }

        while (play) {
            alpha_float = ((int)alpha_float == 1) ? 0 : alpha_float + 0.333;

            if (alpha_down) {
                    white_alpha.a -= (int)alpha_float;
                    if (white_alpha.a == 1) alpha_down = 0;
                }
                else {
                    white_alpha.a += (int)alpha_float;
                    if (white_alpha.a == 255) alpha_down = 1;
                }

                pressEnter_surf = TTF_RenderText_Solid(title_font, pressEnter, white_alpha);
                pressEnter_text = SDL_CreateTextureFromSurface(renderer, pressEnter_surf);

                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, title_text, NULL, &title_rect);
                SDL_RenderCopy(renderer, score_text, NULL, &score_rect);
                SDL_RenderCopy(renderer, pressEnter_text, NULL, &pressEnter_rect);
                if (!one_player_bool) score_popup(renderer, window, p1_score, p2_score, 0);
                if (image_text) SDL_RenderCopy(renderer, image_text, NULL, &image_rect);
                SDL_RenderPresent(renderer);

            while (SDL_PollEvent(&evt)) {

                switch (evt.type) {
                    case SDL_QUIT:
                        *menu = QUIT;
                        play = SDL_FALSE;
                    break;
                    case SDL_WINDOWEVENT:
                        if (evt.window.event = SDL_WINDOWEVENT_RESIZED) {
                            SDL_GetWindowSize(window, &w, &h);
                            title_rect.w = w/3;
                            title_rect.h = h/5;
                            title_rect.x = w/2 - title_rect.w/2;
                            title_rect.y = 10;

                            image_rect.w = w/9;
                            image_rect.h = h/4;
                            image_rect.y = h*0.35;

                            score_rect.h = h/6;
                            score_rect.y = h*0.4;

                            pressEnter_rect.w = w/3;
                            pressEnter_rect.h = h/6;
                            pressEnter_rect.x = w/2 - pressEnter_rect.w/2;
                            pressEnter_rect.y = h*0.7;
                            if (one_player_bool) {
                                score_rect.w = strlen(score)*(w/38);
                                if (image_text) score_rect.x = w/2-(score_rect.w+image_rect.w+20)/2;
                                else score_rect.x = w/2-score_rect.w/2;
                                image_rect.x = score_rect.x + score_rect.w + 20;
                            }
                            else {
                                score_rect.w = strlen(winner) * (w/48);
                                score_rect.x = w/2-score_rect.w/2;
                            }
                        }
                    break;
                    case SDL_KEYUP:
                        if (evt.key.keysym.sym == SDLK_RETURN) play = SDL_FALSE;
                    break;
                }
            }
        }

        SDL_RenderClear(renderer);
    }

    SDL_FreeSurface(title_surf);
    SDL_FreeSurface(score_surf);
    SDL_FreeSurface(pressEnter_surf);
    SDL_DestroyTexture(image_text);
    SDL_DestroyTexture(title_text);
    SDL_DestroyTexture(score_text);
    SDL_DestroyTexture(pressEnter_text);
    TTF_CloseFont(title_font);
    TTF_CloseFont(font);
}

void training_mode(SDL_Renderer *renderer, SDL_Window *window, AVL_Image *image_avl, char **french_array, int french_array_size, char *language, MenuState *menu) 
{
    AVL_Image *temp_image = NULL;
    AVL_Trans *temp_trans;
    TTF_Font *font = NULL;
    SDL_Surface *txt_surf = NULL, *stop_surf = NULL;
    SDL_Texture *txt_text = NULL, *stop_text = NULL, *quitImage_text = NULL;
    SDL_Rect rect_input, stop_rect, quitImage_rect;
    SDL_Event evt;
    SDL_bool play = SDL_TRUE;
    int w, h, i = 0, j=0;;
    char *font_path = "data/fonts/retro.ttf", *txtOutput = "Entrez votre reponse...", txtInput[50] = "", input_plural[50] = "tsodzu", *quit = "Quitter";


    SDL_GetWindowSize(window, &w, &h);

    rect_input.w = w/4;
    rect_input.h = h/10;
    rect_input.x = w/2 - rect_input.w/2;
    rect_input.y = (h-(15+h*0.33)) + h/9 + 30 ;

    stop_rect.w = w/11;
    stop_rect.h = h/16;
    stop_rect.x = w*0.09-stop_rect.w/2;
    stop_rect.y = h*0.84;

    quitImage_rect.w = w/15;
    quitImage_rect.h = h/8;
    quitImage_rect.x = w*0.09-quitImage_rect.w/2;
    quitImage_rect.y = h*0.71;

    font = TTF_OpenFont(font_path, 50);

    if (font) {
        temp_image = find_AVLi(image_avl, french_array[i]);
        temp_trans = find_AVLt(temp_image->translation, language, NULL, 0);
        if (!strcmp(temp_trans->language, "francais")) plural(temp_image, temp_image->translation, input_plural);

        txt_surf = TTF_RenderText_Solid(font, txtOutput, white);
        txt_text = SDL_CreateTextureFromSurface(renderer, txt_surf);

        stop_surf = TTF_RenderText_Solid(font, quit, white);
        stop_text = SDL_CreateTextureFromSurface(renderer, stop_surf);

        quitImage_text = IMG_LoadTexture(renderer, "data/utils/exit.png");

        SDL_RenderCopy(renderer, stop_text, NULL, &stop_rect);
        SDL_RenderCopy(renderer, txt_text, NULL, &rect_input);
        SDL_RenderCopy(renderer, quitImage_text, NULL, &quitImage_rect);
        game_window(renderer, window, temp_image, temp_trans->language);

        while (play && temp_image && temp_trans) {
            SDL_WaitEvent(&evt);
            switch (evt.type) {
                case SDL_WINDOWEVENT:
                    if (evt.window.event = SDL_WINDOWEVENT_RESIZED) {
                        SDL_GetWindowSize(window, &w, &h);
                        rect_input.h = h/10;
                        rect_input.y = (h-(15+h*0.33)) + h/9 + 30;

                        stop_rect.w = w/11;
                        stop_rect.h = h/16;
                        stop_rect.x = w*0.09-stop_rect.w/2;
                        stop_rect.y = h*0.84;

                        quitImage_rect.w = w/15;
                        quitImage_rect.h = h/8;
                        quitImage_rect.x = w*0.09-quitImage_rect.w/2;
                        quitImage_rect.y = h*0.71;
                    }
                break;
                case SDL_KEYUP:
                    switch(evt.key.keysym.sym) {
                        case SDLK_RETURN:
                            if (is_valid(txtInput, 0)) {
                                if(strcmp(txtInput, temp_trans->word) == 0 || strcmp(txtInput, input_plural) == 0) {
                                    if (++i == french_array_size) play = SDL_FALSE;
                                    else {
                                        temp_image = find_AVLi(image_avl, french_array[i]);
                                        temp_trans = find_AVLt(temp_image->translation, language, NULL, 0);
                                    }
                                }
                            }
                            strcpy(txtInput, "");
                            j=0;
                        break;
                        case SDLK_LEFT:
                            if (i) {
                                --i;
                                strcpy(txtInput, "");
                                temp_image = find_AVLi(image_avl, french_array[i]);
                                temp_trans = find_AVLt(temp_image->translation, language, NULL, 0);
                                if (!strcmp(language, "francais")) plural(temp_image, temp_trans, input_plural);
                            }
                        break;
                        case SDLK_RIGHT:
                            if (i < french_array_size-1) { 
                                ++i;
                                strcpy(txtInput, "");
                                temp_image = find_AVLi(image_avl, french_array[i]);
                                temp_trans = find_AVLt(temp_image->translation, language, NULL, 0);
                                if (!strcmp(language, "francais")) plural(temp_image, temp_trans, input_plural);
                            }
                        break;
                        case SDLK_BACKSPACE:
                            if (j) txtInput[--j] = '\0';
                        break;
                    }
                break;
                case SDL_MOUSEBUTTONUP:
                    if (evt.button.x >= stop_rect.x && evt.button.x <= (stop_rect.x+stop_rect.w) && evt.button.y >= quitImage_rect.y && evt.button.y <= (stop_rect.y+stop_rect.h)) {
                        play = SDL_FALSE;
                    }
                break;
                case SDL_TEXTINPUT:
                    if (j<sizeof(txtInput)-2) {
                        strcat(txtInput, evt.text.text);
                        j++;
                    }
                break;
                case SDL_QUIT:
                    *menu = QUIT;
                    play = SDL_FALSE;
                break;
            }
            if (evt.type == SDL_KEYUP || evt.type == SDL_TEXTINPUT || evt.window.event == SDL_WINDOWEVENT_RESIZED) {
                SDL_RenderClear(renderer);
                if (strlen(txtInput) != 0) {
                    //Resize rect
                    rect_input.w = (w/64)*strlen(txtInput);
                    rect_input.x = w/2 - rect_input.w/2;
                    //Update output
                    txt_surf = TTF_RenderText_Solid(font, txtInput, white);
                    txt_text = SDL_CreateTextureFromSurface(renderer, txt_surf);
                }
                else {
                    rect_input.w = w/4;
                    rect_input.x = w/2 - rect_input.w/2;
                    txt_surf = TTF_RenderText_Solid(font, txtOutput, white);
                    txt_text = SDL_CreateTextureFromSurface(renderer, txt_surf);
                }
                stop_surf = TTF_RenderText_Solid(font, quit, white);
                stop_text = SDL_CreateTextureFromSurface(renderer, stop_surf);
               
                SDL_RenderCopy(renderer, stop_text, NULL, &stop_rect);
                SDL_RenderCopy(renderer, txt_text, NULL, &rect_input);
                SDL_RenderCopy(renderer, quitImage_text, NULL, &quitImage_rect);
                game_window(renderer, window, temp_image, temp_trans->language);
            }
        }
        SDL_RenderClear(renderer);
    }

    TTF_CloseFont(font);
    SDL_FreeSurface(txt_surf);
    SDL_FreeSurface(stop_surf);
    SDL_DestroyTexture(stop_text);
    SDL_DestroyTexture(txt_text);
    SDL_DestroyTexture(quitImage_text);
}

void play_game(SDL_Renderer *renderer, SDL_Window *window, AVL_Image *image, char **french_array, int french_array_size, char **language_array, int lang_array_size, int one_player, int nb_images, Mode mode, MenuState *menu)
{
    AVL_Image *temp = NULL;
    Mix_Music *music = NULL;
    Mix_Chunk *enter = NULL;
    char *language = NULL;
    char *random[(one_player) ? nb_images : (nb_images*2)];
    int i, p1_score = 0, p2_score = 0;

    //Music init
    music = Mix_LoadMUS("data/music/play_game.mp3");
    Mix_PlayMusic(music, -1);
    enter = Mix_LoadWAV("data/music/enter.wav");

    if (one_player) {
        switch (mode) {
            case CLASSIC:       //Random image, language defined
                language = select_language_window(renderer, window, language_array, lang_array_size, menu);
                Mix_PlayChannel(-1, enter, 0);
                if (*menu != QUIT) {
                    rand_image_array(french_array, french_array_size, random, nb_images);

                    for (i=0; i<nb_images; ++i) {
                        temp = find_AVLi(image, random[i]);
                        p1_score += play_game_window(temp, find_AVLt(temp->translation, language, NULL, 0), renderer, window, menu);
                        if (*menu == QUIT) break;
                        if (i == nb_images-1)  end_game_window(renderer, window, 1, p1_score, 0, nb_images, menu);
                    }
                }
            break;
            case RANDOM:        //Random image, random language
                rand_image_array(french_array, french_array_size, random, nb_images);

                for (i=0; i<nb_images; ++i) {
                    temp = find_AVLi(image, random[i]);
                    p1_score += play_game_window(temp, find_AVLt(temp->translation, NULL, language_array, lang_array_size), renderer, window, menu);
                    if (*menu == QUIT) break;
                    if (i == nb_images-1) end_game_window(renderer, window, 1, p1_score, 0, nb_images, menu);
                }
            break;
            case TRAINING:      //Image in order, language defined
                language = select_language_window(renderer, window, language_array, lang_array_size, menu);
                if (*menu != QUIT) {
                    training_mode(renderer, window, image, french_array, french_array_size, language, menu);
                }
            break;
        }
    }
    else {
        switch (mode) {
            case CLASSIC:       //Image chosen by players, language defined
                language = select_language_window(renderer, window, language_array, lang_array_size, menu);
                if (*menu != QUIT) {
                    player_turn_window(renderer, window, 2, p1_score, p2_score);
                    temp = select_image_window(renderer, window, image, french_array, french_array_size, menu);
                    if (*menu != QUIT) {
                        player_turn_window(renderer, window, 1, p1_score, p2_score);

                        for (i=0; i<(nb_images*2); ++i) {
                            if(i%2+1 == 1) {    //Player 1 turn
                                p1_score += play_game_window(temp, find_AVLt(temp->translation, language, NULL, 0), renderer, window, menu);                    }
                            else {              //Player 2 turn
                                p2_score += play_game_window(temp, find_AVLt(temp->translation, language, NULL, 0), renderer, window, menu);
                            }
                            if (*menu == QUIT) break;
                            //If it's not the end of the game
                            if (i+1 < nb_images*2) {
                                temp = select_image_window(renderer, window, image, french_array, french_array_size, menu);
                                if (*menu == QUIT) break;
                                player_turn_window(renderer, window, (i+1)%2 +1, p1_score, p2_score);
                            }   
                            else {
                                end_game_window(renderer, window, 0, p1_score, p2_score, nb_images, menu);
                            }
                        }
                    }
                }
            break;
            case RANDOM:        //Random image, random language
                rand_image_array(french_array, french_array_size, random, nb_images*2);

                for (i=0; i<(nb_images*2); ++i) {
                    player_turn_window(renderer, window, i%2+1, p1_score, p2_score);
                    temp = find_AVLi(image, random[i]);
                    if (i%2+1 == 1) p1_score += play_game_window(temp, find_AVLt(temp->translation, NULL, language_array, lang_array_size), renderer, window, menu);
                    else p2_score += play_game_window(temp, find_AVLt(temp->translation, NULL, language_array, lang_array_size), renderer, window, menu);
                    if (*menu == QUIT) break;
                    if (i == (nb_images*2)-1) end_game_window(renderer, window, 0, p1_score, p2_score, nb_images, menu);
                }
            break;
        }
    }
    Mix_FreeMusic(music);
    Mix_FreeChunk(enter);
}

/*========= Add image ==========*/

void add_trans_window(SDL_Renderer *renderer, SDL_Window *window, char *french_word, char *language, char *input, int input_size)
{
    TTF_Font *font = NULL, *title_font = NULL;
    SDL_Rect title_rect, flag_rect, input_rect;
    SDL_Surface *title_surf = NULL, *input_surf = NULL;
    SDL_Texture *title_text = NULL, *flag_text = NULL, *input_text = NULL;
    SDL_bool play = SDL_TRUE;
    SDL_Event evt;
    int w, h, i = 0;
    char 
        title[40],
        *no_input = "Entrez une traduction...", 
        *myfont = "data/fonts/retro.ttf",
        flag_path[40]
    ;

    //Get window size
    SDL_GetWindowSize(window, &w, &h);

    //Rect init
    title_rect.w = w*0.8;
    title_rect.h = h/5;
    title_rect.x = w/2-title_rect.w/2;
    title_rect.y = h/6;

    flag_rect.w = w/5;
    flag_rect.h = h/5;
    flag_rect.x = w/2-flag_rect.w/2;
    flag_rect.y = h*0.4;

    input_rect.w = w/2;
    input_rect.h = h/6;
    input_rect.x = w/2 - input_rect.w/2;
    input_rect.y = h*0.65;

    //Open fonts
    font = TTF_OpenFont(myfont, 50);
    title_font = TTF_OpenFont(myfont, 50);
    TTF_SetFontStyle(title_font, TTF_STYLE_UNDERLINE);

    //Title
    if (!strcmp(language, "francais")) {
        sprintf(title, "Entrez la traduction en %s :", language);
    }
    else {
        sprintf(title, "Entrez \"%s\" en %s :",french_word, language);
    }
    //Flag image
    sprintf(flag_path, "data/flags/%s.png", language);

    if (font && title_font) {
        SDL_RenderClear(renderer);
        title_surf = TTF_RenderText_Solid(title_font, title, white);
        input_surf = TTF_RenderText_Solid(font, no_input, white);

        title_text = SDL_CreateTextureFromSurface(renderer, title_surf);
        input_text = SDL_CreateTextureFromSurface(renderer, input_surf);
        flag_text = IMG_LoadTexture(renderer, flag_path);

        SDL_RenderCopy(renderer, title_text, NULL, &title_rect);
        SDL_RenderCopy(renderer, input_text, NULL, &input_rect);
        SDL_RenderCopy(renderer, flag_text, NULL, &flag_rect);

        SDL_RenderPresent(renderer);

        while (play) {
            SDL_WaitEvent(&evt);
            switch(evt.type) {
                case SDL_WINDOWEVENT:
                    if (evt.window.event == SDL_WINDOWEVENT_RESIZED) {
                        SDL_GetWindowSize(window, &w, &h);
                        title_rect.w = w*0.8;
                        title_rect.h = h/5;
                        title_rect.x = w/2-title_rect.w/2;
                        title_rect.y = h/6;

                        flag_rect.w = w/5;
                        flag_rect.h = h/5;
                        flag_rect.x = w/2-flag_rect.w/2;
                        flag_rect.y = h*0.4;

                        input_rect.h = h/6;
                        input_rect.y = h*0.65;          
                    }
                break;
                case SDL_KEYUP:
                    if (evt.key.keysym.sym == SDLK_BACKSPACE) {
                        if (strlen(input)) input[--i] = '\0';
                    }
                    if (evt.key.keysym.sym == SDLK_RETURN) {
                        if (strlen(input) == 0) {
                            window_popup(renderer, window, "Entrez une traduction.", black, silver, 1500);
                        }
                        else {
                            if (is_valid(lower(input), 0)) play = SDL_FALSE;
                            else {
                                window_popup(renderer, window, "Mot invalide. Recommencez.", black, silver, 1500);
                                strcpy(input, "\0");
                                i = 0;
                            }
                        }
                    }
                break;
                case SDL_TEXTINPUT:
                    if (i< input_size-1) {
                        strcat(input, evt.text.text);
                        i++;
                    }
                break;
            }
            if (evt.type == SDL_KEYUP || evt.type == SDL_TEXTINPUT ||evt.window.event == SDL_WINDOWEVENT_RESIZED) {
                SDL_RenderClear(renderer);
                
                if (strlen(input)) {
                    input_surf = TTF_RenderText_Solid(font, input, white);
                    input_rect.w = strlen(input) * (w/48);
                }
                else {
                    input_surf = TTF_RenderText_Solid(font, no_input, white);
                    input_rect.w = w/2;
                }
                input_rect.x = w/2-input_rect.w/2;
                input_text = SDL_CreateTextureFromSurface(renderer, input_surf);

                SDL_RenderCopy(renderer, title_text, NULL, &title_rect);
                SDL_RenderCopy(renderer, input_text, NULL, &input_rect);
                SDL_RenderCopy(renderer, flag_text, NULL, &flag_rect);
                SDL_RenderPresent(renderer);
            }
        }
    }

    SDL_RenderClear(renderer);

    SDL_FreeSurface(title_surf);
    SDL_FreeSurface(input_surf);
    SDL_DestroyTexture(title_text);
    SDL_DestroyTexture(input_text);
    SDL_DestroyTexture(flag_text);
    TTF_CloseFont(font);
    TTF_CloseFont(title_font);
}

void add_image_menu(SDL_Renderer *renderer, SDL_Window *window, AVL_Image **image_avl, char ***french_array, int *french_array_size, char **language_array, int lang_array_size, MenuState *menu)
{
    TTF_Font *font = NULL;
    SDL_Rect title_rect, input_rect;
    SDL_Surface *title_surf = NULL, *input_surf = NULL;
    SDL_Texture *title_text = NULL, *input_text = NULL;
    SDL_bool play = SDL_TRUE, play_depot = SDL_TRUE;
    SDL_Event evt;
    AVL_Image *new = NULL;
    AVL_Trans *new_trans[lang_array_size], *new_avl_trans = NULL;
    int w, h, i=0, isOK = 0, j;
    char
        *title = "Entrez le nom de l'image ainsi que son extension (.png ou .jpg) :",
        image_input[38] = "",
        word_input[50] = "",
        *no_input = "Entrez le nom de l'image...",
        *deposit = "Deposez l'image dans ce dossier.",
        *myfont = "data/fonts/retro.ttf",
        *dropped = NULL;
    ;

    SDL_GetWindowSize(window, &w, &h);

    title_rect.w = w*0.95;
    title_rect.h = h/7;
    title_rect.x = w/2-title_rect.w/2;
    title_rect.y = h*0.1;

    input_rect.w = w/3;
    input_rect.h = h/7;
    input_rect.x = w/2-input_rect.w/2;
    input_rect.y = h*0.35;

    font = TTF_OpenFont(myfont, 50);

    if (font) {
        SDL_RenderClear(renderer);
        title_surf = TTF_RenderText_Solid(font, title, white);
        input_surf = TTF_RenderText_Solid(font, no_input, white);

        title_text = SDL_CreateTextureFromSurface(renderer, title_surf);
        input_text = SDL_CreateTextureFromSurface(renderer, input_surf);

        SDL_RenderCopy(renderer, title_text, NULL, &title_rect);
        SDL_RenderCopy(renderer, input_text, NULL, &input_rect);

        SDL_RenderPresent(renderer);
        while(play)  {

            SDL_WaitEvent(&evt); 
            switch(evt.type) {
                case SDL_QUIT:
                    *menu = QUIT;
                    play = SDL_FALSE;
                break;
                case SDL_WINDOWEVENT:
                    if (evt.window.event == SDL_WINDOWEVENT_RESIZED) {
                        SDL_GetWindowSize(window, &w, &h);        
                        title_rect.w = w*0.95;
                        title_rect.h = h/7;
                        title_rect.x = w/2-title_rect.w/2;
                        title_rect.y = h*0.1;
                        input_rect.h = h/7;
                        input_rect.y = h*0.35;
                    }
                break;
               /*case SDL_DROPFILE: not working as i want
                break;*/
                case SDL_KEYUP:
                    if (evt.key.keysym.sym == SDLK_BACKSPACE) {
                        if (strlen(image_input)) image_input[--i] = '\0';
                    else play = SDL_FALSE;
                    }
                    else if (evt.key.keysym.sym == SDLK_RETURN) {
                        if (is_valid(lower(image_input), 1)) {
                            if (is_PNG_or_JPG(image_input)) {
                                system("open data/images");
                                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Message", "Veuillez déposer l'image dans ce dossier(data/images/).\n\nCliquez sor OK une fois que c'est fait.", window);
                                if (is_in_dir(image_input, "images")) {
                                    isOK = 1;
                                    window_popup(renderer, window, "Image trouvee.", black, silver, 1200);
                                }
                                else {
                                    isOK = 0;
                                    window_popup(renderer, window, "Image non trouvee. Recommencez", black, silver, 1200);
                                }

                                if (isOK) {
                                    add_trans_window(renderer, window, NULL, language_array[0], word_input, sizeof(word_input));

                                    if (is_in_array(*french_array, *french_array_size, word_input)) {
                                        window_popup(renderer, window, "Mot deja present.", black, silver, 1500);
                                        //Reset
                                        strcpy(word_input, "");
                                    }
                                    else {
                                        /*==== Only alloc memory if the word is new and valid ====*/

                                        //Realloc french array
                                        (*french_array_size)++;
                                        *french_array =(char**)realloc(*french_array, (*french_array_size)*sizeof(char*));
                                        (*french_array)[(*french_array_size)-1] = calloc(28, sizeof(char));

                                        //Alloc new AVL cells
                                        new = (AVL_Image*)malloc(sizeof(AVL_Image));
                                        new->left = NULL;
                                        new->right = NULL;
                                        for (j=0; j<lang_array_size; ++j) {                                                
                                            new_trans[j] =(AVL_Trans*)malloc(sizeof(AVL_Trans));
                                            new_trans[j]->left = NULL;
                                            new_trans[j]->right = NULL;
                                        }

                                        strcpy((*french_array)[(*french_array_size)-1],word_input);
                                        sprintf(new->image, "data/images/%s", image_input);
                                        strcpy(new->french, word_input);
                                        strcpy(new_trans[0]->language, language_array[0]);
                                        strcpy(new_trans[0]->word, word_input);
                                        writerow_CSV(new->image, 0);
                                        writerow_CSV(new_trans[0]->word, (lang_array_size == 1) ? 1 : 0);

                                        //Add all translations and write it in CSV
                                        for (j=1; j<lang_array_size; ++j) {
                                            //Reset word_input
                                            strcpy(word_input, "");

                                            add_trans_window(renderer, window, new->french, language_array[j], word_input, sizeof(word_input));
                                            strcpy(new_trans[j]->language, language_array[j]);
                                            strcpy(new_trans[j]->word, word_input);
                                            writerow_CSV(new_trans[j]->word, (j == lang_array_size-1) ? 1 : 0);
                                        }

                                        //Setup AVL trans
                                        for(i=0; i<lang_array_size; ++i) {
                                            new_avl_trans = add_AVLt(new_avl_trans, new_trans[i]);
                                        }
                                        //AVL_Image is now pointing to its translations
                                        new->translation = new_avl_trans;
                                        //Add new to base AVL
                                        *image_avl = add_AVLi(*image_avl, new);
                                        //Sort the array
                                        qsort(*french_array, *french_array_size, sizeof(char*), compare);
                                        //Stop add image menu
                                        play = SDL_FALSE;
                                    }
                                }          
                            }
                            else {
                                window_popup(renderer, window, "Erreur : extension ou nom.", black, silver, 1200);
                            }
                        }
                    else {
                        window_popup(renderer, window, "Nom d'image invalide", black, silver, 1200);
                    }
                    //Reset
                    strcpy(image_input, "");
                    i = 0;
                }
                break;
                case SDL_TEXTINPUT:
                    if (i<37) {
                        strcat(image_input, evt.text.text);
                        i++;
                    }
                break;
            }
            if ((evt.type == SDL_KEYUP || evt.type == SDL_TEXTINPUT || evt.window.event == SDL_WINDOWEVENT_RESIZED)&& play) {
                SDL_RenderClear(renderer);
                if (strlen(image_input)) {
                    input_surf = TTF_RenderText_Solid(font, image_input, white);
                    input_rect.w = strlen(image_input)*(w/48);
                }
                else {
                    input_surf = TTF_RenderText_Solid(font, no_input, white);
                    input_rect.w = w/3;
                }
                input_rect.x = w/2-input_rect.w/2;
                input_text = SDL_CreateTextureFromSurface(renderer, input_surf);

                SDL_RenderCopy(renderer, title_text, NULL, &title_rect);
                SDL_RenderCopy(renderer, input_text, NULL, &input_rect);

                SDL_RenderPresent(renderer);
            }
        }
        SDL_RenderClear(renderer);
    }

    SDL_FreeSurface(title_surf);
    SDL_FreeSurface(input_surf);
    SDL_DestroyTexture(title_text);
    SDL_DestroyTexture(input_text);
    TTF_CloseFont(font);
}

/*========= Add Language ==========*/

void add_language_menu(SDL_Renderer *renderer, SDL_Window *window, AVL_Image *image_avl, char **french_array, int french_array_size, char ***language_array, int *lang_array_size, MenuState *menu)
{
    TTF_Font *font = NULL;
    SDL_Rect title_rect, input_rect;
    SDL_Surface *title_surf = NULL, *input_surf = NULL;
    SDL_Texture *title_text = NULL, *input_text = NULL;
    SDL_bool play = SDL_TRUE;
    SDL_Event evt;
    AVL_Trans *tabt[french_array_size];
    AVL_Image *tempi = NULL;
    int w, h, i=0, j, isOK;
    char 
        *title = "Entrez la nouvelle langue (en francais) :",
        lang_input[30] = "",
        word_input[46] ="",
        *no_input = "Entrez le nom de l'image...",
        *myfont = "data/fonts/retro.ttf",
        *temp_lang[*lang_array_size],
        flag_image[50],
        message_box[200]
    ;

    SDL_GetWindowSize(window, &w, &h);

    //temp_lang = *language_array but sorted
    for (int i=0; i<*lang_array_size; ++i) {
        temp_lang[i] = (*language_array)[i];
    }
    qsort(temp_lang, *lang_array_size, sizeof(char*), compare);

    title_rect.w = w*0.95;
    title_rect.h = h/7;
    title_rect.x = w/2-title_rect.w/2;
    title_rect.y = h*0.1;

    input_rect.w = w/3;
    input_rect.h = h/7;
    input_rect.x = w/2-input_rect.w/2;
    input_rect.y = h*0.35;

    font = TTF_OpenFont(myfont, 50);

    if (font) {
        SDL_RenderClear(renderer);
        title_surf = TTF_RenderText_Solid(font, title, white);
        input_surf = TTF_RenderText_Solid(font, no_input, white);

        title_text = SDL_CreateTextureFromSurface(renderer, title_surf);
        input_text = SDL_CreateTextureFromSurface(renderer, input_surf);

        SDL_RenderCopy(renderer, title_text, NULL, &title_rect);
        SDL_RenderCopy(renderer, input_text, NULL, &input_rect);

        SDL_RenderPresent(renderer);

        while (play) {
            SDL_WaitEvent(&evt);
            switch(evt.type) {
                case SDL_QUIT:
                    *menu = QUIT;
                    play = SDL_FALSE;
                break;
                case SDL_WINDOWEVENT:
                    if (evt.window.event == SDL_WINDOWEVENT_RESIZED) {
                        SDL_GetWindowSize(window, &w, &h);
                        title_rect.w = w*0.95;
                        title_rect.h = h/7;
                        title_rect.x = w/2-title_rect.w/2;
                        title_rect.y = h*0.1;
                        input_rect.h = h/7;
                        input_rect.y = h*0.35;
                    }
                break;
                case SDL_KEYUP:
                    if (evt.key.keysym.sym == SDLK_BACKSPACE) {
                        if (strlen(lang_input)) lang_input[--i] = '\0';
                        else play = SDL_FALSE;
                    }
                    if (evt.key.keysym.sym == SDLK_RETURN) {
                        if (is_valid(lower(lang_input), 0)) {
                            if (is_in_array(temp_lang, *lang_array_size, lang_input)) {
                                window_popup(renderer, window, "Cette langue est deja presente.", black, silver, 1200);
                                i=0;
                                strcpy(lang_input, "");
                            }
                            else {
                                sprintf(message_box, "Veuillez mettre l'image \"%s.png\" dans ce dossier(data/flags).\n\nCliquez sur OK qand c'est fait.", lang_input);
                                sprintf(flag_image, "%s.png", lang_input);
                                printf("%s\n", flag_image);
                                system("open data/flags");
                                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Message", message_box, window);
                                if (is_in_dir(flag_image, "flags")) {
                                    isOK = 1;
                                }
                                else {
                                    isOK = 0;
                                    window_popup(renderer, window, "Image non trouvee.", black, silver, 1200);
                                }
                                if (isOK) {
                                    /*==== Alloc new memory ====*/
                                     //language array
                                    (*lang_array_size)++;
                                    *language_array = (char**)realloc(*language_array, (*lang_array_size)*sizeof(char*));
                                    (*language_array)[(*lang_array_size)-1] = (char*)calloc(28, sizeof(char));
                                    //AVL_Trans
                                    for (j=0; j<french_array_size; ++j) {
                                        tabt[j] = (AVL_Trans*)malloc(sizeof(AVL_Trans));
                                        tabt[j]->left = NULL;
                                        tabt[j]->right = NULL;
                                        //Copy the language in all cells
                                        strcpy(tabt[j]->language, lang_input);
                                    }
                                    //Add the new translation to the language array
                                    strcpy((*language_array)[(*lang_array_size)-1], lang_input);

                                    /*==== Add all new translations ====*/
                                    for (j=0; j<french_array_size; ++j) {
                                        //Reset
                                        strcpy(word_input, "");

                                        tempi = find_AVLi(image_avl, french_array[j]);
                                        add_trans_window(renderer, window, french_array[j], lang_input, word_input, sizeof(word_input));
                                        //Copy the input
                                        strcpy(tabt[j]->word, word_input);
                                        //Add it in the AVL_Trans
                                        tempi->translation = add_AVLt(tempi->translation, tabt[j]);
                                    }

                                    //Write in CSV
                                    writeFrenchArray_CSV(image_avl, french_array, french_array_size, *language_array, *lang_array_size);

                                    play = SDL_FALSE;
                                }
                            }
                        }
                    }
                break;
                case SDL_TEXTINPUT:
                    if (i<29) {
                        strcat(lang_input, evt.text.text);
                        i++;
                    }
                break;
            }
            if (evt.type == SDL_KEYUP || evt.type == SDL_TEXTINPUT || evt.window.event == SDL_WINDOWEVENT_RESIZED) {
                SDL_RenderClear(renderer);
                if (strlen(lang_input)) {
                    input_surf = TTF_RenderText_Solid(font, lang_input, white);
                    input_rect.w = strlen(lang_input)*(w/48);
                }
                else {
                    input_surf = TTF_RenderText_Solid(font, no_input, white);
                    input_rect.w = w/3;
                }
                input_rect.x = w/2-input_rect.w/2;
                input_text = SDL_CreateTextureFromSurface(renderer, input_surf);

                SDL_RenderCopy(renderer, title_text, NULL, &title_rect);
                SDL_RenderCopy(renderer, input_text, NULL, &input_rect);

                SDL_RenderPresent(renderer);
            }
        }
        SDL_RenderClear(renderer);
    }
    SDL_FreeSurface(title_surf);
    SDL_FreeSurface(input_surf);
    SDL_DestroyTexture(title_text);
    SDL_DestroyTexture(input_text);
    TTF_CloseFont(font);
} 
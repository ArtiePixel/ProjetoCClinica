#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_rect.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define MAX_SYMBOLS 1000

typedef enum {
    SYMBOL_NONE,
    SYMBOL_X,
    SYMBOL_CIRCLE,
    SYMBOL_ASTERISK,
    SYMBOL_VERTICAL_LINE,
    SYMBOL_CHECK
} SymbolType;

typedef struct {
    SymbolType type;
    int x, y;
}tSymbol;

static int Symbol_cont = 0;

void draw_circle(SDL_Renderer* renderer, int cx, int cy, int radius) {
    for (int w = -radius; w <= radius; w++) {
        for (int h = -radius; h <= radius; h++) {
            if ((w * w + h * h) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, cx + w, cy + h);
            }
        }
    }
}

void draw_symbol(SDL_Renderer* renderer, SymbolType symbol, int x, int y) {
    switch (symbol) {
        case SYMBOL_X:
            SDL_RenderDrawLine(renderer, x - 10, y - 10, x + 10, y + 10);
            SDL_RenderDrawLine(renderer, x + 10, y - 10, x - 10, y + 10);
            break;
        case SYMBOL_CIRCLE:
            draw_circle(renderer, x, y, 5);
            break;
        case SYMBOL_ASTERISK:
            SDL_RenderDrawLine(renderer, x - 10, y, x + 10, y);       // horizontal
            SDL_RenderDrawLine(renderer, x, y - 10, x, y + 10);       // vertical
            SDL_RenderDrawLine(renderer, x - 7, y - 7, x + 7, y + 7); // diagonal
            SDL_RenderDrawLine(renderer, x - 7, y + 7, x + 7, y - 7); // diagonal
            break;
        case SYMBOL_VERTICAL_LINE:
            SDL_RenderDrawLine(renderer, x, y - 10, x, y + 10);
            break;
        case SYMBOL_CHECK:
            SDL_RenderDrawLine(renderer, x - 8, y, x - 2, y + 8);

            SDL_RenderDrawLine(renderer, x - 2, y + 8, x + 10, y - 4);
            break;
        default:
            break;
    }
}

void save_symbol(int *Symbol_cont, tSymbol symbols[], SymbolType currentSymbol, int x, int y){
    if(*Symbol_cont < MAX_SYMBOLS)
    {
        symbols[*Symbol_cont].type = currentSymbol;
        symbols[*Symbol_cont].x = x;
        symbols[*Symbol_cont].y = y;
        (*Symbol_cont)++;
    }

    else
    printf("Numero maximo de simbolos atingido!!!");
}

void draw_window(tSymbol symbols[], SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* sizewin, int Symbol_cont){
    //Coloca o fundo
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, sizewin);

    //Cor para as simbolos
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for(int i = 0; i < Symbol_cont; i++)
    {
        draw_symbol(renderer, symbols[i].type, symbols[i].x, symbols[i].y);
    }
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    //Array para salvar os simbolos
    tSymbol symbols[MAX_SYMBOLS];

    //Inicia a janela
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Desenhar Símbolos SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //Cria a textura de fundo a partir da imagem bitmap
    SDL_Surface* image = SDL_LoadBMP("FUNDO.bmp");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

    SDL_Rect sizewin;
    sizewin.h = WINDOW_HEIGHT;
    sizewin.w = WINDOW_WIDTH;
    sizewin.x = 0;
    sizewin.y = 0;

    bool running = true;
    SDL_Event event;
    SymbolType currentSymbol = SYMBOL_NONE;

    // Aplica o fundo
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // branco opcional para o fundo
    SDL_RenderClear(renderer); // limpa antes de desenhar
    SDL_RenderCopy(renderer, texture, NULL, &sizewin); // aplica textura
    SDL_RenderPresent(renderer);

    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_1:
                            currentSymbol = SYMBOL_X;
                            break;
                        case SDLK_2:
                            currentSymbol = SYMBOL_CIRCLE;
                            break;
                        case SDLK_3:
                            currentSymbol = SYMBOL_ASTERISK;
                            break;
                        case SDLK_4:
                            currentSymbol = SYMBOL_VERTICAL_LINE;
                            break;
                        case SDLK_5:
                            currentSymbol = SYMBOL_CHECK;
                            break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT && currentSymbol != SYMBOL_NONE) {
                        int x = event.button.x;
                        int y = event.button.y;

                        //Salva os simbolos no array
                        save_symbol(&Symbol_cont, symbols, currentSymbol, x, y);
                        //redesenha a janela
                        draw_window(symbols, renderer, texture, &sizewin, Symbol_cont);
                    }
                    break;
            }
        }
    }
    //Salva em um arquivo PNG
    SDL_Surface *screenshot = SDL_CreateRGBSurface(0, WINDOW_WIDTH, WINDOW_HEIGHT, 32, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA32, screenshot->pixels, screenshot->pitch);
    IMG_SavePNG(screenshot, "foto.png");

    //Finaliza depois de fechar a janela
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

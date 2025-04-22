#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define MAX_SYMBOLS 1000

// contador de simbolos
static int Symbol_cont = 0;

//contador de background
static int currentWindow = 1;

typedef enum {
    SYMBOL_NONE,
    SYMBOL_X,
    SYMBOL_CIRCLE,
    SYMBOL_ASTERISK,
    SYMBOL_VERTICAL_LINE,
    SYMBOL_CHECK,
    SYMBOL_ERASE,
} SymbolType;

typedef struct {
    SymbolType type;
    int x, y;
}tSymbol;

//Array para salvar os simbolos (se for adicionar mais backgrounds mudar o [número]
    tSymbol symbols[3][MAX_SYMBOLS];

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
        case SYMBOL_ERASE:
            //como não existe nada, na hora de redesenhar ele sobrescreve essa região pelo background
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
    //Seta direita
    SDL_Rect sizewin2;
    sizewin2.h = 50;
    sizewin2.w = 50;
    sizewin2.x = 1150;
    sizewin2.y = 300;

    SDL_Surface* imagemSurface = IMG_Load("seta.png");
    SDL_Texture* imagemTextura = SDL_CreateTextureFromSurface(renderer, imagemSurface);
    SDL_RenderCopy(renderer, imagemTextura, NULL, &sizewin2);

    SDL_RenderCopy(renderer, imagemTextura, NULL, &sizewin2);
    SDL_RenderPresent(renderer);

    //Seta esquerda
    SDL_Rect sizewin3;
    sizewin3.h = 50;
    sizewin3.w = 50;
    sizewin3.x = 0;
    sizewin3.y = 300;

    SDL_Surface* imagemSurface2 = IMG_Load("seta.png");
    SDL_Texture* imagemTextura2 = SDL_CreateTextureFromSurface(renderer, imagemSurface2);

    SDL_RenderCopyEx(renderer, imagemTextura2, NULL, &sizewin3,360.0,NULL, SDL_FLIP_HORIZONTAL);
    SDL_RenderPresent(renderer);
}

double distancia_Dois_Pontos(int xc, int yc, int x, int y) {
    double raiz = pow(xc - x, 2) + pow(yc - y, 2);
    double distancia = sqrt(raiz);
    return distancia;
}

SDL_Texture* background_changer(SDL_Renderer* renderer, char* windowN) {

        SDL_Surface* image = SDL_LoadBMP(windowN);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);

        return texture;
    }

int main(int argc, char* argv[]) {

    //Inicia a janela
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Desenhar Símbolos SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //Cria a textura de fundo a partir da imagem bitmap
    SDL_Surface* image = SDL_LoadBMP("1");
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

    // Desenha as setas
    draw_window(symbols[1], renderer, texture, &sizewin, Symbol_cont);

    // Escolher qual imagem de fundo aparecerá
    char windowN[2];
    strcpy(windowN, "1"); // Default

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
                        case SDLK_6:
                            currentSymbol = SYMBOL_ERASE;
                            break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        int x = event.button.x;
                        int y = event.button.y;


                        //Detecta quando clica na seta direita
                        if (distancia_Dois_Pontos(1150,320,x,y) < 40) {

                            currentWindow++;
                            if (currentWindow > 3) currentWindow = 3; // Evita passar para um background inexistente
                            if (currentWindow == 1) strcpy(windowN, "1");
                            if (currentWindow == 2) strcpy(windowN, "2");
                            if (currentWindow == 3) strcpy(windowN, "3");

                            draw_window(symbols[currentWindow], renderer, background_changer(renderer,windowN), &sizewin, Symbol_cont);
                            continue;
                        }


                        //Detecta quando clica na seta esquerda
                        if (distancia_Dois_Pontos(50,320,x,y) < 40) {

                            currentWindow--;
                            if (currentWindow < 1) currentWindow = 1; // Evita voltar para um background inexistente
                            if (currentWindow == 1) strcpy(windowN, "1");
                            if (currentWindow == 2) strcpy(windowN, "2");
                            if (currentWindow == 3) strcpy(windowN, "3");


                            draw_window(symbols[currentWindow], renderer, background_changer(renderer,windowN), &sizewin, Symbol_cont);
                            continue;
                        }
                        //Transforma a forma em uma do tipo Erase
                        if (currentSymbol == SYMBOL_ERASE)
                        {
                            for (int i = Symbol_cont - 1; i >= 0; i--)
                            {
                                if (symbols[currentWindow][i].type == SYMBOL_ERASE) {continue;}

                                if (distancia_Dois_Pontos(symbols[currentWindow][i].x, symbols[currentWindow][i].y, x, y) < 20)
                                {
                                    symbols[currentWindow][i].type = SYMBOL_ERASE;
                                    break;
                                }
                            }
                        } else {
                            //Salva os simbolos no array
                            save_symbol(&Symbol_cont, symbols[currentWindow], currentSymbol, x, y);
                        }
                        draw_window(symbols[currentWindow], renderer, background_changer(renderer,windowN), &sizewin, Symbol_cont);
                    }
                    break;
            }
        }
    }
    //Salva em um arquivo PNG
    SDL_Surface *screenshot = SDL_CreateRGBSurface(0, WINDOW_WIDTH, WINDOW_HEIGHT, 32, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA32, screenshot->pixels, screenshot->pitch);
    //IMG_SavePNG(screenshot, "foto.png");

    //Finaliza depois de fechar a janela
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

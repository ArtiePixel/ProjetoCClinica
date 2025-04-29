#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <stdbool.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900
#define MAX_SYMBOLS 1000
#define WINDOWS_NUM 8

//Soma das doses de cada simbolo
unsigned int aplication_sum[13] = {0};
unsigned int aplication_total = 0;

// contador de simbolos
static int Symbol_cont[WINDOWS_NUM] = {0};

//contador de background
static int currentWindow = 0;

//ocultar icones
bool show = true;

typedef enum {
    SYMBOL_NONE = -1,
    SYMBOL_VERTICAL_LINE,
    SYMBOL_X,
    SYMBOL_ASTERISK,
    SYMBOL_PLUS,
    SYMBOL_HASHTAG,
    SYMBOL_CIRCLE,
    SYMBOL_UNFILLEDCIRCLE,
    SYMBOL_DOLLAR,
    SYMBOL_CHECK,
    SYMBOL_TRIANGLE,
    SYMBOL_FILLEDTRIANGLE,
    SYMBOL_SQUARE,
    SYMBOL_FILLEDSQUARE,
    SYMBOL_ERASE,
} SymbolType;

typedef struct {
    SymbolType type;
    int x, y;
}tSymbol;

//Array para salvar os simbolos (se for adicionar mais backgrounds mudar o [número]
    tSymbol symbols[WINDOWS_NUM][MAX_SYMBOLS];

void draw_circle(SDL_Renderer* renderer, int cx, int cy, int radius, bool filled){
    if (filled){ // Circulo fechado
        for (int w = -radius; w <= radius; w++) {
            for (int h = -radius; h <= radius; h++) {
                if ((w * w + h * h) <= (radius * radius)) {
                    SDL_RenderDrawPoint(renderer, cx + w, cy + h);
                }
            }
        }
    } else { // Circulo aberto
        for (float angle = 0; angle < 360; angle += 10) { // passos de 10 graus
            float rad = (angle * 3.14159265358979323846) / 180.0f; // converte para radianos
            int x = cx + (int)(cos(rad) * radius);
            int y = cy + (int)(sin(rad) * radius);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}

void draw_triangle(SDL_Renderer* renderer,int x, int y, bool filled) {
    // Define o tamanho do triângulo
    int size = 7;
    // 3 vértices em volta do mouse
    int x1 = x;
    int y1 = y - size;// topo

    int x2 = x - size;
    int y2 = y + size;

    int x3 = x + size;
    int y3 = y + size;

    if (!filled) {
        // Desenha o triângulo vazio
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        SDL_RenderDrawLine(renderer, x2, y2, x3, y3);
        SDL_RenderDrawLine(renderer, x3, y3, x1, y1);
    } else {
        int minY = y1;
        int maxY = y2 > y3 ? y2 : y3;

        for (int y = minY; y <= maxY; y++) {
            float t1 = (y - y1) / (float)(y2 - y1);
            float t2 = (y - y1) / (float)(y3 - y1);

            int startX = x1 + (x2 - x1) * t1;
            int endX   = x1 + (x3 - x1) * t2;

            if (startX > endX) {
                int temp = startX;
                startX = endX;
                endX = temp;
            }
            SDL_RenderDrawLine(renderer, startX, y, endX, y);
        }
    }
}

void draw_square(SDL_Renderer* renderer, int x, int y, int size,  bool filled) {

    int x1 = x - size;
    int y1 = y - size;
    int x2 = x + size;
    int y2 = y + size;

    if (!filled){
        SDL_RenderDrawLine(renderer, x1, y1, x2, y1); // topo
        SDL_RenderDrawLine(renderer, x2, y1, x2, y2); // direita
        SDL_RenderDrawLine(renderer, x2, y2, x1, y2); // base
        SDL_RenderDrawLine(renderer, x1, y2, x1, y1); // esquerda
    } else {
        for (int y = y1; y <= y2; y++) {
            SDL_RenderDrawLine(renderer, x1, y, x2, y);
        }
    }
}

void draw_dollar(SDL_Renderer* renderer, int x, int y, int size){
    int w = size * 2;
    int h = size * 4;

    int top = y - h / 2;
    int bottom = y + h / 2;

    // Parte do S
    // Linha horizontal no topo do S
    SDL_RenderDrawLine(renderer, x - w/2, top + size, x + w/2, top + size);

    // Lado esquerdo descendo até meio
    SDL_RenderDrawLine(renderer, x - w/2, top + size, x - w/2, y);

    // Meio virando para lado direito
    SDL_RenderDrawLine(renderer, x - w/2, y, x + w/2, y);

    // Lado direito descendo até embaixo
    SDL_RenderDrawLine(renderer, x + w/2, y, x + w/2, bottom - size);

    // Linha de base (horizontal) fechando o S
    SDL_RenderDrawLine(renderer, x + w/2, bottom - size, x - w/2, bottom - size);

    // Linha vertical do cifrão (em cima do S)
    SDL_RenderDrawLine(renderer, x, top - size/5, x, top + size);

    // Linha vertical do cifrão (embaixo do S)
    SDL_RenderDrawLine(renderer, x, bottom - size, x, bottom + size/5);
}

void draw_symbol(SDL_Renderer* renderer, SymbolType symbol, int x, int y) {
    switch (symbol) {
        case SYMBOL_X:
            SDL_RenderDrawLine(renderer, x - 10, y - 10, x + 10, y + 10);
            SDL_RenderDrawLine(renderer, x + 10, y - 10, x - 10, y + 10);
            break;
        case SYMBOL_CIRCLE:
            draw_circle(renderer, x, y, 7, true);
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
        case SYMBOL_PLUS:
            SDL_RenderDrawLine(renderer, x - 10, y, x + 10, y);       // linha horizontal
            SDL_RenderDrawLine(renderer, x, y - 10, x, y + 10);       // linha vertical
            break;
        case SYMBOL_HASHTAG:
            // Linhas horizontais
            SDL_RenderDrawLine(renderer, x - 10, y - 5, x + 10, y - 5);
            SDL_RenderDrawLine(renderer, x - 10, y + 5, x + 10, y + 5);

            // Linhas verticais
            SDL_RenderDrawLine(renderer, x - 5, y - 10, x - 5, y + 10);
            SDL_RenderDrawLine(renderer, x + 5, y - 10, x + 5, y + 10);
            break;
        case SYMBOL_UNFILLEDCIRCLE:
            draw_circle(renderer, x, y, 7, false);
            break;
        case SYMBOL_TRIANGLE:
            draw_triangle(renderer,x,y,true);
            break;
        case SYMBOL_FILLEDTRIANGLE:
            draw_triangle(renderer,x,y,false);
            break;
        case SYMBOL_SQUARE:
            draw_square(renderer,x,y,7,true);
            break;
        case SYMBOL_FILLEDSQUARE:
            draw_square(renderer,x,y,7,false);
            break;
        case SYMBOL_DOLLAR:
            draw_dollar(renderer, x, y, 7);
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

void draw_window(tSymbol symbols[], SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* sizewin, int Symbol_cont, bool show){
    //Coloca o fundo
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, sizewin);

    //Cor para os simbolos
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
    for(int i = 0; i < Symbol_cont; i++)
    {
        draw_symbol(renderer, symbols[i].type, symbols[i].x, symbols[i].y);
    }

    if(show){
    //Seta direita
    SDL_Rect sizewin2;
    sizewin2.h = 50;
    sizewin2.w = 50;
    sizewin2.x = (WINDOW_WIDTH - 50);
    sizewin2.y = (WINDOW_HEIGHT/2) - 100;

    SDL_Surface* imagemSurface = IMG_Load("icons/seta.png");
    SDL_Texture* imagemTextura = SDL_CreateTextureFromSurface(renderer, imagemSurface);

    SDL_RenderCopy(renderer, imagemTextura, NULL, &sizewin2);
    SDL_FreeSurface(imagemSurface);
    SDL_DestroyTexture(imagemTextura);

    //Seta esquerda
    SDL_Rect sizewin3;
    sizewin3.h = 50;
    sizewin3.w = 50;
    sizewin3.x = 0;
    sizewin3.y = (WINDOW_HEIGHT/2) - 100;

    SDL_Surface* imagemSurface2 = IMG_Load("icons/seta.png");
    SDL_Texture* imagemTextura2 = SDL_CreateTextureFromSurface(renderer, imagemSurface2);

    SDL_RenderCopyEx(renderer, imagemTextura2, NULL, &sizewin3,360.0,NULL, SDL_FLIP_HORIZONTAL);
    SDL_FreeSurface(imagemSurface2);
    SDL_DestroyTexture(imagemTextura2);

    //Botao de salvar imagem
    SDL_Rect sizewin4;
    sizewin4.h = 60;
    sizewin4.w = 60;
    sizewin4.x = 5;
    sizewin4.y = 5;

    SDL_Surface* imagemSurface3 = IMG_Load("icons/save_icon.png");
    SDL_Texture* imagemTextura3 = SDL_CreateTextureFromSurface(renderer, imagemSurface3);

    SDL_RenderCopy(renderer, imagemTextura3, NULL, &sizewin4);
    SDL_FreeSurface(imagemSurface3);
    SDL_DestroyTexture(imagemTextura3);
    }

    //LEGENDA
    SDL_Rect sizewin5;
    sizewin5.h = 92;
    sizewin5.w = 1200;
    sizewin5.x = -10;
    sizewin5.y = WINDOW_HEIGHT - 75;

    SDL_Surface* imagemSurface4 = IMG_Load("icons/legendav2.png");
    SDL_Texture* imagemTextura4 = SDL_CreateTextureFromSurface(renderer, imagemSurface4);

    SDL_RenderCopy(renderer, imagemTextura4, NULL, &sizewin5);
    SDL_FreeSurface(imagemSurface4);
    SDL_DestroyTexture(imagemTextura4);

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
    SDL_FreeSurface(image);

    return texture;
}

void sum(){
    for(int i = 0; i < 13; i++)
        aplication_sum[i] = 0;

    //não mexer, necessário
    aplication_sum[0] -= 1;
    aplication_total = -1;
    for(int i = 0; i <= Symbol_cont[currentWindow]; i++)
    {
        if(symbols[currentWindow][i].type == SYMBOL_ERASE)
        continue;

        else
        aplication_sum[symbols[currentWindow][i].type] += 1;
    }

    for(int j = 0; j <= Symbol_cont[currentWindow]; j++)
    {
        if(symbols[currentWindow][j].type == SYMBOL_ERASE)
        continue;

        else if(symbols[currentWindow][j].type < 9)
        aplication_total += symbols[currentWindow][j].type + 1;

        else if(symbols[currentWindow][j].type == SYMBOL_FILLEDTRIANGLE)
        aplication_total += 15;

        else if(symbols[currentWindow][j].type == SYMBOL_SQUARE)
        aplication_total += 20;

        else if(symbols[currentWindow][j].type == SYMBOL_FILLEDSQUARE)
        aplication_total += 40;
    }
    return;
}

void save_image(SDL_Renderer* renderer, int currentWindow){
    //faz os calculos da quantidade de simbolos divididos por indicie e dose total
    sum();

    //Nome do arquivo
    char name_save[10];

    //Salva em um arquivo PNG
    SDL_Surface *screenshot = SDL_CreateRGBSurface(0, WINDOW_WIDTH, WINDOW_HEIGHT, 32, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA32, screenshot->pixels, screenshot->pitch);
    IMG_SavePNG(screenshot, "foto.png");

    //Altera o nome do arquivo
    snprintf(name_save, sizeof(name_save), "foto%d.png", currentWindow);
    rename("foto.png", name_save);

}

//Muda o simbolo do cursor para o que vai ser desenhado
void change_cursor(int type){
    if(type == 14) {
        SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
        SDL_SetCursor(cursor);
    } else {
        char image[20];
        SDL_Surface* surfacemouse;

        snprintf(image,sizeof(image),"mouseimages/%d.png",type);
        surfacemouse = IMG_Load(image);

        SDL_Cursor* cursor = SDL_CreateColorCursor(surfacemouse, 12, 12); // 12 é para sincronizar a imagem do simbolo com onde vai ser desenhado
        SDL_SetCursor(cursor);
        SDL_FreeSurface(surfacemouse);
    }
}

int main(int argc, char* argv[]) {

    //Inicia a janela
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Desenhar Símbolos SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //Cria a textura de fundo a partir da imagem bitmap
    SDL_Surface* image = SDL_LoadBMP("background/1");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

    SDL_Rect sizewin;
    sizewin.h = WINDOW_HEIGHT - 100;
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
    draw_window(symbols[1], renderer, texture, &sizewin, Symbol_cont[currentWindow], show);

    // Escolher qual imagem de fundo aparecerá
    char windowN[200];
    strcpy(windowN, "background/1"); // Default

    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_1:
                            currentSymbol = SYMBOL_VERTICAL_LINE;
                            change_cursor(1);
                            break;
                        case SDLK_2:
                            currentSymbol = SYMBOL_X;
                            change_cursor(2);
                            break;
                        case SDLK_3:
                            currentSymbol = SYMBOL_ASTERISK;
                            change_cursor(3);
                            break;
                        case SDLK_4:
                            currentSymbol = SYMBOL_PLUS;
                            change_cursor(4);
                            break;
                        case SDLK_5:
                            currentSymbol = SYMBOL_HASHTAG;
                            change_cursor(5);
                            break;
                        case SDLK_6:
                            currentSymbol = SYMBOL_CIRCLE;
                            change_cursor(6);
                            break;
                        case SDLK_7:
                            currentSymbol = SYMBOL_UNFILLEDCIRCLE;
                            change_cursor(7);
                            break;
                        case SDLK_8:
                            currentSymbol = SYMBOL_DOLLAR;
                            change_cursor(8);
                            break;
                        case SDLK_9:
                            currentSymbol = SYMBOL_CHECK;
                            change_cursor(9);
                            break;
                        case SDLK_0:
                            currentSymbol = SYMBOL_TRIANGLE;
                            change_cursor(11);
                            break;
                        case SDLK_q:
                            currentSymbol = SYMBOL_FILLEDTRIANGLE;
                            change_cursor(10);
                            break;
                        case SDLK_w:
                            currentSymbol = SYMBOL_SQUARE;
                            change_cursor(13);
                            break;
                        case SDLK_e:
                            currentSymbol = SYMBOL_FILLEDSQUARE;
                            change_cursor(12);
                            break;
                        case SDLK_BACKSPACE:
                            currentSymbol = SYMBOL_ERASE;
                            change_cursor(14);
                            break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        int x = event.button.x;
                        int y = event.button.y;


                        //Detecta quando clica na seta direita
                        if (distancia_Dois_Pontos((WINDOW_WIDTH - 30),370,x,y) < 40) {

                            currentWindow++;
                            if (currentWindow > 7) currentWindow = 7; // Evita passar para um background inexistente
                            snprintf(windowN, sizeof(windowN), "background/%d", currentWindow + 1);
                            draw_window(symbols[currentWindow], renderer, background_changer(renderer,windowN), &sizewin, Symbol_cont[currentWindow], show);
                            continue;
                        }


                        //Detecta quando clica na seta esquerda
                        if (distancia_Dois_Pontos(30,370,x,y) < 40) {
                            currentWindow--;
                            if (currentWindow < 0) currentWindow = 0; // Evita voltar para um background inexistente
                            snprintf(windowN, sizeof(windowN), "background/%d", currentWindow + 1);

                            draw_window(symbols[currentWindow], renderer, background_changer(renderer,windowN), &sizewin, Symbol_cont[currentWindow], show);
                            continue;
                        }

                        //Detecta clique no botão de salvar
                        if(distancia_Dois_Pontos(20, 20, x, y) < 50){
                            show = false;
                            draw_window(symbols[currentWindow], renderer, background_changer(renderer, windowN), &sizewin, Symbol_cont[currentWindow], show);
                            save_image(renderer, currentWindow);
                            show = true;
                            draw_window(symbols[currentWindow], renderer, background_changer(renderer, windowN), &sizewin, Symbol_cont[currentWindow], show);
                            continue;
                        }

                        if(currentSymbol == SYMBOL_NONE) continue;

                        //Transforma a forma em uma do tipo Erase
                        if (currentSymbol == SYMBOL_ERASE)
                        {
                            for (int i = Symbol_cont[currentWindow] - 1; i >= 0; i--)
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
                            save_symbol(&Symbol_cont[currentWindow], symbols[currentWindow], currentSymbol, x, y);
                        }
                        draw_window(symbols[currentWindow], renderer, background_changer(renderer,windowN), &sizewin, Symbol_cont[currentWindow], show);
                    }
                    break;
            }
        }
    }

    //Finaliza depois de fechar a janela
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

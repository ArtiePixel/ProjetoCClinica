#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef enum {
    SYMBOL_NONE,
    SYMBOL_X,
    SYMBOL_CIRCLE,
    SYMBOL_ASTERISK,
    SYMBOL_VERTICAL_LINE,
    SYMBOL_CHECK
} SymbolType;

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
            draw_circle(renderer, x, y, 10);
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
            // Novo símbolo de "check"
            // Primeiro traço: parte de baixo (para a esquerda)
            SDL_RenderDrawLine(renderer, x - 8, y, x - 2, y + 8);
            // Segundo traço: parte de cima (para a direita)
            SDL_RenderDrawLine(renderer, x - 2, y + 8, x + 10, y - 4);
            break;
        default:
            break;
    }
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Desenhar Símbolos SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;
    SDL_Event event;
    SymbolType currentSymbol = SYMBOL_NONE;

    // Fundo branco
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
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

                        // Cor preta para os símbolos
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        draw_symbol(renderer, currentSymbol, x, y);
                        SDL_RenderPresent(renderer);
                    }
                    break;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

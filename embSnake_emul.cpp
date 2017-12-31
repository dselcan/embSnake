#include <iostream>
#include <SDL.h>
#include <stdlib.h>

Uint32 * pixels;

#define FRAME_TICKS 50

//************************
//*CODE RELEVANT FOR GAME*
//************************

//Graphics and game loop code
#include "embSnake_loop.h"
#include "embSnake_draw.h"

level_t level = {0};
draw_location_t draw_location = {0};

uint8_t get_rand_color()
{
	return ((uint8_t)rand()) >> 2;
}

uint8_t get_rand_max(uint8_t max_val)
{
	return ((uint8_t)rand()) % max_val; //This is not equally distributed, but for a game it should be OK
}

uint8_t get_rand()
{
	return rand();
}

void draw_pixel(int x, int y, uint16_t pix)
{
	uint8_t r, g, b;
	Uint32 pixel;
	
	r = pix >> 11;
	r = r&0x1f;
	r = (r << 3) | (r >> 2);
	
	g = pix >> 5;
	g = g&0x3f;
	g = (g << 2) | (g >> 3);
	
	b = pix >> 0;
	b = b&0x1f;
	b = (b << 3) | (b >> 2);
	
	pixel = (r << 16) | (g << 8) | b;
	pixels[(2*y+0)*220*2 + (2*x+0)] = pixel;
	pixels[(2*y+0)*220*2 + (2*x+1)] = pixel;
	pixels[(2*y+1)*220*2 + (2*x+0)] = pixel;
	pixels[(2*y+1)*220*2 + (2*x+1)] = pixel;
}

void draw_trow(int x, int y, uint16_t* trow)
{
	uint8_t i;
	for(i = 0; i < 11; i++){
		draw_pixel(x+i, y, trow[i]);
	}
}

int main(int argc, char ** argv)
{
	int x, y;
    bool leftMouseButtonDown = false;
    bool quit = false;
    SDL_Event event;

	srand(1);

	uint32_t next_time = SDL_GetTicks();
	uint32_t current_time = SDL_GetTicks();

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window * window = SDL_CreateWindow("embSnake",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 220*2, 176*2, 0);

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture * texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 220*2, 176*2);

    pixels = new Uint32[220*2 * 176*2];
    //memset(pixels, 0, 512 * 512 * sizeof(Uint32));
	game_reset(&level);

	uint8_t ev = EV_NONE;
    while (!quit)
    {
        SDL_UpdateTexture(texture, NULL, pixels, 220*2 * sizeof(Uint32));

		memset(pixels, 0, 220*2 * 176*2 * sizeof(Uint32));
		

		while(SDL_PollEvent(&event)){
			switch (event.type)
			{
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
				case SDLK_LEFT:
						ev |= EV_LEFT;
					break;
				case SDLK_RIGHT:
						ev |= EV_RIGHT;
					break;
				}
                break;
			case SDL_QUIT:
				quit = true;
				break;
			}
		}

		game_loop(&level, &ev);
		
		reset_trow_location(&draw_location);
		
		
		for(y = 0; y < 176; y++){
			for(x = 0; x < 220; x += 11){
				uint16_t trow[11];
				get_next_trow(&draw_location, &level, trow);
				draw_trow(x, y, trow);
			}
		}

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

		//Very crude frame rate limiter, might not work
		current_time = SDL_GetTicks();
		next_time += FRAME_TICKS;
		if(current_time > next_time){
			next_time = current_time;
		}else{
			SDL_Delay( next_time - current_time );
		}
    }

    delete[] pixels;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
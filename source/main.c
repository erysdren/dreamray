/*
MIT License

Copyright (c) 2023 erysdren (it/she/they)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*
 *
 * MAIN.C
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "SDL.h"

#include "eui_sdl2.h"

#define WIDTH (640)
#define HEIGHT (480)
#define TITLE "DREAMRAY"

/* SDL2 state */
static SDL_Window *window;
static SDL_Surface *surface8;
static SDL_Surface *surface32;
static SDL_Renderer *renderer;
static SDL_Texture *texture;
static SDL_Rect rect;
static SDL_Event event;

static SDL_Surface *palette;

/* main */
int main(int argc, char **argv)
{
	uint32_t format;
	unsigned int rmask, gmask, bmask, amask;
	int bpp;
	int i;

	EUI_UNUSED(argc);
	EUI_UNUSED(argv);

	/* init */
	SDL_Init(SDL_INIT_VIDEO);

	/* create window */
	window = SDL_CreateWindow(TITLE,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WIDTH, HEIGHT,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
	);

	/* create renderer */
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);
	SDL_RenderSetIntegerScale(renderer, SDL_TRUE);
	SDL_SetWindowMinimumSize(window, WIDTH, HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

	/* create our render surface */
	surface8 = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 8, 0, 0, 0, 0);
	SDL_FillRect(surface8, NULL, 0);

	/* load palette */
	palette = SDL_LoadBMP("gfx/palette.bmp");
	if (palette == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to load \"gfx/palette.bmp\"", window);
		return 1;
	}

	/* install palette */
	SDL_SetPaletteColors(surface8->format->palette, palette->format->palette->colors, 0, 256);

	/* create display surface */
	format = SDL_GetWindowPixelFormat(window);
	SDL_PixelFormatEnumToMasks(format, &bpp, &rmask, &gmask, &bmask, &amask);
	surface32 = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, bpp, rmask, gmask, bmask, amask);
	texture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

	/* make sure relative mouse mode is disabled */
	SDL_SetRelativeMouseMode(SDL_FALSE);

	/* setup blit rect */
	rect.x = 0;
	rect.y = 0;
	rect.w = WIDTH;
	rect.h = HEIGHT;

	/* main loop */
	while (!SDL_QuitRequested())
	{
		/* clear back buffer */
		SDL_FillRect(surface8, NULL, 0x00);

		/* copy to screen */
		SDL_BlitSurface(surface8, &rect, surface32, &rect);
		SDL_UpdateTexture(texture, NULL, surface32->pixels, surface32->pitch);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	/* quit */
	SDL_FreeSurface(surface8);
	SDL_FreeSurface(surface32);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

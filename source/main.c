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

#include "SDL.h"
#include "eui.h"
#include "eui_evnt.h"
#include "eui_sdl2.h"
#include "eui_widg.h"
#include "dreamray.h"

/*
 *
 * state
 *
 */

static SDL_Window *window = NULL;
static SDL_Surface *surface8 = NULL;
static SDL_Surface *surface32 = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;
static SDL_Rect rect;
static SDL_Event event;

static SDL_Surface *palette = NULL;
static SDL_Surface *colormap = NULL;

void quit(int code)
{
	eui_quit();

	if (colormap) SDL_FreeSurface(colormap);
	if (palette) SDL_FreeSurface(palette);
	if (surface8) SDL_FreeSurface(surface8);
	if (surface32) SDL_FreeSurface(surface32);
	if (texture) SDL_DestroyTexture(texture);
	if (renderer) SDL_DestroyRenderer(renderer);
	if (window) SDL_DestroyWindow(window);
	SDL_Quit();

	exit(code);
}

void error_load(char *filename)
{
	static char message[256];
	snprintf(message, 256, "Failed to load \"%s\"", filename);
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", message, window);
	quit(1);
}

static void load_graphics(void)
{
	/* load palette */
	palette = SDL_LoadBMP("gfx/palette.bmp");
	if (palette == NULL)
		error_load("gfx/palette.bmp");

	/* load colormap */
	colormap = SDL_LoadBMP("gfx/colormap.bmp");
	if (colormap == NULL)
		error_load("gfx/colormap.bmp");
}

/* main */
int main(int argc, char **argv)
{
	unsigned int format, rmask, gmask, bmask, amask;
	int bpp, i;

	EUI_UNUSED(argc);
	EUI_UNUSED(argv);

	/* init */
	SDL_Init(SDL_INIT_VIDEO);

	/* create window */
	window = SDL_CreateWindow(DREAMRAY_TITLE,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		DREAMRAY_WIDTH, DREAMRAY_HEIGHT,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
	);

	/* create renderer */
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_RenderSetLogicalSize(renderer, DREAMRAY_WIDTH, DREAMRAY_HEIGHT);
	SDL_RenderSetIntegerScale(renderer, SDL_TRUE);
	SDL_SetWindowMinimumSize(window, DREAMRAY_WIDTH, DREAMRAY_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

	/* create our render surface */
	surface8 = SDL_CreateRGBSurface(0, DREAMRAY_WIDTH, DREAMRAY_HEIGHT, 8, 0, 0, 0, 0);
	SDL_FillRect(surface8, NULL, 0x00);

	/* load graphics */
	load_graphics();

	/* install palette */
	SDL_SetPaletteColors(surface8->format->palette, palette->format->palette->colors, 0, 256);

	/* create display surface */
	format = SDL_GetWindowPixelFormat(window);
	SDL_PixelFormatEnumToMasks(format, &bpp, &rmask, &gmask, &bmask, &amask);
	surface32 = SDL_CreateRGBSurface(0, DREAMRAY_WIDTH, DREAMRAY_HEIGHT, bpp, rmask, gmask, bmask, amask);
	texture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_STREAMING, DREAMRAY_WIDTH, DREAMRAY_HEIGHT);

	/* make sure relative mouse mode is disabled */
	SDL_SetRelativeMouseMode(SDL_FALSE);

	/* setup blit rect */
	rect.x = 0;
	rect.y = 0;
	rect.w = DREAMRAY_WIDTH;
	rect.h = DREAMRAY_HEIGHT;

	/* init eui */
	eui_init(surface8->w, surface8->h, surface8->format->BitsPerPixel, surface8->pitch, surface8->pixels);

	/* main loop */
	while (!SDL_QuitRequested())
	{
		/* push events */
		while (SDL_PollEvent(&event))
			eui_sdl2_event_push(&event);

		/* process events */
		eui_event_queue_process();

		switch (gamestate)
		{
			case STATE_CONSOLE:
				console_main();
				break;

			case STATE_MENU:
				menu_main();
				break;

			case STATE_GAME:
				game_main();
				break;
		}

		/* copy to screen */
		SDL_BlitSurface(surface8, &rect, surface32, &rect);
		SDL_UpdateTexture(texture, NULL, surface32->pixels, surface32->pitch);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	/* quit */
	quit(0);
	return 0;
}

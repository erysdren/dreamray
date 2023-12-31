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
static SDL_Surface *logo = NULL;
static SDL_Rect logo_rect;
static SDL_Surface *background = NULL;

static void button_play(void *user)
{
	EUI_UNUSED(user);
}

static void button_editor(void *user)
{
	EUI_UNUSED(user);
}

static void button_options(void *user)
{
	EUI_UNUSED(user);
}

static void button_quit(void *user)
{
	EUI_UNUSED(user);

	eui_quit();

	if (logo) SDL_FreeSurface(logo);
	if (background) SDL_FreeSurface(background);
	if (colormap) SDL_FreeSurface(colormap);
	if (palette) SDL_FreeSurface(palette);
	if (surface8) SDL_FreeSurface(surface8);
	if (surface32) SDL_FreeSurface(surface32);
	if (texture) SDL_DestroyTexture(texture);
	if (renderer) SDL_DestroyRenderer(renderer);
	if (window) SDL_DestroyWindow(window);
	SDL_Quit();

	exit(0);
}

static void load_graphics(void)
{
	/* load palette */
	palette = SDL_LoadBMP("gfx/palette.bmp");
	if (palette == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to load \"gfx/palette.bmp\"", window);
		exit(1);
	}

	/* load colormap */
	colormap = SDL_LoadBMP("gfx/colormap.bmp");
	if (colormap == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to load \"gfx/colormap.bmp\"", window);
		exit(1);
	}

	/* load logo */
	logo = SDL_LoadBMP("gfx/logo.bmp");
	if (logo == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to load \"gfx/logo.bmp\"", window);
		exit(1);
	}

	/* add logo color key */
	SDL_SetColorKey(logo, SDL_TRUE, 0xFF);

	/* load background */
	background = SDL_LoadBMP("gfx/background.bmp");
	if (background == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to load \"gfx/background.bmp\"", window);
		exit(1);
	}
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

	/* resize window */
	SDL_SetWindowSize(window, DREAMRAY_WIDTH * 2, DREAMRAY_HEIGHT * 2);
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

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

	/* add background */
	SDL_BlitSurface(background, &rect, surface8, &rect);

	/* add logo */
	logo_rect.x = (surface8->w / 2) - (logo->w / 2);
	logo_rect.y = 8;
	logo_rect.w = logo->w;
	logo_rect.h = logo->h;
	SDL_BlitSurface(logo, NULL, surface8, &logo_rect);

	/* main loop */
	while (!SDL_QuitRequested())
	{
		/* push events */
		while (SDL_PollEvent(&event))
			eui_sdl2_event_push(&event);

		/* process events */
		eui_event_queue_process();

		/* run eui */
		if (eui_context_begin())
		{
			eui_frame_align_set(EUI_ALIGN_MIDDLE, EUI_ALIGN_MIDDLE);

			eui_widget_button(0, -48, 80, 16, "Play", button_play, NULL);
			eui_widget_button(0, -24, 80, 16, "Editor", button_editor, NULL);
			eui_widget_button(0, 0, 80, 16, "Options", button_options, NULL);
			eui_widget_button(0, 24, 80, 16, "Quit", button_quit, NULL);

			/* end eui context */
			eui_context_end();
		}

		/* copy to screen */
		SDL_BlitSurface(surface8, &rect, surface32, &rect);
		SDL_UpdateTexture(texture, NULL, surface32->pixels, surface32->pitch);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	/* quit */
	button_quit(NULL);

	return 0;
}

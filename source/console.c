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
 * CONSOLE.C
 *
 */

#include <stdio.h>

#include "SDL.h"
#include "eui.h"
#include "eui_evnt.h"
#include "eui_sdl2.h"
#include "eui_widg.h"
#include "dreamray.h"

void console_main(void)
{
	int key;

	/* parse queued inputs */
	while ((key = eui_key_pop()) >= 0)
	{
		switch (key)
		{
			case EUI_SCANCODE_ESCAPE:
				gamestate = GAMESTATE_MENU;
				break;
		}
	}

	/* run eui */
	if (eui_context_begin())
	{
		eui_frame_align_set(EUI_ALIGN_MIDDLE, EUI_ALIGN_MIDDLE);

		eui_draw_text(0, 0, 0x0F, "CONSOLE");

		/* end eui context */
		eui_context_end();
	}
}

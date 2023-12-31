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

#pragma once
#ifndef _DREAMRAY_H_
#define _DREAMRAY_H_
#ifdef __cplusplus
extern "C" {
#endif

/*
 *
 * macros
 *
 */

#define DREAMRAY_WIDTH (640)
#define DREAMRAY_HEIGHT (480)
#define DREAMRAY_TITLE "DREAMRAY"

/*
 *
 * enums
 *
 */

enum {
	GAMESTATE_CONSOLE,
	GAMESTATE_MENU,
	GAMESTATE_GAME
};

/*
 *
 * globals
 *
 */

extern int gamestate;

/*
 *
 * public functions
 *
 */

void quit(int code);

void error_load(char *filename);

void console_main(void);
void menu_main(void);
void game_main(void);

#ifdef __cplusplus
}
#endif
#endif /* _DREAMRAY_H_ */

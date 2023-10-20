/*Header file contains relevant includes, defintions, 
globals, structures and function declerations.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <Windows.h>

#define NUL	0x00
#define ESC	0x1b

#define UL		218
#define LL		192
#define UR		191
#define LR		217
#define HBAR	196
#define VBAR	179
#define SHADE   178

#define	DISP(x)	putc((x),stdout)

extern HANDLE Console;

void move_cursor(int row, int col);
void draw_building(int ulr, int ulc);
void draw_quadrant(int numX, int numY, int clock);
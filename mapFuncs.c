#include "mapImp.h"


void terminate(char* msg)
{
/* Fatal error detected - terminate */
printf("Error: %s\n", msg);
(void) getchar();
exit(1);
}

void screen_size()
{
/*
 - Query Windows for new window size
 - The screen (window) must be resized before this is called (see check_kb())
*/
CONSOLE_SCREEN_BUFFER_INFO ScreenBufferInfo;

/* Get new screen size */
GetConsoleScreenBufferInfo(scrout, &ScreenBufferInfo);

/* Erase existing diagnostic line */
CUP(1, scr_size.Y)
EL(scr_size.Y)

/* Get new screen size */
scr_size.X = ScreenBufferInfo.srWindow.Right - ScreenBufferInfo.srWindow.Left + 1;
scr_size.Y = ScreenBufferInfo.srWindow.Bottom - ScreenBufferInfo.srWindow.Top + 1;

/* Clear the screen */
CLRSCR

/* Resizing displays cursor - hide it */
printf(CSI "?25l");
}

void buildBlock(){

    int x, y;
    x = 4;
    y = 3;

    block = (BLDNG**)malloc(sizeof(BLDNG*) * x);
    for(int i = 0; i < x; i++)
        block[i] = (BLDNG*)malloc(sizeof(BLDNG) * y);
    
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            block[i][j].x = i;
            block[i][j].y = j;
            printf("Building %i,%i\n",block[i][j].x,block[i][j].y);
        }
    }

}
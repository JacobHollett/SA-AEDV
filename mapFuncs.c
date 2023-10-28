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

void buildBlock(int x, int y){

    
    //dynamically creating 2D arrays of buildings and roads
    block = (BLDNG**)malloc(sizeof(BLDNG*) * x);
    for(int i = 0; i < x; i++)
        block[i] = (BLDNG*)malloc(sizeof(BLDNG) * y);

    roads = (CELL**)malloc(sizeof(CELL) * (x+1));
    for(int i = 0; i < x; i++)
        roads[i] = (CELL*)malloc(sizeof(CELL) * (y + 1));
    
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            block[i][j].x = i;
            block[i][j].y = j;
        }
    }

}

void build_fleet(){
    //dynamically creating fleet of four AEDV's
    fleet = (AEDV*)malloc(sizeof(AEDV*) * 4);
    for(int i = 0; i < 4; i++){
        fleet[i].x = i;
        fleet[i].y = i+3;
        fleet[i].destx = 0;
        fleet[i].desty = 0;
        fleet[i].IDNUM = (i*3+1)*2-3;
    }
}

void box(int ulx, int uly, char *name, int colour, char *ID){

    printf(CSI "%dm", colour);
    printf(ESC "(0");

    CUP(ulx, uly)
    printf("lqqk"); //UL horizontal line x2 UR
    CUP(ulx, uly+1)
    printf("x%sx", name); //Vert name vert
    CUP(ulx, uly+2)
    printf("x%sx", ID); //vert ID vert
    CUP(ulx, uly+3)
    printf("mqqj");			/* LL Hor Hor LR */
    
    printf(ESC "(B");		/* Return to ASCII */
    printf(CSI "0m");		/* Return to default FG anf BG */

}

void populate_map(int x, int y){

    char name[] = "AA";
    char *ID = "BT";

    printf(CSI "2J"); /* Clear screen */
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            box(i*8+4,j*8+4,name, BGRED, ID);
            name[1]++;
        }
        name[0]++;
    }
    print_controls(1);
}

void print_controls(int code){

    CUP(4, scr_size.Y-1)
    printf(CSI "%dm", BGBLUE);
    if(code == 1)
        printf("D = DIAGNOSTIC WINDOW           R = Resize Window           ! = END");
    else
        printf("M = MAP WINDOW                  R = Resize Window           ! = END");
        printf(CSI "%dm", BGBLACK);
}

void status_window(){

    CUP(1,4)

    printf("DIAGNOSTIC WINDOW\n\n");
    for(int i = 0; i < 4; i++){
        printf("AEDV #%i: %i,%i -> %i,%i\n", fleet[i].IDNUM, fleet[i].x, fleet[i].y, fleet[i].destx, fleet[i].desty);
    }
    print_controls(2);

}

int check_kb(){

    int rc;
    rc = (char)_getch();

    switch(rc){
        case 'r':
        case 'R':
            screen_size();
	        populate_map(8,3);
            rc = FALSE;
	        break;
        case 'd':
        case 'D':
            printf(CSI "?1049h");
            status_window();
            rc = FALSE;
	        break;
        case 'm':
        case 'M':
            printf(CSI "?1049l");
            populate_map(8,3);
            rc = FALSE;
	        break;
        case '!':
            rc = TRUE;
	        break;
    }
    printf(CSI "?25l");	/* Hide cursor */
    return rc;
}
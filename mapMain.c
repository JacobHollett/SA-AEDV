#include "mapImp.h"

HANDLE scrout, keyin;
COORD scr_size;

BLDNG **block;
CELL **roads;
AEDV *fleet;


int main(){

    long outmode;

    /* Microsoft specific commands: */
    if ((scrout = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE)
	    terminate("Can't open output");

    if (!GetConsoleMode(scrout, &outmode))
	    terminate("Can't get old console mode");

    outmode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(scrout, outmode))
	    terminate("Can't set console mode");
        
    screen_size();
    buildBlock();

    //fflush(stdin);
    getchar();
    return 0;
}


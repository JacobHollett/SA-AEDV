#include "mapImp.h"

HANDLE scrout, keyin;
COORD scr_size;

BLDNG **block;
CELL **roads;
AEDV *fleet;
int STOP = FALSE;


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
    buildBlock(8, 3);
    build_fleet();

    populate_map(8,3);
    
    while(!STOP){
        if (_kbhit())
			STOP = check_kb();
    }
    
    free(block);
    free(roads);
    free(fleet);
    return 0;
}


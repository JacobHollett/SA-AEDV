/*AEDV Final Project main file
Jacob Hollet & Paul Sujith
OCT 31, 2023*/
#include "mapImp.h"

HANDLE scrout, keyin;
COORD scr_size;
FILE *bfd;
BLDNG **block;
CELL **roads;
AEDV *fleet;
XY bounds;
int fleetSize = 5;
int STOP = 1;
float TIME = 0;


int main(int argc, char *argv[]){

    long outmode;
    //boilerplate error checking for virtual vt100 terminal and input file
    /* Microsoft specific commands: */
    if ((scrout = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE)
	    terminate("Can't open output");

    if (!GetConsoleMode(scrout, &outmode))
	    terminate("Can't get old console mode");

    outmode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(scrout, outmode))
	    terminate("Can't set console mode");

    if (argc != 2){
	printf("Supply name of data file\n");
	(void) getchar();
	return 1;
    }
        
    _set_fmode(O_BINARY);
    if ((bfd = fopen(argv[1], "r")) < 0){
	printf("Can't open %s\n", argv[1]);
	getchar();
	return 1;
    }

    screen_size();
    read_file();

    build_fleet();

    populate_map();
    move();
    
    free(block);
    free(roads);
    free(fleet);
    return 0;
}


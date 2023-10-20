#include "mapImp.h"

void move_cursor(int row, int col){

    /* Move cursor to specified row and column */
    COORD coord;
    coord.X = col;
    coord.Y = row;
    /* Windows function to position cursor */
    SetConsoleCursorPosition(Console, coord);

}

//function adapted from Prof. Hughes, for drawing fixed 3x3 buildings
void draw_building(int ulr, int ulc){
    /* 
    - Draw box at ulr, ulc
    - Height and width are "scaled"
    */
    int rc;	/* Row count */
    int cc;	/* Column count */

    /* Top row */
    move_cursor(ulr, ulc);
    DISP(UL);
	DISP(HBAR);
    DISP(UR);

    /* Center Row */
	move_cursor(ulr+1, ulc);
	DISP(VBAR);
    DISP(SHADE);
	DISP(VBAR);
    

    /* Bottom row */
    move_cursor(ulr+2, ulc);
    DISP(LL);
    DISP(HBAR);
    DISP(LR);
}

void draw_quadrant(int numX, int numY, int clock){

    for(int i = 0; i < numX; i++){
        for(int j = 0; j < numY; j++){
            draw_building(5+6*j,6*i);
        }
    }

}
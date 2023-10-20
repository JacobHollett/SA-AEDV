#include "mapImp.h"

HANDLE Console;		/* Identify action and I/O stream */

int main(){

    COORD scrsize;
    int row;
    int col;
    unsigned char ch;

    /* For console output via Windows */
    Console = GetStdHandle(STD_OUTPUT_HANDLE);
    /* Determine screen size */
    scrsize = GetLargestConsoleWindowSize(Console);
    printf("Size: r: %d c: %d\n", scrsize.Y, scrsize.X);
    
    move_cursor(2, 1);
    printf("Please enter number of buildings along the avenue:");
    scanf("%i", &row);
    printf("Please enter number of buildings along the street:");
    scanf("%i", &col);
    
    draw_quadrant(row, col, 4);
    
    
    fflush(stdin);
    getchar();
    return 0;
}


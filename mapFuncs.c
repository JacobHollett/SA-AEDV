#include "mapImp.h"


void terminate(char* msg)
{
/* Fatal error detected - terminate */
printf("Error: %s\n", msg);
(void) getchar();
exit(1);
}

void read_file(){

    enum ST_DIR s1dir;
    enum AVE_DIR a1dir;

    fread(&bounds.x, sizeof(int), 1, bfd);
    fread(&bounds.y, sizeof(int), 1, bfd);

    buildBlock();

    fread(&s1dir, sizeof(int), 1, bfd);
    fread(&a1dir, sizeof(int), 1, bfd);

    orient_roads(s1dir, a1dir);

    fclose(bfd);
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

void buildBlock(){  //dynamically creating 2D arrays of buildings and roads

    block = (BLDNG**)malloc(sizeof(BLDNG*) * bounds.x);
    for(int i = 0; i < bounds.x; i++)
        block[i] = (BLDNG*)malloc(sizeof(BLDNG) * bounds.y);

    roads = (CELL**)malloc(sizeof(CELL) * (bounds.x+1));   //We create K more cells than is required,
    for(int i = 0; i < (bounds.x+1); i++)                    //where K is number of buildings, these cells go unused
        roads[i] = (CELL*)malloc(sizeof(CELL) * (bounds.y+1));
    
    for(int i = 0; i < bounds.x; i++){
        for(int j = 0; j < bounds.y; j++){
            block[i][j].x = i;
            block[i][j].y = j;
        }
    }

}

void orient_roads(int a1, int s1){
    
    roads[0][0].next1 = &roads[1][0];
    roads[0][0].next2 = &roads[0][1];
    roads[bounds.x][0].next1 = &roads[bounds.x-1][0];
    roads[bounds.x][0].next2 = &roads[bounds.x][1];
    roads[0][bounds.y].next1 = &roads[1][bounds.y];
    roads[0][bounds.y].next2 = &roads[0][bounds.y-1];
    roads[bounds.x][bounds.y].next1 = &roads[bounds.x-1][bounds.y];
    roads[bounds.x][bounds.y].next2 = &roads[bounds.x][bounds.y];
    
}

void build_fleet(){
    //dynamically creating fleet of four AEDV's
    fleet = (AEDV*)malloc(sizeof(AEDV*) * 4);
    for(int i = 0; i < 4; i++){
        fleet[i].x = i;
        fleet[i].y = i+3;
        fleet[i].destx = fleet[i].x;
        fleet[i].desty = fleet[i].y;
        fleet[i].IDNUM = i+1;
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
        printf("M = MAP WINDOW                  N = Set Destination         R = Resize Window           ! = END");
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

void set_dest(){

    int idval;
    int dstx;
    int dsty;
    
    CUP(1, 12)
    printf("\nInput AEDV ID: ");
    scanf("%i", &idval);
    getchar();
    printf("Input new destination x coordinate: ");
    scanf("%i", &dstx);
    getchar();
    printf("Input new destination y coordinate: ");
    scanf("%i", &dsty);
    getchar();
    fleet[idval-1].destx = dstx;
    fleet[idval-1].desty = dsty;
    status_window();
}

int check_kb(){

    int rc;
    rc = (char)_getch();

    switch(rc){
        case 'r':
        case 'R':
            screen_size();
	        populate_map(bounds.x,bounds.y);
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
            populate_map(bounds.x,bounds.y);
            rc = FALSE;
	        break;
        case 'n':
        case 'N':
            printf(CSI "?1049h");
            set_dest();
            rc = FALSE;
            break;
        case '!':
            rc = TRUE;
	        break;
    }
    printf(CSI "?25l");	/* Hide cursor */
    return rc;
}


#include "mapImp.h"


void terminate(char* msg)
{
/* Fatal error detected - terminate */
printf("Error: %s\n", msg);
(void) getchar();
exit(1);
}

void read_file(){
    BLDNG bd;
    enum ST_DIR s1dir;
    enum AVE_DIR a1dir;

    fread(&bounds.x, sizeof(int), 1, bfd);
    fread(&bounds.y, sizeof(int), 1, bfd);

    buildBlock();

    fread(&s1dir, sizeof(int), 1, bfd);
    fread(&a1dir, sizeof(int), 1, bfd);

    orient_roads(s1dir, a1dir);

    fread(&bd, sizeof(BLDNG), 1, bfd);
    
    while(bd.x > 0){
        if(bd.x <= (bounds.x) && bd.y <= (bounds.y)){
            block[bd.x-1][bd.y-1].bt = bd.bt;
            block[bd.x-1][bd.y-1].qd = bd.qd;
        }
        
        fread(&bd, sizeof(BLDNG), 1, bfd);
    }
    
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

    roads = (CELL**)malloc(sizeof(CELL) * ((2*bounds.x)+1));   //We create K more cells than is required,
    for(int i = 0; i < ((2*bounds.x)+1); i++)                    //where K is number of buildings, these cells
        roads[i] = (CELL*)malloc(sizeof(CELL) * ((2*bounds.y)+1));    //are considered occupied
    
    for(int i = 0; i < bounds.x; i++){
        for(int j = 0; j < bounds.y; j++){
            block[i][j].x = i;
            block[i][j].y = j;
            block[i][j].bt = 3;
        }
    }
    
    for(int i = 0; i <= (2*bounds.x); i++){
        for(int j = 0; j <= (2*bounds.y); j++){
            roads[i][j].x = i;
            roads[i][j].y = j;
            
            roads[i][j].next1 = NULL;
            roads[i][j].next2 = NULL;
            roads[i][j].next3 = NULL;
            roads[i][j].next4 = NULL;
            
        }
    }

}

void orient_roads(int a1, int s1){
    
    int flag = s1;
    
    //East West Connections
    for(int i = 0; i <= 2*bounds.x; i++){
        for(int j = 0; j <= 2*bounds.y; j+=2){
            if(j == 0 || j == 2*bounds.y){
                if(i != 0)
                    roads[i][j].next1 = &roads[i-1][j];
                
                if(i != 2*bounds.x)
                    roads[i][j].next2 = &roads[i+1][j];
            }
            else if(flag == 0 && i != 2*bounds.x){
                roads[i][j].next2 = &roads[i+1][j];
                flag = 1;
            }
            else if(flag == 1 && i != 0){
                roads[i][j].next1 = &roads[i-1][j];
                flag = 0;
            }
        }
    }

    flag = a1;
    //North South connections
    for(int i = 0; i <= 2*bounds.x; i+=2){
        for(int j = 0; j <= 2*bounds.y; j++){
            if(i == 0 || i == 2*bounds.x){
                if(j != 0)
                    roads[i][j].next3 = &roads[i][j-1];
                if(j != 2*bounds.y)
                    roads[i][j].next4 = &roads[i][j+1];
            }
            else if(flag == 0 && j != 0){
                roads[i][j].next3 = &roads[i][j-1];
                flag = 1;
            }
            else if(flag == 1 && j != 2*bounds.y){
                roads[i][j].next4 = &roads[i][j+1];
                flag = 0;
            }
        }
    }
    
}

void build_fleet(){
    //dynamically creating fleet of four AEDV's
    fleet = (AEDV*)malloc(sizeof(AEDV*) * 4);
    for(int i = 0; i < 4; i++){
        fleet[i].x = i;
        fleet[i].y = 2*i;
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

void populate_map(){

    char name[] = "AA";
    char *ID;

    printf(CSI "2J"); /* Clear screen */
    for(int i = 0; i < bounds.x; i++){
        for(int j = 0; j < bounds.y; j++){
            if(block[i][j].bt == 0)
                ID = "CG";
            else if(block[i][j].bt == 1)
                ID = "ST";
            else if(block[i][j].bt == 2)
                ID = "BT";
            else
                ID = "NA";
            
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

    printf("    DIAGNOSTIC   WINDOW\n\n");
    for(int i = 0; i < 4; i++){
        printf("    AEDV #%i: %i,%i -> %i,%i\n", fleet[i].IDNUM, fleet[i].x, fleet[i].y, fleet[i].destx, fleet[i].desty);
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


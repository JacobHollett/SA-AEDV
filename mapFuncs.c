/*AEDV Final Project functional file
Jacob Hollet & Paul Sujith
OCT 31, 2023
Contains functions responsible for several processes*/

#include "mapImp.h"


void terminate(char* msg) //given error handling function
{
/* Fatal error detected - terminate */
printf("Error: %s\n", msg);
(void) getchar();
exit(1);
}

void read_file(){   //function reads through given standard binary file (building file)
    BLDNG bd;       //and call appropiate functions to pass off read information.
    enum ST_DIR s1dir;
    enum AVE_DIR a1dir;

    fread(&bounds.x, sizeof(int), 1, bfd);
    fread(&bounds.y, sizeof(int), 1, bfd);

    buildBlock();

    fread(&s1dir, sizeof(int), 1, bfd);
    fread(&a1dir, sizeof(int), 1, bfd);

    orient_roads(s1dir, a1dir);

    fread(&bd, sizeof(BLDNG), 1, bfd);
    
    while(bd.x > 0){    //token error handling so as not to assign properties to buildings outside range
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
        roads[i] = (CELL*)malloc(sizeof(CELL) * ((2*bounds.y)+1));    //are buildings and will in the future be
    
    for(int i = 0; i < bounds.x; i++){                                //considered occupied
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
            roads[i][j].occupied = 0;
            roads[i][j].next1 = NULL;
            roads[i][j].next2 = NULL;
            roads[i][j].next3 = NULL;
            roads[i][j].next4 = NULL;
            
        }
    }

}

void orient_roads(int a1, int s1){ 
    //algorithm to determine cell linkages based on road directions
    //potential area of improvement
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
            }
            else if(flag == 1 && i != 0){
                roads[i][j].next1 = &roads[i-1][j];
            }
            if(flag == 0 && j != 0 && j != 2*bounds.y)
                flag = 1;
            else if(flag == 1 && j != 0 && j != 2*bounds.y)
                flag = 0;
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
            }
            else if(flag == 1 && j != 2*bounds.y){
                roads[i][j].next4 = &roads[i][j+1];
            }
            
        }
        if(flag == 0 && i != 0 && i != 2*bounds.x)
            flag = 1;
        else if(flag == 1 && i != 0 && i != 2*bounds.x)
            flag = 0;
    }
    
}

void build_fleet(){
    //dynamically creating fleet of four AEDV's
    //similar to buildBlock
    fleet = (AEDV*)malloc(sizeof(AEDV*) * fleetSize);
    for(int i = 0; i < fleetSize; i++){
        fleet[i].x = 0;
        fleet[i].y = 0;
        fleet[i].destx = fleet[i].x;
        fleet[i].desty = fleet[i].y;
        fleet[i].IDNUM = i+1;
        fleet[i].battery = 60;
        fleet[i].state = 0;
        fleet[i].pathStep = 0;
        fleet[i].load = 0;
        fleet[i].climbTime = 0;
        fleet[i].potLoad = 0; 
    }
}

void box(int ulx, int uly, char *name, int colour, char *ID){
    //draws a 4x4 box so as to simplify movement logic
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
    printf(CSI "0m");		/* Return to default FG and BG */

}

void populate_map(){
    //draws and labels buildings
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
            
            box(i*8+5,j*8+5,name, BGRED, ID);
            name[1]++;
        }
        name[1] = 'A';
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
        printf("M = MAP WINDOW  N = SET MANUAL DESTINATION  L = LOAD EVENT FILE     R = Resize Window   ! = END");
        printf(CSI "%dm", BGBLACK);
}

void status_window(){
    
    printf(CSI "?1049h");
    CLRSCR
    CUP(4,4)
    printf(CSI "%dm", BGBLUE);
    int ypos = 0;   //keeps track of cursor y position for formatting
    int counter = 0; //index for event list
    printf("                           DIAGNOSTIC  WINDOW                            ");
    CUP(4,5)
    printf(" ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~AEDV  REPORT~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ");
    CUP(4,6)
    for(int i = 0; i < fleetSize; i++){
        printf(" AEDV #%i: %2i,%2i -> %2i,%2i  Battery Level:%2i  State:%i  Current Load: %2i Kg ",
         fleet[i].IDNUM, fleet[i].x, fleet[i].y, fleet[i].destx, fleet[i].desty, fleet[i].battery, fleet[i].state, fleet[i].load);
        CUP(4,7+i)
        ypos = i + 7;
    }
    printf(" ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~NEXT  REQUEST~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ");
    ypos++;//for foromatting
    CUP(4,ypos)
    printf(" TIME      TYPE    Source Customer    Destination Customer        WEIGHT ");
    ypos++;
    CUP(4, ypos)
    printf("%4i       %c       %4i               %4i                        %3i Kg ", 
        currentEvent.time, currentEvent.type, currentEvent.srcID, currentEvent.destID, currentEvent.weight);
    

    printf(CSI "0m");
    print_controls(2);
    while(!STOP){
        if (_kbhit())
	        check_kb();
    }
}

void set_dest(){

    int idval;
    int dstx;
    int dsty;
    printf(CSI "?1049h");
    CUP(1, 12)
    printf("\nInput AEDV ID: ");
    scanf("%i", &idval);
    getchar();
    idval--;
    printf("Input new destination x coordinate: ");
    scanf("%i", &dstx);
    getchar();
    printf("Input new destination y coordinate: ");
    scanf("%i", &dsty);
    fleet[idval].destx = dstx;
    fleet[idval].desty = dsty;
    getchar();
    find_path(idval);
    status_window();
}

void check_kb(){

    int rc;
    rc = (char)_getch();

    switch(rc){
        case 'r':
        case 'R':
            screen_size();
	        populate_map(bounds.x,bounds.y);
            STOP = 1;
	        break;
        case 'd':
        case 'D':
            STOP = 0;
            status_window();
	        break;
        case 'm':
        case 'M':
            printf(CSI "?1049l");
            STOP = 1;
	        break;
        case 'n':
        case 'N':
            STOP = 0;
            set_dest();
            break;
        case 'l':
        case 'L':
            STOP = 0;
            read_events();
            break;
        case '!':
            STOP = 2;
	        break;
    }
    printf(CSI "?25l");	/* Hide cursor */
}

void read_events(){

    printf(CSI "?1049h");
    CUP(1, 12)
    char fileName[MAXLINELENGTH];
    _set_fmode(O_TEXT);
    printf("Enter name of event file: ");
    scanf("%s", &fileName);
    getchar();
    elf = fopen(fileName, "r");
    fgets(fileName,MAXLINELENGTH, elf); //skip over header
    get_next_event();
    printf("Event File loaded! Press Enter to return to diagnostic window.");
    getchar();
    status_window();
}

void get_next_event(){
    char tempString[MAXLINELENGTH];

    if(fgets(tempString,MAXLINELENGTH, elf)){
        sscanf(tempString, "%i %c%i%i%i", 
        &currentEvent.time,&currentEvent.type,&currentEvent.srcID,&currentEvent.destID,&currentEvent.weight);
    }
    else{
        currentEvent.time = -1;
        currentEvent.type = 'N';
        currentEvent.destID = 0000;
        currentEvent.srcID = 0000;
        currentEvent.weight = 0;
    }
}
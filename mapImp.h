/*AEDV Final Project header file
Jacob Hollet & Paul Sujith
OCT 31, 2023
Header file contains relevant includes, defintions, 
globals, structures and function declerations.*/
#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>     /* for _O_TEXT and _O_BINARY */

#define NUL	'\0'

/* Prefixes for ESC sequences */
#define ESC "\x1b"	/* Escape */
#define CSI "\x1b["	/* Control Sequence Introducer */

#define TRUE	1
#define FALSE	0

/* Some commonly used VT-100 commands */
#define CUP(c,r)	printf(CSI "%d;%dH", (r), (c)); /* Move cursor position to col, row */
#define EL(r)		printf(CSI "%d;1H" CSI "K", (r)); /* Erase in Line */
#define CLRSCR		printf(CSI "2J");
#define MAXPATHLENGTH 900 //placeholder maximum length of path
#define BIGNUM 99999999 //big number for placeholder F values in calc_path
#define MAXEVENTS 100
#define MAXLINELENGTH 50
#define database "customers.bin"
#define log "deleveries.log"
#define NAMELEN 16 //maximum name length

/* Colour: ESC [ <n> m */
enum VT100_COLOURS {
FGWHITE = 32,
BGRED = 41,
BGGREEN = 42, 
BGYELLOW = 43,
BGBLUE = 44,
BGCYAN = 46,
BGBLACK = 49
};

enum ST_DIR { East, West };
enum AVE_DIR { North, South };
enum BLDG_TYPE { CHG, STB, BOTH, NEITHER};
enum QUAD { NE, N, NW, E, LBL, W, SE, S, SW };
enum REC_STATUS {ACTIVE, DELETED};


typedef struct aedv{
    int state; //0 is idle 1 is active 2 is looking for charger 3 is charging
    int IDNUM;
    int x,y;
    int destx, desty;
    int prevx, prevy; // previous destination point, needs to be held in case of charging event
    int nextx, nexty; // next destination, used only for deliveries
    int climbTime; //stores time taken to pick up and drop off
    int battery;
    int pathStep;
    int path[MAXPATHLENGTH];
    int load;
    int potLoad; //hiding next load in here
    char srcName[MAXLINELENGTH];
    char dstName[MAXLINELENGTH];
    int pickupTime;
}AEDV;

typedef struct bldng{ //standard building structure. Cannot change.
    int x;
    int y;
    enum BLDG_TYPE bt;
    enum QUAD qd;
}BLDNG;

typedef struct cell{    //roads structure mimics linked list 
    int x;
    int y;
    struct cell *next1; //points behind
    struct cell *next2; //points forward
    struct cell *next3; //points up
    struct cell *next4; //points down
    int occupied;
}CELL;

typedef struct event{
    int time;
    char type;
    int srcID;
    int destID;
    int weight;
}EVENT;

typedef struct xy{
    int x;
    int y;
}XY;

typedef struct customer{
    enum REC_STATUS status; 
    char lname[NAMELEN]; 
    char fname[NAMELEN];
    int id; 
    XY bld;
    enum QUAD quadrant;
    int floor; 
}CUSTOMER;

extern HANDLE scrout, keyin;
extern COORD scr_size;
extern BLDNG **block;
extern CELL **roads;
extern AEDV *fleet;
extern XY bounds;
extern int fleetSize;
extern int STOP;    //loop stop controller
extern float TIME;  //current time in clock ticks
extern FILE *bfd;   //building file descriptor
extern FILE *cdf;   //customer data file
extern FILE *elf;   //event list file
extern FILE *olf;   //output log file
extern EVENT currentEvent;


void terminate(char* msg);
void read_file();
void screen_size();
void buildBlock();
void orient_roads(int a1, int s1);
void build_fleet();
void box(int ulx, int uly, char *name, int colour, char *ID);
void populate_map();
void print_controls(int code);
void status_window();
void check_kb();
void move();
void find_path(int k);
void check_for_charger(int k);
void read_events();
void check_events();
void get_next_event();
void handle_destination(int k);
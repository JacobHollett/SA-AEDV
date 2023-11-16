#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h> 

#define NL	'\n'
#define NUL	'\0'
#define NAMELEN	33
#define TRUE	1
#define FALSE	0
#define DEL_END -1l

/*TODO
Chuck all this junk in a header file 
then make an oredr file that just contains
source customer, destingation customer and time
then we just need one file to convert customer database to relative access binary
order files can remain sequential access.*/
enum REC_STATUS {ACTIVE, DELETED};
enum QUAD { NE, N, NW, E, LBL, W, SE, S, SW };

typedef struct xy{
    int x;
    int y;
}XY;

typedef struct header{
    int del_rec; //number of deleted records
    int first_id; //next available customer id
}HEADER;

typedef struct customer{
    enum REC_STATUS status; //status
    char lname[NAMELEN]; //name
    char fname[NAMELEN];
    int id; //customer id
    XY bldng; //building inhabited
    enum QUAD quadrant; //quadrant inhabited
    int floor; //floor inhabited
}CUSTOMER;

typedef struct deleted_customer{
    enum REC_STATUS status;
    int next_deleted;   
}DEL_CUST;

typedef union customerFile{
    HEADER hrec;
    CUSTOMER crec;
    DEL_CUST drec;
}CFREC;


int main(int argc, char *argv[]){




    return 0;
}
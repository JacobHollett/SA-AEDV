#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

#define NAMELEN	16	/* Filename length */

enum REC_STATUS { ACTIVE, DELETED };
enum QUAD { NE, N, NW, E, LBL, W, SE, S, SW };
//Need to check what LBL is. Could be typo?

typedef struct xy {
    int x;
    int y;
} XY;

typedef struct customer {
    enum REC_STATUS status;
    char lname[NAMELEN];
    char fname[NAMELEN];
    int id;
    XY bldng;
    enum QUAD quadrant;
    int floor;
} CUSTOMER;

int main(int argc, char* argv[]) {
    const char* TextFile = "customers.txt";

    FILE* binary_fp = fopen(argv[1], "rb");

    FILE* text_fp = fopen(TextFile, "w");

    if (binary_fp == NULL || text_fp == NULL) {
        printf("Error opening files.\n");
        return 1;
    }

    CUSTOMER customer;

    //Header row to the text file
    fprintf(text_fp, "Status,Last Name,First Name,Customer ID,Coord X,Coord Y,Quad,Floor\n");

    //Read each struct from the binary file
    while (fread(&customer, sizeof(CUSTOMER), 1, binary_fp) == 1) {
        fprintf(text_fp, "%d, %s, %s, %d, %d, %d, %d, %d", 
                customer.status,
                customer.lname,
            	customer.fname,
                customer.id,
                customer.bldng.x,
                customer.bldng.y,
                customer.quadrant,
                customer.floor);
    }

    fclose(binary_fp);
    fclose(text_fp);

    printf("Binary File is converted to Text File");
    getchar();

    return 0;
}
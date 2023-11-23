#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

#define NAMELEN	16	/* Filename length */

// Following code is taken from the order-converter file on github
enum REC_STATUS {ACTIVE, DELETED};
enum QUAD { NE, N, NW, E, LBL, W, SE, S, SW };

typedef struct xy{
    int x;
    int y;
}XY;

typedef struct customer{
    enum REC_STATUS status; 
    char lname[NAMELEN]; 
    char fname[NAMELEN];
    int id; 
    XY bldng;
    enum QUAD quadrant; //Should be a number in the CSV!!
    int floor; 
}CUSTOMER;

int main(int argc, char *argv[]) {

    _set_fmode(_O_BINARY);
    
    const char* BinaryFile = "customers";

    FILE* csv_fp = fopen(argv[1], "r");

    if (argc == 1) {
       printf("CSV file cannot be opened.\n");
       getchar();
       return 1; 
    }

    FILE* binary_fp = fopen(BinaryFile, "w");
   if (binary_fp == NULL) {
       printf("Binary File not opened correctly");
       fclose(csv_fp); 
       getchar();
       return 1; 
   }

    char line[256];
    CUSTOMER customer;

    //Should (theoretically) skip over the first line
    //fgets(line, sizeof(line), csv_fp);

    //Read each line from the CSV file
    while (fgets(line, sizeof(line), csv_fp)) {
        sscanf(line, "%d,%[^,],%[^,],%d,%d,%d,%d,%d",
               (int*)&customer.status,
               customer.lname,
               customer.fname,
               &customer.id,
               &customer.bldng.x,
               &customer.bldng.y,
               (int*)&customer.quadrant,
               &customer.floor);

        // Write the customer struct to the binary file
        fwrite(&customer, sizeof(customer), 1, binary_fp);
    }

    fclose(binary_fp);
    fclose(csv_fp);
    printf("Binary File _customers.bin_ is created. \n");
    getchar();

    return 0;
}
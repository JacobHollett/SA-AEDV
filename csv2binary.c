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
    enum REC_STATUS status; //status
    char lname[NAMELEN]; //name
    char fname[NAMELEN];
    int id; //customer id
    XY bldng; //building inhabited
    enum QUAD quadrant; //quadrant inhabited
    int floor; //floor inhabited
}CUSTOMER;

int main(int argc, char *argv[]) {
    const char* BinaryFile = "customers.bin";

    FILE* csv_fp = fopen(argv[1], "r");
   if (argc != 1)
{
	printf("Supply the csv file\n");
}

    FILE* binary_fp = fopen(BinaryFile, "wb");
    if (binary_fp == NULL) {
        printf("Failed to open the binary file.\n");
        fclose(csv_fp);
        return 1;
    }

    char line[256];
    CUSTOMER customer;

    // Read each line from the CSV file
    while (fgets(line, sizeof(line), csv_fp)) {
        sscanf(line, "%d,%c[^,],%c[^,],%d,%d,%d,%d,%d",
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

    printf("01010100 01101000 01100101 00100000 01000011 01110101 01110011 01110100 01101111 01101101 01100101 01110010 00100000 01101001 01110011 00100000 01101110 01100101 01110110 01100101 01110010 00100000 01110010 01101001 01100111 01101000 01110100.\n");

    return 0;
}
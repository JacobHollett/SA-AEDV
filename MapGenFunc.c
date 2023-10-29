#include "MapGenReader.h"

void read_file()
{
struct bldg_data bd;
unsigned int xbldg;
unsigned int ybldg;
enum ST_DIR s1dir;
enum AVE_DIR a1dir;

/* #x #y int int buildings in row */
fread(&xbldg, sizeof(int), 1, bfd);
fread(&ybldg, sizeof(int), 1, bfd);
printf("# X bldgs %d # Y bldgs %d\n", xbldg, ybldg);

/* 1Str 1Ave int int (EW 01) and (NS 01) */
fread(&s1dir, sizeof(int), 1, bfd);
fread(&a1dir, sizeof(int), 1, bfd);
printf("First Street dir %s First Avenue dir %s\n", ew[s1dir].name, ns[a1dir].name);

/* Read first record */
fread(&bd, sizeof(struct bldg_data), 1, bfd);
while (bd.x > 0)
{
	printf("Bldg XY: %d %d Type: %s Quad: %s\n", bd.x, bd.y, 
		bldg_t[bd.bt].name, bldg_q[bd.qd].name);
	/* Read next record */
	fread(&bd, sizeof(struct bldg_data), 1, bfd);
}

fclose(bfd);

}

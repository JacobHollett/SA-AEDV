#include "MapGenReader.h"
#include "MapGenFunc.c"

int main(int argc, char *argv[])
{
if (argc != 2)
{
	printf("Supply name of data file\n");
	(void) getchar();
	return;
}

/* Open in binary mode for reading */
_set_fmode(O_BINARY);

if ((bfd = fopen(argv[1], "r")) < 0)
{
	printf("Can't open %s\n", argv[1]);
	getchar();
	return;
}

read_file();

printf("\nDone\n");
(void) getchar();

return 0;
}

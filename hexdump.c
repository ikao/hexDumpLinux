#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <libgen.h>

static const unsigned char *const HexTable = "0123456789abcdef";
int main(int argc, char **argv){
	char printBuff[128];
	unsigned char readBuff[64];
	size_t	rlen	=	0;
	int	pos;
	unsigned int	fpos;
	int	printPos;

	FILE	*ifp	=	NULL;

	if(argc == 1){
		ifp	=	stdin;
	} else if(argc == 2){
		ifp = fopen(argv[1], "rb");
		if(ifp == NULL){
			fprintf(stderr, "File Open error(%s)\n", argv[1]);
			return -1;
		}
	} else {
		fprintf(stderr, "Usage %s [ dump file path ]\n", basename(argv[0]));
		return -2;
	}

	memset(readBuff, 0x20, sizeof(readBuff));
	fpos = 0;
	for(;(rlen = fread(readBuff, 1, 16, ifp)) > 0;){
		memset(printBuff, 0x20, sizeof(printBuff));
		printPos = 0;
		for(pos=0;pos < (int)rlen;pos++){
			printPos = ((pos / 4) * 2) + (pos * 2);
			printBuff[printPos] = HexTable[readBuff[pos] / 16];
			printBuff[printPos + 1] = HexTable[readBuff[pos] % 16];
			if(isprint((int)readBuff[pos])){
				printBuff[40 + pos] = readBuff[pos];
			} else {
				printBuff[40 + pos] = '.';
			}
		}
		printBuff[56]	=	'\0';
		printf("%08x  %s\n", fpos, printBuff);
		memset(readBuff, 0x20, sizeof(readBuff));
		fpos += rlen;
	}

	if(argc > 1)
		fclose(ifp);
	return 0;
}

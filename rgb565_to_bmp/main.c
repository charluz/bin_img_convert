#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

/*---------------------------------------------------------------------
 * Constants
 */
#define SZ_FNAME_LEN_MAX			128

/*------------------------------------------------------------------
 * Global Variables
 */
FILE *fd_src, *fd_dst;
static char *psz_prog, *psz_fnsrc, *psz_fndst;


/*---------------------------------------------------------------------
 * External reference
 */
void bitmap(char *pfbmp, unsigned int width, unsigned height, char *prgb);

/*------------------------------------------------------------------
 * Implementation
 */


/*---------------------------------------------------------------------
 * _print_prog_usage
 */
static void _print_prog_usage(char *psz_prog_name)
{
	printf("\n");
	printf("Usage:\n");
	printf("\t%s W H IMG565\n",  psz_prog_name);
	printf("Descriptions:\n");
	printf("\tConvert rgb565-image to bmp-image ...\n");
	printf("\t\tW, H : the resolution of input BIN image.\n");
	printf("\t\tBMP : the source BMP file.\n");
}

/*---------------------------------------------------------------------
 * main
 */
int main(int argc, char *argv[])
{
	char *psrcbuf, *pbufsave;
	int width, height;
	char *pdstbuf, *pdstsave;
	unsigned u565;
	size_t wsize;
	int is_swapBG = 0;

	psz_prog = argv[0];
	if (argc<4) {
		_print_prog_usage(psz_prog);
		exit(0);
	}

	width = atoi(argv[1]);
	height = atoi(argv[2]);

	if (argc==5 && !strcmp(argv[4], "-s")) {
		is_swapBG = 1;
	}

	/*-- open source file */
	psz_fnsrc = argv[3];
	printf("Opening source file: %s\n", psz_fnsrc);
	fd_src = fopen(psz_fnsrc, "rb");
	if (!fd_src) {
		printf("FATAL: error opening file [%s] !\n", psz_fnsrc);
		exit(1);
	}

	char sz_oext[] = "bmp";
	psz_fndst = prepare_file_name(psz_fnsrc, sz_oext, "_CONV");
	/*-- open/create target file */
	fd_dst = fopen(psz_fndst, "wb");
	if (!fd_dst) {
		printf("FATAL: error creating destination [%s] !\n", psz_fndst);
		if (fd_src) fclose(fd_src);
		exit(1);
	}

	/*-- alloc & read SRC image */
	size_t fsize;
	fseek(fd_src, 0, SEEK_END);
	fsize = ftell(fd_src);

	pbufsave = psrcbuf = (char *)malloc(fsize);
	//printf("SRC[%s] size : %d\n", psz_fnsrc, fsize);

	if (!psrcbuf) {
		printf("Error: insufficient memory!!\n");
		goto _exit;
	}

	fseek(fd_src, 0, SEEK_SET);
	if (fsize != fread(psrcbuf, sizeof(char), fsize, fd_src)) {
		printf("Error: failed to read SRC %s\n", psz_fnsrc);
		goto _exit;
	}

	// printf("Converting %d x %d...\n", width, height);

	wsize = 3 * (fsize / 2);	// rgb565(2 bytes) --> BGR(3 bytes)
	pdstsave =(char *)malloc(wsize);

	//--- bin2bmp function will store the image up-side donw, and
	// use BGR format, leave it to bin2bmp function !!
	for(int row=0; row<height; row++)  {
		pdstbuf = pdstsave + (row * width * 3);
		for (int col=0; col<width; col++) {
			char r, g, b;
			unsigned color565;

			//-- for kneron rgb565 -- gb:rg
			color565 = ((unsigned)psrcbuf[1])<<8;
			color565 += (((unsigned)psrcbuf[0]) & 0xFFu);

			// if (row==height-1 && col==0) {
			// 	printf("[0]= %02x, [1]= %02x -- color565= %04x\n", psrcbuf[0] & 0xFF, psrcbuf[1] & 0xFF, color565 & 0xFFFF);
			// }

			//獲取高位元組的5個bit
			r = (char)((color565 & 0xF800)>>(8));
			//獲取中間6個bit
			g = (char)((color565 & 0x07E0)>>(3));
			//獲取低位元組5個bit
			b = (char)((color565 & 0x001F)<<3);

			psrcbuf += 2;
			//-- BMP 是 BGR 排列
			*pdstbuf++ = r;
			*pdstbuf++ = g;
			*pdstbuf++ = b;
		}
	}
#if 0
	do {
		char r, g, b;
		unsigned color565;

		color565 = ((unsigned)*psrcbuf++)<<8;
		color565 += (((unsigned)*psrcbuf++) & 0xFFu);

		//獲取高位元組的5個bit
		r = (char)((color565 & 0xF800)>>(8));
		//獲取中間6個bit
		g = (char)((color565 & 0x07E0)>>(3));
		//獲取低位元組5個bit
		b = (char)((color565 & 0x001F)<<3);

		//printf("r= %02x, g= %02x, b= %02x");
		*pdstbuf-- = b;
		*pdstbuf-- = g;
		*pdstbuf-- = r;

		fsize -= 2;
		//printf("--> %d ", fsize);
	} while(fsize > 0);

	if (fsize) {
		printf("error: Conversion failed!!\n");
		goto _exit;
	}
#endif

	printf("Writing %s ...\n", psz_fndst);
	bitmap(psz_fndst, width, height, pdstsave);

	/*-- exiting program */
_exit:
	if (fd_src) fclose(fd_src);
	if (fd_dst) fclose(fd_dst);
	if (pbufsave) free(pbufsave);
	if (pdstsave) free(pdstsave);

    exit(0);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdarg.h>			/*for valist */
#include <iostream>			/*for std*/
#include "common.h"
// #include "user_util.h"

/*---------------------------------------------------------------------
 * Constants
 */
#define SZ_FNAME_LEN_MAX			32

/*------------------------------------------------------------------
 * Global Variables
 */
FILE *fd_bmp, *fd_out;
static char *psz_prog, *psz_fnbmp, sz_fnraw[SZ_FNAME_LEN_MAX];


/*---------------------------------------------------------------------
 * External reference
 */


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
	printf("\t%s fname.bmp\n",  psz_prog_name);
	printf("DESCRIPTION:\n");
	printf("\tTransform I80 DVI BMP to I80 RAW image file...\n");
//	printf("\tBMP_FILENAME shall specify the filename without the extension part.\n");
	printf("\tThe program automatically generates the raw file - fname.raw.\n");
//	printf("OPTION:\n");
//	printf(" -w     *width* : specify width of raw image\n");
//	printf(" -h     *height* : specify height of raw image\n");
//	printf(" -bayer *pattern_id* : bayer pattern\n");
//	printf("\t 1=R_GR_GB_B, 2=GR_R_B_GB, 3=B_GB_GR_R, 4=GB_B_R_GR\n");
//	printf(" -b     *bits* : specify bits per pixel\n");
//	printf(" -of    *raw_fname* : file name of output RAW image.\n");
//	printf(" -bs    *block_hsize* : specify the hsize of do block\n");
//	printf(" -cfg   *cfg_fname* : specify full name of cfg file\n");
}

#define ALPHA		0x00
/*---------------------------------------------------------------------
 * _reform_output_image
 */
static void _reform_output_image(FILE *fd, char *pbmp, unsigned hsize, unsigned vsize)
{
	int row, col;
	char *prgb;
	char rawdat[4];

	_assert(fd && hsize && vsize && pbmp);

	// printf("%s() : hsize(%d) vsize(%d)\n", __FUNCTION__, hsize, vsize);

	for (row=vsize-1; row>=0; row--) {
		for (col=0; col<hsize; col++) {
			//printf("row=%d col=%d\n", row, col);
			prgb = pbmp+(hsize*3*row + col*3);
			rawdat[2] = *prgb;	// B
			prgb++;
			rawdat[1] = *prgb;	// G
			prgb++;
			rawdat[0] = *prgb;	// R

			rawdat[3] = ALPHA;
 			//printf("row=%4d col=%4d, data=%4x\n", row, col, rawdat);
//			fwrite(rawdat, sizeof(char), 3, fd);
			fwrite(&rawdat[0], sizeof(char), 1, fd);
			fwrite(&rawdat[1], sizeof(char), 1, fd);
			fwrite(&rawdat[2], sizeof(char), 1, fd);
			fwrite(&rawdat[3], sizeof(char), 1, fd);
		}
	}
}


/*---------------------------------------------------------------------
 * _read_bmp_image
 */
static unsigned _read_bmp_image(FILE *fd, char *pbuf, unsigned imgsize)
{
	size_t fsize;

	_assert(pbuf && imgsize && fd);

	fseek(fd, 54, SEEK_SET);
	fsize = fread((void *)pbuf, sizeof(char), imgsize, fd);
	// printf("%s: fsize= %d\n", __FUNCTION__, fsize);

	return fsize;
}


/*---------------------------------------------------------------------
 * _parse_bmp_dimension
 */
static void _parse_bmp_dimension(FILE *fd, unsigned *phsize, unsigned *pvsize)
{
	char szsize[4];

	_assert(fd);

	/*-- get hsize */
	fseek(fd, 18, SEEK_SET);
	fread(szsize, sizeof(char), 4, fd);
	*phsize = *((unsigned *)szsize);
	// printf("hsize=%d\n", *phsize);

	/*-- get vsize */
	fseek(fd, 22,SEEK_SET);
	fread(szsize, sizeof(char), 4, fd);
	*pvsize = *((unsigned *)szsize);
	// printf("vsize=%d\n", *pvsize);
}



/*---------------------------------------------------------------------
 * main
 */
int main(int argc, char *argv[])
{
	size_t szlen;
	unsigned err;

	psz_prog = argv[0];
	if (argc<2) {
		_print_prog_usage(psz_prog);
		exit(0);
	}

	/*-- open bmp file */
	psz_fnbmp = argv[1];
//	printf("openning bmp fname : %s\n", psz_fnbmp);
	fd_bmp = fopen(psz_fnbmp, "rb");
	if (!fd_bmp) {
		printf("FATAL: error openning BMP [%s] !\n", psz_fnbmp);
		exit(1);
	}

	unsigned hsize, vsize;
	unsigned imgsize;
	char *pbmpbuf=NULL;

	/*-- bmp size parse */
	_parse_bmp_dimension(fd_bmp, &hsize, &vsize);
	printf("BMP[%s] size : %d x %d\n", psz_fnbmp, hsize, vsize);

	/*-- open output file */
	char *psz_oext = (char *)"bin";
	char postfix[32];
	snprintf(postfix, 32, "_%dx%d_XRGBA", hsize, vsize);
	char *psz_ofname = prepare_file_name(psz_fnbmp, psz_oext, postfix);

	fd_out = fopen(psz_ofname, "wb");
	if (!fd_out) {
		printf("FATAL: error openning RAW [%s] !\n", sz_fnraw);
		goto _exit;
	}


	/*-- alloc & read BMP image */
	imgsize = hsize*vsize*3;
	pbmpbuf = (char *)malloc(imgsize);
	if (!pbmpbuf) {
		printf("error: insufficient memory for BMP image!\n");
		goto _exit;
	}

	// printf("read bmp image...\n");
	err = _read_bmp_image(fd_bmp, pbmpbuf, imgsize);
	if (err!=imgsize) {
		printf("error: incorrect image size! req(%d) but read(%d)\n", imgsize, err);
		goto _exit;
	}

#if 0		// DEBUG ONLY
	do {
		FILE *fd_dbg;
		char dbgFN[32];
		snprintf(dbgFN, 32, "BMP_DUMP_%dx%d.bin", hsize, vsize);
		fd_dbg = fopen(dbgFN, "wb");
		fwrite(pbmpbuf, sizeof(char), imgsize, fd_dbg);
		fclose(fd_dbg);
	} while(0);
#endif

	/*-- go reform the RAW image */
	printf("writing raw image...%s\n", sz_fnraw);
	_reform_output_image(fd_out, pbmpbuf, hsize, vsize);

	/*-- exiting program */
_exit:
	if (fd_bmp) fclose(fd_bmp);
	if (fd_out) fclose(fd_out);

	// if (!pbmpbuf) free(pbmpbuf);
    exit(0);
}

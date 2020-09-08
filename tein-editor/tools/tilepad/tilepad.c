#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

typedef uint8_t   u8;
typedef uint16_t u16;
typedef uint16_t u32;
typedef uint16_t u64;
typedef int8_t    s8;
typedef int16_t  s16;
typedef int16_t  s32;
typedef int16_t  s64;

#define PREFIX "out_"
#define TILESIZE 64
#define BPP 4

/* helper function to concatenate two strings together (handles alloc, etc). */
char* __str_concat__ (const char* _a, const char* _b);
/* creates the padded output image from the supplied input and padding data */
void tile_padding(void* _in, void* _out, int _w, int _h, int _xpad, int _ypad, int _padding);

int main (int _argc, char* _argv[])
{
	/* variable declaration */
	int w, h;
	void *in, *out;
	int xpad, ypad;
	int padding;
	char* filename;
	int i;

	/* make sure that enough input args are given */
	if (_argc < 3) { fprintf(stderr, "ERR: Not enough input provided!\n"); }
	else {
		/* cache the padding value for easy access */
		padding = atoi(_argv[1]);
		/* go through each image passed in */
		for (i=2; i<_argc; ++i) {
			/* load the image file to be padded */
			in = (void*)stbi_load(_argv[i], &w,&h, NULL, BPP);
			if (!in) { fprintf(stderr, "ERR: Couldn't load '%s'!\n", _argv[i]); }
			else {
				/* calculate the amount of extra space for padding */
				xpad = ((w / TILESIZE)*2)*padding, ypad = ((h / TILESIZE)*2)*padding;
				/* allocate memory for the padded output */
				out = malloc(((w+xpad)*(h+ypad))*BPP);
				if (!out) { fprintf(stderr, "ERR: Couldn't alloc '%s'!\n", _argv[i]); }
				else {
					/* perform the padding process */
					tile_padding((u8*)in, (u8*)out, w, h, xpad, ypad, padding);
					/* save the padded output */
					filename = __str_concat__(PREFIX, _argv[i]);
					if (!filename) { fprintf(stderr, "ERR: Couldn't save '%s'!\n", _argv[i]); }
					else {
						stbi_write_png(filename, w+xpad, h+ypad, BPP, out, (w+xpad)*BPP);
						free(filename);
					}
					free(out);
				}
				stbi_image_free(in);
			}
		}
	}

	return 0;
}

char* __str_concat__ (const char* _a, const char* _b)
{
	/* don't attempt to concatenate invalid strings */
	size_t size;
	char* output = NULL;
	if (_a && _b) {
		/* allocate enough memory for both strings in output */
		size = (strlen(_a)+strlen(_b)+1)*sizeof(char);
		output = (char*)malloc(size);
		if (output) {
			/* merge the two strings into one */
			memset(output, 0, size);
			strcat(output, _a); strcat(output, _b);
		}
	}
	return output;
}

void tile_padding (u8* _in, u8* _out, int _w, int _h, int _xpad, int _ypad, int _padding)
{
	/* variable declaration */
	int tilesize;
	int tilecount;
	u8 *buffer;
	int xoff, yoff;
	int i,j,k, ix,iy;
	int xpix, ypix;

	/* determine information regarding the tiles */
	tilesize = TILESIZE+(_padding*2);
	tilecount = (_w*_h)/(TILESIZE*TILESIZE);

	/* create a buffer for handling each tile's padding */
	buffer = (u8*)malloc((tilesize*tilesize)*BPP);
	if (!buffer) { fprintf(stderr, "ERR: Alloc buffer!\n"); }
	else {
		/* go through for each tile in the image */
		xoff = 0, yoff = 0;
		for (i=0; i<tilecount; ++i) {
			/* copy from the input into the tile buffer */
			for (j=(yoff*TILESIZE), k=0; j<((yoff*TILESIZE)+TILESIZE); ++j, ++k) {
				memcpy((void*)&buffer[(_padding+k)*(tilesize*BPP)+(_padding*BPP)], (void*)&_in[j*(_w*BPP)+((xoff*TILESIZE)*BPP)], (TILESIZE*BPP));
			}
			/* handle applying the padding for the tile buffer */
			xpix = 0, ypix = 0;
			for (iy=0; iy<tilesize; ++iy) {
				for (ix=0; ix<(tilesize*BPP); ix+=BPP) {
					xpix = (ix/BPP)-_padding, ypix = iy-_padding;
					if (xpix < 0) { xpix = 0; } else if (xpix >= TILESIZE-1) { xpix = TILESIZE-1; }
					if (ypix < 0) { ypix = 0; } else if (ypix >= TILESIZE-1) { ypix = TILESIZE-1; }
					xpix += _padding, ypix += _padding;
					buffer[iy*(tilesize*BPP)+(ix+0)] = buffer[ypix*(tilesize*BPP)+((xpix*BPP)+0)];
					buffer[iy*(tilesize*BPP)+(ix+1)] = buffer[ypix*(tilesize*BPP)+((xpix*BPP)+1)];
					buffer[iy*(tilesize*BPP)+(ix+2)] = buffer[ypix*(tilesize*BPP)+((xpix*BPP)+2)];
					buffer[iy*(tilesize*BPP)+(ix+3)] = buffer[ypix*(tilesize*BPP)+((xpix*BPP)+3)];
				}
			}
			/* copy from the tile buffer to the output */
			for (j=(yoff*tilesize), k=0; j<((yoff*tilesize)+tilesize); ++j, ++k) {
				memcpy((void*)&_out[j*((_w+_xpad)*BPP)+((xoff*tilesize)*BPP)], (void*)&buffer[k*(tilesize*BPP)], (tilesize*BPP));
			}
			/* shift to the next tile in the image */
			if (++xoff >= (_w / TILESIZE)) { xoff = 0, ++yoff; }
		}

		free(buffer);
	}
}
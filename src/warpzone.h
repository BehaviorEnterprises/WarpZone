
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <libgen.h>
#include <fcntl.h>

#define POINT(M,x,y)			(M)->data[(x) * (M)->h + y]
#define DISTANCE(a,b)		(a > b ? a - b : b - a)
#define MINIMUM(a,b,c)		(a < b ? (a < c ? a : c) : (b < c ? b : c))

#define COST_PLOT		0x0000000100000000
#define WARP_PLOT		0x0000000200000000
#define AXIS_PLOT		0x0000000400000000
#define ALL_PLOTS		0x0000000F00000000

// optimal warping path data structure?

typedef struct Series {
	uint64_t len;
	double *data;
} Series;

typedef struct Matrix {
	uint64_t w, h;
	double *data;
} Matrix;

typedef struct Warp {
	Series s1, s2;
	Matrix cost, cummulative;
	char *name;
	uint64_t flags;
} Warp;

/* warp.c */
Warp *warp_create_from_files(const char *, const char *);
int warp_caclulate_matrices(Warp *);
int warp_destroy(Warp **);

/* cairo.c */
int cairo_plot(Warp *);

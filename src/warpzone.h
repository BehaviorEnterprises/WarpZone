
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <libgen.h>
#include <fcntl.h>

#define POINT(M,x,y)			(M)->data[x * (M)->w + y]
#define DISTANCE(a,b)		(a > b ? a - b : b - a)
#define MINIMUM(a,b,c)		(a < b ? (a < c ? a : c) : (b < c ? b : c))

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
} Warp;

/* warp.c */
Warp *warp_create_from_files(const char *, const char *);
int warp_caclulate_matrices(Warp *);
int warp_destroy(Warp **);

/* cairo.c */
int cairo_plot(Warp *);

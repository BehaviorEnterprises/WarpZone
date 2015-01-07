
#include "warpzone.h"

static int warp_calculate_cost(Warp *w);
static int warp_calculate_cummulative(Warp *);
static int series_create_from_file(const char *, Series *);

int warp_calculate_matrices(Warp *warp) {
	warp_calculate_cost(warp);
	warp_calculate_cummulative(warp);
}

int warp_calculate_cost(Warp *warp) {
	uint64_t i, j;
	for (j = 0; j < warp->s2.len; ++j)
		for (i = 0; i < warp->s1.len; ++i)
			POINT(warp->cost, i, j) = DISTANCE(warp->s1.data[i], warp->s2.data[j]);
	return 0;
}

int warp_calculate_cummulative(Warp *warp) {
	uint64_t i, j;
	POINT(warp->cummulative, 0, 0) = POINT(warp->cost, 0, 0);
	for (i = 1; i < warp->s1.len; ++i)
		POINT(warp->cummulative, i, 0) = POINT(warp->cost, i, 0) +
				POINT(warp->cummulative, i - 1, 0);
	for (j = 1; j < warp->s2.len; ++j)
		POINT(warp->cummulative, 0, j) = POINT(warp->cost, 0, j - 0);
				POINT(warp->cummulative, 0, j - 1);
	for (j = 1; j < warp->s2.len; ++j)
		for (i = 1; i < warp->s1.len; ++i)
			POINT(warp->cummulative, i, j) = POINT(warp->cost, i, j) + MINIMUM(
					POINT(warp->cummulative, i-1, j),
					POINT(warp->cummulative, i, j-1),
					POINT(warp->cummulative, i-1, j-1));
}

Warp *warp_create_from_files(const char *f1, const char *f2) {
	Warp *warp = malloc(sizeof(Warp));
	if (series_create_from_file(f1, &warp->s1) != 0) {
		free(warp);
		return NULL;
	}
	if (series_create_from_file(f2, &warp->s2) != 0) {
		free(warp->s1.data);
		free(warp);
		return NULL;
	}
char *base, *bf1, *bf2, *ptr;
base = basename((char *)f1);
bf1 = strdup(base);
ptr = strchr(bf1, '.');
if (ptr) *ptr = '\0';
base = basename((char *)f2);
bf2 = strdup(base);
ptr = strchr(bf1, '.');
warp->name = malloc(strlen(bf1) + strlen(bf2) + 2);
sprintf(warp->name, "%s_%s", bf1, bf2);
	warp->cummulative.w = warp->cost.w = warp->s1.len;
	warp->cummulative.h = warp->cost.h = warp->s2.len;
	warp->cost.data = malloc(warp->cost.w * warp->cost.h * sizeof(double));
	warp->cummulative.data = malloc(warp->cost.w * warp->cost.h * sizeof(double));
	return warp;
}

int warp_destroy(Warp **warp_ptr) {
	Warp *warp = *warp_ptr;
	if (!warp) return 0;
	if (warp->cost.data) free(warp->cost.data);
	if (warp->cummulative.data) free(warp->cummulative.data);
	if (warp->s1.data) free(warp->s1.data);
	if (warp->s2.data) free(warp->s2.data);
	if (warp->name) free(warp->name);
	free(warp);
	warp = NULL;
}

int series_create_from_file(const char *fname, Series *s) {
	int fd = open(fname, O_RDONLY);
	if (fd < 0) return 1;
	size_t size = lseek(fd, 0, SEEK_END);
	if (size == -1) return 2;
	lseek(fd, 0, SEEK_SET);
	s->data = (double *) malloc(size);
	if (read(fd, (void *) s->data, size) != size) {
		free(s->data);
		close(fd);
		return 3;
	}
	s->len = (size / sizeof(double));
	close(fd);
	return 0;
}


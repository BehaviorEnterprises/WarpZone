
#include "warpzone.h"
#include "cairo.h"

typedef struct Mask {
	cairo_surface_t *surface;
	unsigned char *data;
} Mask;

static int cairo_create_plot(Matrix *, const char *, const char *);
static Mask *mask_create(Matrix *m);
static int mask_destroy(Mask **);

Mask *mask_create(Matrix *m) {
	Mask *mask = malloc(sizeof(Mask));
	uint64_t i, j;
	int stride;
	char *a;
	double max = 0.0;
	for (j = 0; j < m->h; ++j)
		for (i = 0; i < m->w; ++i)
			if (max < POINT(m, i, j)) max = POINT(m, i, j);
	stride = cairo_format_stride_for_width(CAIRO_FORMAT_A8, m->w);
	mask->data = (unsigned char *) calloc(stride, m->h);
	for (j = 0; j < m->h; ++j) {
		a = mask->data + j * stride;
		for (i = 0; i < m->w; ++i, ++a)
			*a = (unsigned char) (255.0 * POINT(m, i, j) / max);
	}
	mask->surface = cairo_image_surface_create_for_data(mask->data,
			CAIRO_FORMAT_A8, m->w, m->h, stride);
	return mask;
}

int mask_destroy(Mask **mask_ptr) {
	Mask *mask = *mask_ptr;
	if (!mask) return 1;
	if (mask->surface) cairo_surface_destroy(mask->surface);
	if (mask->data) free(mask->data);
	free(mask);
	mask = NULL;
}

int cairo_create_plot(Matrix *m, const char *base, const char *ext) {
	cairo_surface_t *img;
	Mask *mask = mask_create(m);
	img = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, m->w, m->h);
	cairo_t *ctx = cairo_create(img);
	cairo_set_source_rgba(ctx, 1.0, 1.0, 1.0, 1.0);
	cairo_rectangle(ctx, 0, 0, m->w, m->h);
	cairo_fill(ctx);
	cairo_set_source_rgba(ctx, 1.0, 1.0, 1.0, 1.0);
	cairo_mask_surface(ctx, mask->surface, 0, 0);
	char *fname = malloc(strlen(base) + strlen(ext) + 2);
	strcpy(fname, base);
	strcat(fname, ext);
	cairo_surface_write_to_png(img, fname);
	free(fname);
	cairo_destroy(ctx);
	cairo_surface_destroy(img);
	mask_destroy(&mask);
	return 0;
}

int cairo_plot(Warp *warp) {
	cairo_create_plot(&warp->cost, warp->name, "_cost.png");
	cairo_create_plot(&warp->cummulative, warp->name, "_cummulative.png");
	return 0;
}

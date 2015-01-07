
#include "warpzone.h"
#include "cairo.h"

typedef struct Mask {
	cairo_surface_t *surface;
	unsigned char *data;
} Mask;

static int cairo_create_plot(Matrix *, const char *, const char *);
static Mask *mask_create(Matrix *m);
static int mask_destroy(Mask **);

cairo_surface_t *create_trace(Series *s, uint64_t h) {
	cairo_surface_t *trace;
	uint64_t i;
	double min, max, range;
	trace = cairo_image_surface_create(CAIRO_FORMAT_A8, s->len, h);
	min = s->data[0]; max = s->data[0];
	for (i = 1; i < s->len; ++i) {
		if (s->data[i] < min) min = s->data[i];
		if (s->data[i] > max) max = s->data[i];
	}
	cairo_t *ctx = cairo_create(trace);
	cairo_scale(ctx, 1.0, h / (max - min));
	cairo_translate(ctx, 0, min);
	cairo_move_to(ctx, 0, s->data[0]);
	for (i = 1; i < s->len; ++i)
		cairo_line_to(ctx, i, s->data[i]);
	cairo_stroke(ctx);
	cairo_destroy(ctx);
	return trace;
}

int create_frame(Warp *warp) {
	cairo_surface_t *img;
	uint64_t w, h, margin;
	margin = (warp->s1.len + warp->s2.len) / 10.0;
	w = warp->s1.len + margin + 1;
	h = warp->s2.len + margin + 1;


	img = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
	cairo_t *ctx = cairo_create(img);
	cairo_scale(ctx, 1.0, -1.0);
	cairo_translate(ctx, 0, -1.0 * h);
	cairo_set_source_rgba(ctx, 0.0, 0.0, 1.0, 1.0);
	cairo_surface_t *trace;
	trace = create_trace(&warp->s1, margin);
	cairo_mask_surface(ctx, trace, margin, 0);
	cairo_surface_destroy(trace);
	cairo_rotate(ctx, M_PI / 2.0);
	trace = create_trace(&warp->s2, margin);
	cairo_mask_surface(ctx, trace, margin, -1.0 * margin);
	cairo_surface_destroy(trace);

	char *fname = malloc(strlen(warp->name) + 11);
	strcpy(fname, warp->name);
	strcat(fname, "_frame.png");

	cairo_surface_write_to_png(img, fname);
	cairo_surface_destroy(img);
	cairo_destroy(ctx);

	free(fname);
	return 0;
}




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
	cairo_scale(ctx, 1.0, -1.0);
	cairo_translate(ctx, 0, -1.0 * m->h);
	cairo_set_source_rgba(ctx, 0.0, 0.0, 0.0, 1.0);
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
	if (warp->flags & COST_PLOT)
		cairo_create_plot(&warp->cost, warp->name, "_cost.png");
	if (warp->flags & WARP_PLOT)
		cairo_create_plot(&warp->cummulative, warp->name, "_warp.png");
	if (warp->flags & AXIS_PLOT)
		create_frame(warp);
	return 0;
}

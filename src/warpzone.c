
#include "warpzone.h"

int main(int argc, const char **argv) {
	if (argc < 3) return 1;
	int a, b;
	Warp *warp;
	uint64_t flags = 0;
	for (a = 1; argv[a][0] == '-' && a < argc; ++a) {
		for (b = 1; argv[a][b] != '\0'; ++b) switch (argv[a][b]) {
			case 'h': /* help(); */ break;
			case 'v': /* version(); */ break;
			case 'w': flags |= WARP_PLOT; break;
			case 'c': flags |= COST_PLOT; break;
			case 'f': flags |= AXIS_PLOT; break;
			case 'p': flags |= ALL_PLOTS; break;
			case '-': break;
			default: fprintf(stderr, "ignored unknown flag '%c'\n", argv[a][b]);
		}
	}
	for (a = a; a < argc - 1; ++a) {
		for (b = a + 1; b < argc; ++b) {
			warp = warp_create_from_files(argv[a], argv[b]);
			if (warp) {
				warp->flags = flags;
				warp_calculate_matrices(warp);
				cairo_plot(warp);
				printf("%s: %E\n", warp->name, POINT(&warp->cummulative,
						warp->cummulative.w - 1, warp->cummulative.h - 1));
				warp_destroy(&warp);
			}
			else {
				printf("%s: Nan\n");
			}
		} /* 'b' file loop */
	} /* 'a' file loop */
	return 0;
}

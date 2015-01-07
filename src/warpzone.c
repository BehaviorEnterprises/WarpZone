
#include "warpzone.h"

int main(int argc, const char **argv) {
	if (argc < 3) return 1;
	int a, b;
	Warp *warp;
	for (a = 1; a < argc - 1; ++a) {
		for (b = a + 1; b < argc; ++b) {
			warp = warp_create_from_files(argv[a], argv[b]);
			warp_calculate_matrices(warp);
			cairo_plot(warp);
			// TODO plot and save data
			warp_destroy(&warp);
		}
	}
	return 0;
}

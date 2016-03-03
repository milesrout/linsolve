#ifndef LINSOLVE_H
#define LINSOLVE_H

#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct matrix {
	size_t num_rows, num_cols;
	double complex data[];
};

double complex get(struct matrix *m, size_t row, size_t col);
void set(struct matrix *m, size_t row, size_t col, double complex d);
struct matrix *scan_matrix();
void print_matrix(FILE *stream, const char *name, struct matrix *m);

#endif /* LINSOLVE_H */

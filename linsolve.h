#ifndef LINSOLVE_H
#define LINSOLVE_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct matrix {
	size_t num_rows, num_cols;
	double data[];
};

double get(struct matrix *m, size_t row, size_t col);
void set(struct matrix *m, size_t row, size_t col, double d);
struct matrix *scan_matrix();
void print_matrix(FILE *stream, const char *name, struct matrix *m);

#endif /* LINSOLVE_H */

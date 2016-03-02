#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct matrix {
	size_t num_rows, num_cols;
	double data[];
};

static double get(struct matrix *m, size_t row, size_t col)
{
	return *(m->data + row * m->num_cols + col);
}

static struct matrix *scan_matrix()
{
	size_t i, num_rows, num_cols, size;
	struct matrix *m;

	scanf("%lu %lu", &num_rows, &num_cols);
	size = num_rows * num_cols;

	m = malloc(sizeof(size_t)*2 + sizeof(double)*size);
	if (m == NULL) abort();
	m->num_rows = num_rows;
	m->num_cols = num_cols;

	for (i = 0; i < size; i++) {
		scanf("%lf", m->data + i);
	}

	return m;
}

int main()
{
	struct matrix *A, *b;
	int i, j;

	A = scan_matrix();
	b = scan_matrix();

	for (i = 0; i < A->num_rows; i++) {
		for (j = 0; j < A->num_cols; j++) {
			printf("%lf ", get(A, i, j));
		}
		puts("");
	}
	for (i = 0; i < b->num_rows; i++) {
		for (j = 0; j < b->num_cols; j++) {
			printf("%lf ", get(b, i, j));
		}
		puts("");
	}

	free(A);
	free(b);

	return 0;
}

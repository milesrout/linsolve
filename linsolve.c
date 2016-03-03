#include "linsolve.h"

#define exit_if_incompatible(a, b) exit_if((a)->num_cols != (b)->num_rows, "incompatible dimensions")
#define exit_if_invalid_row(a, i) exit_if((i) >= (a)->num_rows, "row out of bounds")
#define exit_if_zero(a) exit_if((a) == 0.0, "matrix is singular")
#define exit_if(condition, message) do { if (condition) {\
	fprintf(stderr, "%s\n", message);\
	exit(EXIT_FAILURE);\
} } while (0)
#define exits(m, v, r1, r2)\

void swap_rows(struct matrix *m, struct matrix *v, size_t r1, size_t r2)
{
	size_t i;
	double temp;

	exit_if_incompatible(m, v);
	exit_if_invalid_row(m, r1);
	exit_if_invalid_row(m, r2);

	temp = get(v, r1, 0);
	set(v, r1, 0, get(v, r2, 0));
	set(v, r2, 0, temp);
	for (i = 0; i < m->num_cols; i++) {
		temp = get(m, r1, i);
		set(m, r1, i, get(m, r2, i));
		set(m, r2, i, temp);
	}
}

void scale_row(struct matrix *m, struct matrix *v, size_t row, double factor)
{
	size_t i;

	exit_if_incompatible(m, v);
	exit_if_invalid_row(m, row);

	set(v, row, 0, factor * get(v, row, 0));
	for (i = 0; i < m->num_cols; i++) {
		set(m, row, i, factor * get(m, row, i));
	}
}

void add_multiple(struct matrix *m, struct matrix *v, size_t r1, size_t r2, double factor)
{
	size_t i;

	exit_if_incompatible(m, v);
	exit_if_invalid_row(m, r1);
	exit_if_invalid_row(m, r2);

	set(v, r1, 0, get(v, r1, 0) + factor * get(v, r2, 0));
	for (i = 0; i < m->num_cols; i++) {
		set(m, r1, i, get(m, r1, i) + factor * get(m, r2, i));
	}
}

void partial_pivot(struct matrix *m, struct matrix *v, size_t j)
{
	double best_val = 0.0;
	size_t i, best_idx;

	for (i = j; i < m->num_rows; i++) {
		double candidate = fabs(get(m, i, j));
		if (candidate > best_val) {
			best_val = candidate;
			best_idx = i;
		}
	}

	exit_if_zero(best_val);

	swap_rows(m, v, best_idx, j);
}

void row_reduction(struct matrix *m, struct matrix *v)
{
	size_t i, j, max;
	double a, b;

	if (m->num_cols > m->num_rows) {
		max = m->num_cols;
	} else {
		max = m->num_rows;
	}
	for (j = 0; j < max; j++) {
		partial_pivot(m, v, j);
		a = get(m, j, j);
		for (i = j + 1; i < m->num_rows; i++) {
			b = get(m, i, j);
			add_multiple(m, v, i, j, -b/a);
		}
	}
}

void back_substitution(struct matrix *m, struct matrix *v)
{
	int i, j;
	double a, b;

	for (j = m->num_cols - 1; j >= 1; j--) {
		a = get(m, j, j);
		for (i = j - 1; i >= 0; i--)  {
			b = get(m, i, j);
			add_multiple(m, v, i, j, -b/a);
		}
		scale_row(m, v, j, 1.0/a);
	}
}

void gauss_jordan(struct matrix *m, struct matrix *v)
{
	row_reduction(m, v);
	if (0) { print_matrix(stdout, "A", m); print_matrix(stdout, "b", v); }
	back_substitution(m, v);
}

int main()
{
	struct matrix *A, *b;
	int i, j;

	A = scan_matrix();
	b = scan_matrix();
	
	if (b->num_rows != A->num_cols) {
		fprintf(stderr, "%s\n", "A and b must have compatible dimensions.");
		exit(EXIT_FAILURE);
	}
	if (b->num_cols != 1) {
		fprintf(stderr, "%s\n", "b must be a column vector.");
		exit(EXIT_FAILURE);
	}

	gauss_jordan(A, b);

	for (i = 0; i < b->num_rows; i++) {
		printf("%lf\n", get(b, i, 0));
	}

	free(A);
	free(b);

	return 0;
}

void print_matrix(FILE *stream, const char *name, struct matrix *m)
{
	size_t i, j;
	printf("%s = ", name);
	for (i = 0; i < m->num_rows; i++) {
		printf("\t");
		for (j = 0; j < m->num_cols; j++) {
			printf("%lf ", get(m, i, j));
		}
		printf("\n");
	}
}

#define make_ref(m, row, col) ((m)->data + (row) * (m)->num_cols + (col))
double get(struct matrix *m, size_t row, size_t col)
{
	return *make_ref(m, row, col);
}
void set(struct matrix *m, size_t row, size_t col, double d)
{
	*make_ref(m, row, col) = d;
}

struct matrix *scan_matrix()
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

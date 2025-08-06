#include <stdio.h>

void idx_to_row_col(char *buff, int idx,  int *row, int *col)
{
	int	r;
	int i;
	int	c;

	r = 1;
	c = 1;
	i = 0;
	while (i < idx && buff)
	{
		c++;
		if (buff[i] == '\n')
		{
			c = 1;
			r++;
		}
		i++;
	}
	if (row)
		*row = r;
	if (col)
		*col = c;
}

int row_col_to_idx(char *buff, int row, int col)
{
	int	r;
	int	c;
	int	i;

	if (row <= 0)
		row = 1;
	if (col <= 0)
		col = 1;
	r = 1;
	c = 1;
	i = 0;
	while ((r < row || c < col) && buff)
	{
		c++;
		if (buff[i] == '\n')
		{
			c = 1;
			r++;
		}
		i++;
	}
	return (i);
}

void print_line_highlight_range_col(char *buff, int start, int len, int ascii_color)
{
	int	row;
	int	i;
	int	curr;
	int start_idx;

	idx_to_row_col(buff, start, &row, NULL);
	i = row_col_to_idx(buff, row, 0);
	start_idx = 0;
	curr = printf("%4i | ", row);
	while (buff && buff[i] && buff[i] != '\n') {
		if (!start_idx && i >= start)
			start_idx = curr;
		if (i >= start && i - start < len)
			printf("\033[%im\033[1m\033[3m\033[4m", ascii_color);
		if (buff[i] == '\t')
			curr += printf("    ");
		else
			curr += printf("%c", buff[i]);
		if (i >= start && i - start <= len)
			printf("\033[0m");
		i++;
	}
	printf("\n");
	i = printf("     | ");
	while (++i <= start_idx)
		printf(" ");
	printf("\033[%im", ascii_color);
	printf("^");
	while (++i <= len + start_idx)
		printf("~");
	printf("\033[0m");
	printf("\n");
}

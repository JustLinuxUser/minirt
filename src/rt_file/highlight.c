#include "rt_tokenizer.h"
#include <stdio.h>
#include "errors.h"
#include "../libft/libft.h"

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
char *warn_level_to_string(enum e_RT_WARN_LEVEL w)
{
	if (w == RT_HELP) return "HELP";
	if (w == RT_INFO) return "INFO";
	if (w == RT_WARN) return "WARN";
	if (w == RT_ERROR) return "ERROR";
	ft_assert("Unreachable" == 0);
	return 0;
}

void print_warn_lvl(enum e_RT_WARN_LEVEL level)
{
	ft_printf("\033[%im"ANSI_BOLD"[%s]: ", level, warn_level_to_string(level));
}

void print_line_highlight_range_col(t_error err)
{
	int	row;
	int	col;
	int	i;
	int	curr;
	int	start_idx;

	ft_printf(ANSI_RESET);
	idx_to_row_col(err.file->contents.buff, err.start_idx, &row, &col);
	ft_printf(ANSI_DIMM ANSI_ITALIC ANSI_UNDERLINE "%s:%i:%i:\033\n" ANSI_RESET, err.file->fname, row, col);
	i = row_col_to_idx(err.file->contents.buff, row, 0);
	start_idx = 0;
	curr = ft_printf("%4i | ", row);
	while (err.file->contents.buff && err.file->contents.buff[i]
		&& err.file->contents.buff[i] != '\n') {
		if (i <= err.start_idx)
			start_idx = curr;
		if (i >= err.start_idx && i - err.start_idx < err.len)
			ft_printf("\033[%im"ANSI_BOLD ANSI_UNDERLINE ANSI_ITALIC, err.warn_level);
		if (err.file->contents.buff[i] == '\t')
			curr += ft_printf("    ");
		else
			curr += ft_printf("%c", err.file->contents.buff[i]);
		if (i >= err.start_idx && i - err.start_idx <= err.len)
			ft_printf(ANSI_RESET);
		i++;
	}
	ft_printf("\n");
	i = ft_printf("     | ");
	while (++i <= start_idx)
		ft_printf(" ");
	ft_printf("\033[%im", err.warn_level);
	ft_printf("^");
	while (++i <= err.len + start_idx)
		ft_printf("~");
	ft_printf("\033[0m");
	ft_printf("\n");
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   highlight.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 08:47:20 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/02 08:47:20 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_tokenizer.h"
#include "errors.h"
#include "../libft/ft_printf/ft_printf.h"
#include "../error.h"

void	print_warn_lvl(enum e_RT_WARN_LEVEL level)
{
	print_err();
	ft_eprintf("\033[%im"ANSI_BOLD"[%s]: ", level, warn_level_to_string(level));
}

static void	print_line_highlight_prelude(t_error err, int *i, int *curr)
{
	int	row;
	int	col;

	ft_eprintf(ANSI_RESET);
	idx_to_row_col(err.file->contents.buff, err.start_idx, &row, &col);
	ft_eprintf(ANSI_DIMM ANSI_ITALIC ANSI_UNDERLINE "%s:%i:%i:\n" ANSI_RESET,
		err.file->fname, row, col);
	*i = row_col_to_idx(err.file->contents.buff, row, 0);
	*curr = ft_eprintf("%4i | ", row);
}

static void	print_line_squiggles(t_error err, int i, int start_idx)
{
	ft_eprintf("\n");
	i = ft_eprintf("     | ");
	while (++i <= start_idx)
		ft_eprintf(" ");
	ft_eprintf("\033[%im", err.warn_level);
	ft_eprintf("^");
	while (++i <= err.len + start_idx)
		ft_eprintf("~");
	ft_eprintf("\033[0m");
	ft_eprintf("\n");
}

void	print_line_highlight_range_col(t_error err)
{
	int	i;
	int	curr;
	int	start_idx;

	start_idx = 0;
	print_line_highlight_prelude(err, &i, &curr);
	while (err.file->contents.buff && err.file->contents.buff[i]
		&& err.file->contents.buff[i] != '\n')
	{
		if (i <= err.start_idx)
			start_idx = curr;
		if (i >= err.start_idx && i - err.start_idx < err.len)
			ft_eprintf("\033[%im"ANSI_BOLD ANSI_UNDERLINE ANSI_ITALIC,
				err.warn_level);
		if (err.file->contents.buff[i] == '\t')
			curr += ft_eprintf("    ");
		else
			curr += ft_eprintf("%c", err.file->contents.buff[i]);
		if (i >= err.start_idx && i - err.start_idx <= err.len)
			ft_eprintf(ANSI_RESET);
		i++;
	}
	print_line_squiggles(err, i, start_idx);
}

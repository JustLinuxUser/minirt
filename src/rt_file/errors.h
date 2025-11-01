/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 18:23:52 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/01 18:23:52 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H
# include "rt_tokenizer.h"

# define ANSI_NUM_RED 		31
# define ANSI_NUM_GREEN 	32
# define ANSI_NUM_YELLOW	33
# define ANSI_NUM_BLUE 		34

# define ANSI_RESET			"\033[0m"
# define ANSI_BOLD			"\033[1m"
# define ANSI_DIMM			"\033[2m"
# define ANSI_ITALIC		"\033[3m"
# define ANSI_UNDERLINE		"\033[4m"

# define ANSI_RED			"\033[31m"
# define ANSI_GREEN			"\033[32m"
# define ANSI_YELLOW		"\033[33m"
# define ANSI_BLUE			"\033[34m"

enum e_RT_WARN_LEVEL
{
	RT_HELP = ANSI_NUM_BLUE,
	RT_INFO = ANSI_NUM_GREEN,
	RT_WARN = ANSI_NUM_YELLOW,
	RT_ERROR = ANSI_NUM_RED,
};

typedef struct t_error
{
	t_file						*file;
	int							start_idx;
	int							len;
	enum e_RT_WARN_LEVEL		warn_level;
}	t_error;

void	print_warn_lvl(enum e_RT_WARN_LEVEL level);
void	print_line_highlight_range_col(t_error err);
#endif

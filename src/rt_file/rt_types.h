/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_types.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 19:28:41 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/01 19:28:41 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_TYPES_H
# define RT_TYPES_H
# include "rt_parser.h"

enum e_RT_NODE_TYPE	effective_type(enum e_RT_NODE_TYPE target,
						enum e_RT_NODE_TYPE got);
bool				rt_type_cmp(enum e_RT_NODE_TYPE target,
						enum e_RT_NODE_TYPE got);
char				*type_to_str(enum e_RT_NODE_TYPE t);
#endif

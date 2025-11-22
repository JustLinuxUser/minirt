NAME := libft.a

UNAME_S := $(shell uname -s)
SHELL := bash

SOURCES := src/ft_atoi.c        \
		   src/ft_checked_atoi.c\
		   src/ft_bzero.c       \
		   src/ft_calloc.c      \
		   src/ft_isalnum.c     \
		   src/ft_isalpha.c     \
		   src/ft_isascii.c     \
		   src/ft_isdigit.c     \
		   src/ft_isprint.c     \
		   src/ft_itoa.c        \
		   src/ft_memchr.c      \
		   src/ft_memcmp.c      \
		   src/ft_memcpy.c      \
		   src/ft_memmove.c     \
		   src/ft_memset.c      \
		   src/ft_putchar_fd.c  \
		   src/ft_putendl_fd.c  \
		   src/ft_putnbr_fd.c   \
		   src/ft_putstr_fd.c   \
		   src/ft_split.c       \
		   src/ft_split_str.c   \
		   src/ft_strchr.c      \
		   src/ft_strnchr.c     \
		   src/ft_strrchr.c     \
		   src/ft_strdup.c      \
		   src/ft_striteri.c    \
		   src/ft_strjoin.c     \
		   src/ft_strlcat.c     \
		   src/ft_strlcpy.c     \
		   src/ft_strlen.c      \
		   src/ft_strmapi.c     \
		   src/ft_strncmp.c     \
		   src/ft_strcmp.c      \
		   src/ft_strnstr.c     \
		   src/str_ends_with.c  \
		   src/ft_strtrim.c     \
		   src/ft_substr.c      \
		   src/ft_tolower.c     \
		   src/ft_toupper.c     \
		   src/ft_memswap.c

SOURCES += src/ft_lstadd_back_bonus.c\
		   src/ft_lstadd_front_bonus.c\
		   src/ft_lstclear_bonus.c\
		   src/ft_lstdelone_bonus.c\
		   src/ft_lstiter_bonus.c\
		   src/ft_lstlast_bonus.c\
		   src/ft_lstmap_bonus.c\
		   src/ft_lstnew_bonus.c\
		   src/ft_lstsize_bonus.c

SOURCES +=	gnl/get_next_line.c\
			gnl/get_next_line_utils.c

SOURCES +=	ft_printf/hex_utils.c\
			ft_printf/parser.c\
			ft_printf/printf.c\
			ft_printf/fdprintf.c\
			ft_printf/eprintf.c\
			ft_printf/printf_hex.c\
			ft_printf/printf_int.c\
			ft_printf/printf_ptr.c\
			ft_printf/ptr_utils.c\
			ft_printf/signed_int_utils.c\
			ft_printf/writer2.c\
			ft_printf/writer.c\
			ft_printf/printf_str.c
		   
SOURCES +=	utils/string_utils.c\
			utils/math_utils.c\
			utils/math_utils2.c\
			utils/misc.c


SOURCES +=	dsa/dyn_str.c\
			dsa/vec_gen.c\
			dsa/dyn_str.c\
			dsa/dyn_str2.c\
			dsa/dyn_str3.c\
			dsa/deque_int2.c\
			dsa/deque_int.c\
			dsa/deque_char.c\
			dsa/deque_char2.c\
			dsa/prng.c\
			dsa/xorshift.c\
			dsa/xoroshiro128plusplus.c\
			dsa/vec_dyn_str.c\
			dsa/smoothsort.c\
			dsa/smoothsort2.c\
			dsa/partition.c

SOURCES +=	alloc/mmalloc.c \
			alloc/alloc.c   \
			alloc/alloc2.c

GEN_NAMES += 							int
GEN_TYPES_$(lastword $(GEN_NAMES)) = 	int

GEN_NAMES += 							fvec3
GEN_TYPES_$(lastword $(GEN_NAMES)) =	t_fvec3

GEN_NAMES += 							obj_token
GEN_TYPES_$(lastword $(GEN_NAMES)) =	t_obj_token

GEN_NAMES += 							outcome
GEN_TYPES_$(lastword $(GEN_NAMES)) =	t_outcome

GEN_NAMES += 							light
GEN_TYPES_$(lastword $(GEN_NAMES)) =	t_light

GEN_NAMES += 							float
GEN_TYPES_$(lastword $(GEN_NAMES)) =	float

GEN_NAMES += 							triangle
GEN_TYPES_$(lastword $(GEN_NAMES)) =	t_triangle

GEN_NAMES += 							  sphere
GEN_TYPES_$(lastword $(GEN_NAMES)) =	t_sphere

GEN_NAMES += 							  plane
GEN_TYPES_$(lastword $(GEN_NAMES)) =	t_plane

GEN_NAMES += 							  mesh
GEN_TYPES_$(lastword $(GEN_NAMES)) =	t_mesh

GEN_NAMES += 							  cylinder
GEN_TYPES_$(lastword $(GEN_NAMES)) =	t_cylinder

GEN_NAMES += 							  shape
GEN_TYPES_$(lastword $(GEN_NAMES)) =	t_shape

GEN_NAMES += 							  densely_sampled_spectrum
GEN_TYPES_$(lastword $(GEN_NAMES)) =	t_densely_sampled_spectrum

GEN_NAMES += 							  alias_bin
GEN_TYPES_$(lastword $(GEN_NAMES)) =	t_alias_bin

BUILD_DIR := build
CODEGEN_DIR := generated

ifndef PROFILE
	PROFILE=debug
endif

TAG_FILE := $(BUILD_DIR)/profile_$(PROFILE)
NAME_PROFILE := $(BUILD_DIR)/$(PROFILE)_$(NAME)
PROFILES := opt debug debug_mem debug_mem_sanitize debug_mem_sanitize_mem

CFLAGS := -MMD -fPIE -Wall -Wextra -Werror --std=c99 -pedantic -g3 -fno-builtin $(FLAGS)

ifeq ($(PROFILE),opt)
	CFLAGS += -O3
endif

ifeq ($(PROFILE),debug)
	CFLAGS += -Og
endif

ifeq ($(PROFILE),debug_mem)
	CFLAGS += -DDEBUG_ALLOC
endif

ifeq ($(PROFILE),debug_mem_sanitize)
	ifeq ($(UNAME_S),Darwin)
		CFLAGS += -DDEBUG_ALLOC -fsanitize=address
	else
		CFLAGS += -DDEBUG_ALLOC -fsanitize=address,leak,undefined
	endif
endif
ifeq ($(PROFILE),debug_mem_sanitize_mem)
	CFLAGS += -DDEBUG_ALLOC -fsanitize-recover=all -fsanitize=memory,undefined -fsanitize-memory-track-origins -fPIE -fno-omit-frame-pointer -O1
endif

PROFILE_FOUND := $(filter $(PROFILE), $(PROFILES))

define NL


endef

$(if ${PROFILE_FOUND}, , $(error Unknown profile" ${PROFILE}${NL}Choose one of: $(PROFILES)))

OBJS := $(SOURCES:%.c=$(BUILD_DIR)/$(PROFILE)/%.o)

GEN_HEADERS		:= $(GEN_NAMES:%=$(CODEGEN_DIR)/vec_%.h)

ifeq ($(PROFILE_FOUND),)
error::
	@echo "Unknown profile" ${PROFILE};
	@echo "Choose one of: $(PROFILES)";
	@exit 1;
endif

all: $(NAME)

$(NAME): $(NAME_PROFILE) ${TAG_FILE}
	cp $(NAME_PROFILE) $(NAME)

generate: $(GEN_HEADERS)

$(NAME_PROFILE): $(OBJS)
	ar -rsc $(NAME_PROFILE) $(OBJS)

${CODEGEN_DIR}/vec_%.h: templates/vec_template.hpp Makefile
	mkdir -p $(dir $@)

	rm -f $@

	echo '/* ************************************************************************** */' >>   $@
	echo '/*                                                                            */' >>   $@
	echo '/*                                                        :::      ::::::::   */' >>   $@
	printf '/*   %-20s                               :+:      :+:    :+:   */\n' $${FILE_NAME} >>$@
	echo '/*                                                    +:+ +:+         +:+     */' >>   $@
	echo '/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */' >>   $@
	echo '/*                                                +#+#+#+#+#+   +#+           */' >>   $@
	echo '/*   Created: 2024/09/28 15:11:48 by anddokhn          #+#    #+#             */' >>   $@
	echo '/*   Updated: 2025/04/08 22:26:02 by anddokhn         ###   ########.fr       */' >>   $@
	echo '/*                                                                            */' >>   $@
	echo '/* ************************************************************************** */' >>   $@

	PROT_NAME=$(shell echo $(notdir $@) | tr . '_' | tr '[:lower:]' '[:upper:]') &&\
		echo '#ifndef '$$PROT_NAME >> $(abspath $@) &&\
		echo '# define '$$PROT_NAME >> $(abspath $@)
	echo '# include "../dsa/vec_gen.h"' >> $(abspath $@)
	echo '# include "../utils/utils.h"' >> $(abspath $@)

	type_len=$$(echo -n $(GEN_TYPES_$(@:$(CODEGEN_DIR)/vec_%.h=%)) | wc -c)\
		num_tabs=$$(echo $$type_len / 4 + 1 | bc) \
			&& str="$$(printf "%$${num_tabs}s")" && size_t_tabs=$$(echo -n $${str// /\\t}) \
																					\
	&& cd templates/ && expand -i $(abspath $<) |\
		gcc -E - -DAUTOGENERATING -DGEN_TYPE=$(GEN_TYPES_$(@:$(CODEGEN_DIR)/vec_%.h=%)) -DGEN_SHORT_TYPE=$(@:$(CODEGEN_DIR)/vec_%.h=%) |\
		unexpand | sed '1,/START/d' |\
		sed -E "s/\s*SIZE_T_TAB\s*/$$size_t_tabs/g" | \
		sed -E "s/\s*TYPE_TAB\s*/\t\t/g" | \
		sed -E "s/\s*TAB\s*/\t/g" >> $(abspath $@)

	echo '#endif' >> $(abspath $@)
	cat $(abspath $@)
	if ! command -v norminette >/dev/null 2>&1; then \
		norminette $(abspath $@); \
	fi

${TAG_FILE}:
	rm -rf $(BUILD_DIR)/profile_*
	touch ${TAG_FILE}

$(BUILD_DIR)/$(PROFILE)/%.o: %.c Makefile
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -rf $(NAME)

re::fclean
re::all

.PHONY: all clean fclean re generate
-include $(SOURCES:%.c=$(BUILD_DIR)/$(PROFILE)/%.d)

NAME := libft.a
CC := cc

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
			dsa/deque_int2.c\
			dsa/deque_int.c\
			dsa/deque_char.c\
			dsa/deque_char2.c\
			dsa/prng.c\
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

BUILD_DIR := build
CODEGEN_DIR := generated

ifndef PROFILE
	PROFILE=debug
endif

TAG_FILE := $(BUILD_DIR)/profile_$(PROFILE)
NAME_PROFILE := $(BUILD_DIR)/$(PROFILE)_$(NAME)
PROFILES := opt debug debug_mem debug_mem_sanitize

CFLAGS := -MMD -fPIE -Wall -Wextra -Werror --std=c99 -pedantic -g3 $(FLAGS)

ifeq ($(PROFILE),opt)
	CFLAGS += -O3 -flto
endif

ifeq ($(PROFILE),debug)
	CFLAGS += -Og
endif

ifeq ($(PROFILE),debug_mem)
	CFLAGS += -DDEBUG_ALLOC
endif

ifeq ($(PROFILE),debug_mem_sanitize)
	CFLAGS += -DDEBUG_ALLOC -fsanitize=address,leak,undefined
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


$(NAME_PROFILE): $(OBJS) $(GEN_HEADERS)
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


	cd templates/ && expand -i $< |\
		gcc -E - -DGEN_TYPE=$(GEN_TYPES_$(@:$(CODEGEN_DIR)/vec_%.h=%)) -DGEN_SHORT_TYPE=$(@:$(CODEGEN_DIR)/vec_%.h=%) |\
		unexpand | sed '1,/START/d' |\
		sed $$'s/\\s*TAB\\s*/\t/g' \
			>> $(abspath $@)

	echo '#endif' >> $(abspath $@)
	cat $(abspath $@)
	norminette $(abspath $@)

${TAG_FILE}:
	rm -rf $(BUILD_DIR)/profile_*
	touch ${TAG_FILE}

$(BUILD_DIR)/$(PROFILE)/%.o: %.c Makefile
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(CODEGEN_DIR)

fclean: clean
	rm -rf $(NAME)

re::fclean
re::all

.PHONY: clean fclean re
-include $(SOURCES:%.c=$(BUILD_DIR)/$(PROFILE)/%.d)

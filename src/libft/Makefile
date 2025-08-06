NAME := libft.a

SOURCES := src/ft_atoi.c\
		   src/ft_checked_atoi.c\
		   src/ft_bzero.c\
		   src/ft_calloc.c\
		   src/ft_isalnum.c\
		   src/ft_isalpha.c\
		   src/ft_isascii.c\
		   src/ft_isdigit.c\
		   src/ft_isprint.c\
		   src/ft_itoa.c\
		   src/ft_memchr.c\
		   src/ft_memcmp.c\
		   src/ft_memcpy.c\
		   src/ft_memmove.c\
		   src/ft_memset.c\
		   src/ft_putchar_fd.c\
		   src/ft_putendl_fd.c\
		   src/ft_putnbr_fd.c\
		   src/ft_putstr_fd.c\
		   src/ft_split.c\
		   src/ft_split_str.c\
		   src/ft_strchr.c\
		   src/ft_strnchr.c\
		   src/ft_strrchr.c\
		   src/ft_strdup.c\
		   src/ft_striteri.c\
		   src/ft_strjoin.c\
		   src/ft_strlcat.c\
		   src/ft_strlcpy.c\
		   src/ft_strlen.c\
		   src/ft_strmapi.c\
		   src/ft_strncmp.c\
		   src/ft_strcmp.c\
		   src/ft_strnstr.c\
		   src/ft_strtrim.c\
		   src/ft_substr.c\
		   src/ft_tolower.c\
		   src/ft_toupper.c
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
			dsa/vec_int.c\
			dsa/vec_int.c\
			dsa/dyn_str.c\
			dsa/dyn_str2.c\
			dsa/deque_int2.c\
			dsa/deque_int.c\
			dsa/deque_char.c\
			dsa/deque_char2.c\
			dsa/prng.c\
			dsa/vec_dyn_str.c



HEADERS := libft.h\
		   gnl/get_next_line.h\
		   ft_printf/ft_printf.h\
		   utils/utils.h\
		   dsa/vec_int.h\
		   dsa/dyn_str.h\
		   dsa/deque_int.h\
		   dsa/deque_char.h\
		   dsa/vec_dyn_str.h

OBJS := ${SOURCES:%.c=build/%.o}
BUILD_DIR := build

ifdef OPT
	CFLAGS := -fPIE -Wall -Wextra -Werror -O3 -flto ${FLAGS}
else
	CFLAGS := -Wall -Wextra -Werror -fPIE --std=c99 -pedantic -g3 -fsanitize=address,leak,undefined ${FLAGS}
endif

CC := cc

all: ${NAME} 

$(NAME): Makefile ${OBJS}
	ar -rsc ${NAME} ${OBJS}

build/%.o: %.c ${HEADERES} Makefile
	@mkdir -p build/$$(dirname $<)
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -rf ${BUILD_DIR}

fclean: clean
	rm -rf ${NAME}

re::fclean
re::all

.PHONY: clean fclean re

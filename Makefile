TARGET := minirt

SRCS_DIR := .
OUT_DIR := build

SOURCES := test.c

HEADERS := 

OBJS := ${SOURCES:%.c=${OUT_DIR}/%.o}

CFLAGS := -I raylib-5.5_linux_amd64/include -Lraylib-5.5_linux_amd64/include/ -lraylib -lm -Wall -Wextra -O3 -flto ${FLAGS}

LIBS :=

CC := cc

all: ${TARGET}

${TARGET}: Makefile build ${OBJS} ${LIBFT_A}
	${CC} ${CFLAGS} -o ${TARGET} ${OBJS} ${LIBS}

${OUT_DIR}/%.o: ${SRCS_DIR}/%.c ${HEADERS}
	mkdir -p $$(dirname $@)
	${CC} ${CFLAGS} -c $< -o $@

${OUT_DIR}:
	mkdir -p build

clean:
	rm -rf ${OUT_DIR}

fclean: clean
	rm -rf ${TARGET}

re:: fclean
re:: all

run: all
	./${TARGET}

.PHONY: clean fclean re

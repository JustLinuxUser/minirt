TARGET := miniRT

SRCS_DIR := src
OUT_DIR := build

SOURCES := ${wildcard ${SRCS_DIR}/*.c} ${wildcard ${SRCS_DIR}/**/*.c}

HEADERS := 

OBJS := ${SOURCES:${SRCS_DIR}/%.c=${OUT_DIR}/%.o}

ifdef OPT
	CFLAGS := -MMD -fPIE -Wall -Wextra  -O3 -flto ${FLAGS}
else
	CFLAGS := -MMD -Wall -Wextra  -fPIE --std=c99 -pedantic -g3 -fsanitize=address,leak,undefined ${FLAGS}
endif

LIBFT_DIR := src/libft
LIBFT := ${LIBFT_DIR}/libft.a

LIBS := -lm -L${LIBFT_DIR} -lft 

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LIBS += -Lraylib-5.5_linux_amd64/lib -lraylib
	CFLAGS += -I raylib-5.5_linux_amd64/include
endif
ifeq ($(UNAME_S),Darwin)
	LIBS += -Lraylib-5.5_macos/lib/ raylib-5.5_macos/lib/libraylib.a -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
	CFLAGS += -I raylib-5.5_macos/include
endif

CC := cc

all: ${TARGET}

${TARGET}: Makefile build ${LIBFT} ${OBJS} ${LIBFT_A}
	${CC} ${CFLAGS} -o ${TARGET} ${OBJS} ${LIBS}

${OUT_DIR}/%.o: ${SRCS_DIR}/%.c ${HEADERS}
	mkdir -p $$(dirname $@)
	${CC} ${CFLAGS} -c $< -o $@

${LIBFT}: ${LIBFT_DIR}
	make -C ${LIBFT_DIR}

${OUT_DIR}:
	mkdir -p build

clean:
	rm -rf ${OUT_DIR}
	make -C ${LIBFT_DIR} fclean

fclean: clean
	rm -rf ${TARGET}

re:: fclean
re:: all

run: all
	./${TARGET}

.PHONY: clean fclean re

-include ${SOURCES:%.c=${OUT_DIR}/%.d}

NAME := miniRT
CC := cc

SRCS_DIR := src
OUT_DIR := build

SOURCES := ${wildcard ${SRCS_DIR}/*.c} ${wildcard ${SRCS_DIR}/**/*.c}

BUILD_DIR := build

ifndef PROFILE
	PROFILE=debug
endif

TAG_FILE := $(BUILD_DIR)/profile_$(PROFILE)
NAME_PROFILE := $(BUILD_DIR)/$(PROFILE)_$(NAME)
PROFILES := opt debug debug_mem debug_mem_sanitize

CFLAGS := -MMD -fPIE -Wall -Wextra -Wmaybe-uninitialized -Wmissing-field-initializers --std=c99 -pedantic -g3 $(FLAGS)

# TODO: Add this before entering the project
# CFLAGS += -Werror

LIBFT_DIR := ./src/libft
LIBFT := ${LIBFT_DIR}/libft.a

LIBS := -lm -L ${LIBFT_DIR} -lft 

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LIBS += -Lsrc/MLX42/build -lmlx42 -lglfw
	CFLAGS += -I src/MLX42/include
	LIBS += -Lraylib-5.5_linux_amd64/lib -lraylib
	CFLAGS += -I raylib-5.5_linux_amd64/include
endif
ifeq ($(UNAME_S),Darwin)
	LIBS += -Lsrc/MLX42/build -lmlx42
	CFLAGS += -I src/MLX42/include
endif

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
	ifeq ($(UNAME_S),Darwin)
		CFLAGS += -DDEBUG_ALLOC -fsanitize=address
	else
		CFLAGS += -DDEBUG_ALLOC -fsanitize=address,leak,undefined
	endif
endif

PROFILE_FOUND := $(filter $(PROFILE), $(PROFILES))

OBJS := $(SOURCES:%.c=$(BUILD_DIR)/$(PROFILE)/%.o)

define NL


endef

$(if ${PROFILE_FOUND}, , $(error Unknown profile" ${PROFILE}${NL}Choose one of: $(PROFILES)))

all:: $(LIBFT)
all:: $(NAME)

$(NAME): $(NAME_PROFILE) ${TAG_FILE}
	cp $(NAME_PROFILE) $(NAME)

$(NAME_PROFILE): $(OBJS) $(LIBFT)
	${CC} ${CFLAGS} -o $(NAME_PROFILE) $(OBJS) $(LIBS) 

${TAG_FILE}:
	rm -rf $(BUILD_DIR)/profile_*
	touch ${TAG_FILE}

$(LIBFT): FORCE
	$(MAKE) -j -C ${LIBFT_DIR}

FORCE: ;

build/${PROFILE}/%.o: %.c Makefile
	@mkdir -p $$(dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
	$(MAKE) -C $(LIBFT_DIR) fclean

fclean: clean
	rm -rf $(NAME)

re::fclean
re::all

.PHONY: clean fclean re
-include $(SOURCES:%.c=$(BUILD_DIR)/$(PROFILE)/%.d)

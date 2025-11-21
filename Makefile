NAME := miniRT
CC := cc

SRCS_DIR := src
OUT_DIR := build

SOURCES := src/bounds.c\
src/bounds2.c\
src/bvh.c\
src/bvh_build_recursive.c\
src/bvh_build_sah.c\
src/bvh_cmp.c\
src/bvh_collide.c\
src/bvh_sah_init.c\
src/camera.c\
src/cie_tables/cie_x_1.c\
src/cie_tables/cie_x_2.c\
src/cie_tables/cie_x_3.c\
src/cie_tables/cie_x_4.c\
src/cie_tables/cie_x_5.c\
src/cie_tables/cie_y_1.c\
src/cie_tables/cie_y_2.c\
src/cie_tables/cie_y_3.c\
src/cie_tables/cie_y_4.c\
src/cie_tables/cie_y_5.c\
src/cie_tables/cie_z_1.c\
src/cie_tables/cie_z_2.c\
src/cie_tables/cie_z_3.c\
src/cie_tables/cie_z_4.c\
src/cie_tables/cie_z_5.c\
src/collide_shapes.c\
src/colors.c\
src/colors2.c\
src/draw.c\
src/errors.c\
src/hooks.c\
src/libft/alloc/alloc.c\
src/libft/alloc/alloc2.c\
src/libft/alloc/mmalloc.c\
src/libft/dsa/deque_char.c\
src/libft/dsa/deque_char2.c\
src/libft/dsa/deque_int.c\
src/libft/dsa/deque_int2.c\
src/libft/dsa/dyn_str.c\
src/libft/dsa/dyn_str2.c\
src/libft/dsa/dyn_str3.c\
src/libft/dsa/partition.c\
src/libft/dsa/prng.c\
src/libft/dsa/smoothsort.c\
src/libft/dsa/smoothsort2.c\
src/libft/dsa/vec_dyn_str.c\
src/libft/dsa/vec_gen.c\
src/libft/dsa/xoroshiro128plusplus.c\
src/libft/dsa/xorshift.c\
src/libft/ft_printf/eprintf.c\
src/libft/ft_printf/fdprintf.c\
src/libft/ft_printf/hex_utils.c\
src/libft/ft_printf/parser.c\
src/libft/ft_printf/printf.c\
src/libft/ft_printf/printf_hex.c\
src/libft/ft_printf/printf_int.c\
src/libft/ft_printf/printf_ptr.c\
src/libft/ft_printf/printf_str.c\
src/libft/ft_printf/ptr_utils.c\
src/libft/ft_printf/signed_int_utils.c\
src/libft/ft_printf/writer.c\
src/libft/ft_printf/writer2.c\
src/libft/gnl/get_next_line.c\
src/libft/gnl/get_next_line_utils.c\
src/libft/src/ft_atoi.c\
src/libft/src/ft_bzero.c\
src/libft/src/ft_calloc.c\
src/libft/src/ft_checked_atoi.c\
src/libft/src/ft_isalnum.c\
src/libft/src/ft_isalpha.c\
src/libft/src/ft_isascii.c\
src/libft/src/ft_isdigit.c\
src/libft/src/ft_isprint.c\
src/libft/src/ft_itoa.c\
src/libft/src/ft_lstadd_back_bonus.c\
src/libft/src/ft_lstadd_front_bonus.c\
src/libft/src/ft_lstclear_bonus.c\
src/libft/src/ft_lstdelone_bonus.c\
src/libft/src/ft_lstiter_bonus.c\
src/libft/src/ft_lstlast_bonus.c\
src/libft/src/ft_lstmap_bonus.c\
src/libft/src/ft_lstnew_bonus.c\
src/libft/src/ft_lstsize_bonus.c\
src/libft/src/ft_memchr.c\
src/libft/src/ft_memcmp.c\
src/libft/src/ft_memcpy.c\
src/libft/src/ft_memmove.c\
src/libft/src/ft_memset.c\
src/libft/src/ft_memswap.c\
src/libft/src/ft_putchar_fd.c\
src/libft/src/ft_putendl_fd.c\
src/libft/src/ft_putnbr_fd.c\
src/libft/src/ft_putstr_fd.c\
src/libft/src/ft_split.c\
src/libft/src/ft_split_str.c\
src/libft/src/ft_strchr.c\
src/libft/src/ft_strcmp.c\
src/libft/src/ft_strdup.c\
src/libft/src/ft_striteri.c\
src/libft/src/ft_strjoin.c\
src/libft/src/ft_strlcat.c\
src/libft/src/ft_strlcpy.c\
src/libft/src/ft_strlen.c\
src/libft/src/ft_strmapi.c\
src/libft/src/ft_strnchr.c\
src/libft/src/ft_strncmp.c\
src/libft/src/ft_strnstr.c\
src/libft/src/ft_strrchr.c\
src/libft/src/ft_strtrim.c\
src/libft/src/ft_substr.c\
src/libft/src/ft_tolower.c\
src/libft/src/ft_toupper.c\
src/libft/utils/math_utils.c\
src/libft/utils/math_utils2.c\
src/libft/utils/misc.c\
src/libft/utils/string_utils.c\
src/light.c\
src/main.c\
src/mymath.c\
src/mymath2.c\
src/mymath3.c\
src/mymath4.c\
src/obj_file/obj_parser.c\
src/obj_file/obj_parser_helpers.c\
src/obj_file/obj_tokenizer.c\
src/obj_file/obj_tokenizer_helpers.c\
src/obj_file/obj_tokenizer_nums.c\
src/obj_file/obj_tokenizer_tuples.c\
src/obj_file/obj_tokenizer_words.c\
src/ppm_writer.c\
src/probability.c\
src/ray.c\
src/ray_helpers.c\
src/render_multithreaded.c\
src/render_multithreaded2.c\
src/render_step.c\
src/render_utils.c\
src/keyhook.c\
src/rt_file/consumer_camera.c\
src/rt_file/consumer_getters.c\
src/rt_file/consumer_getters2.c\
src/rt_file/consumer_items.c\
src/rt_file/consumer_items2.c\
src/rt_file/consumer_main.c\
src/rt_file/consumer_utils.c\
src/rt_file/errors.c\
src/rt_file/errors_consumer.c\
src/rt_file/errors_consumer2.c\
src/rt_file/highlight.c\
src/rt_file/highlight_utils.c\
src/rt_file/parser.c\
src/rt_file/parser2.c\
src/rt_file/parser_utils.c\
src/rt_file/pprint_node.c\
src/rt_file/rt_file.c\
src/rt_file/tokenizer.c\
src/rt_file/tokenizer_num.c\
src/rt_file/tokenizer_tokens.c\
src/rt_file/tokenizer_utils.c\
src/rt_file/type_utils.c\
src/rt_file/utils.c\
src/rt_file/vec_rt_kv.c\
src/rt_file/vec_rt_kv2.c\
src/rt_file/vec_rt_nd.c\
src/samplers.c\
src/shapes.c\
src/shapes2.c\
src/spectrum.c\
src/tinyobj.c\
src/utils.c\
src/xyz_to_spectrum.c

BUILD_DIR := build

ifndef PROFILE
	PROFILE=debug
endif


PROFILES := opt debug debug_mem debug_mem_sanitize debug_mem_sanitize_mem
CFLAGS := -fPIE -MMD -fPIE -Wall -Wextra -Werror --std=c99 -pedantic -g3 $(FLAGS)

ifdef BONUS
	BUILD_NAME := ${PROFILE}_BONUS
	CFLAGS += -DBONUS
endif
ifndef BONUS
	BUILD_NAME := ${PROFILE}
endif

TAG_FILE := $(BUILD_DIR)/profile_$(BUILD_NAME)
NAME_PROFILE := $(BUILD_DIR)/$(BUILD_NAME)_$(NAME)

LIBFT_DIR := ./src/libft
LIBFT := ${LIBFT_DIR}/build/$(PROFILE)_libft.a


MLX_DIR := ./src/MLX42/build
MLX_INCLUDE := ./src/MLX42/include
LIBMLX := ${MLX_DIR}/libmlx42.a

LIBS := -lm -L ${LIBFT_DIR} -lft 

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LIBS += -L${MLX_DIR} -lmlx42 -lglfw
	CFLAGS += -I ${MLX_INCLUDE}
endif
ifeq ($(UNAME_S),Darwin)
	LIBS += -L${MLX_DIR} -lmlx42 -lglfw
	CFLAGS += -I ${MLX_INCLUDE}
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
		CFLAGS += -DDEBUG_ALLOC -fsanitize=address,undefined
	else
		CFLAGS += -DDEBUG_ALLOC -fsanitize=address,leak,undefined
	endif
endif
ifeq ($(PROFILE),debug_mem_sanitize_mem)
	CC := clang
	ifeq ($(UNAME_S),Darwin)
		CFLAGS += -DDEBUG_ALLOC -fsanitize-recover=all -fsanitize=undefined,memory -fsanitize-memory-track-origins -fPIE -fno-omit-frame-pointer
	else
		CFLAGS += -DDEBUG_ALLOC -fsanitize-recover=all -fsanitize=undefined,memory -fsanitize-memory-track-origins -fPIE -fno-omit-frame-pointer
	endif
endif

PROFILE_FOUND := $(filter $(PROFILE), $(PROFILES))

OBJS := $(SOURCES:%.c=$(BUILD_DIR)/$(BUILD_NAME)/%.o)

define NL


endef

$(if ${PROFILE_FOUND}, , $(error Unknown profile" ${PROFILE}${NL}Choose one of: $(PROFILES)))

all: $(NAME)

bonus:
	make BONUS=true

$(NAME): $(NAME_PROFILE) ${TAG_FILE}
	cp $(NAME_PROFILE) $(NAME)

# It's important that libft get's made first, because if it's not,
# the autogenerated header files might be missing as dependencies
$(NAME_PROFILE):: $(LIBFT)
$(NAME_PROFILE):: $(LIBFT) $(LIBMLX) $(OBJS)
	${CC} ${CFLAGS} -o $(NAME_PROFILE) $(OBJS) $(LIBS) 

${TAG_FILE}:
	mkdir -p build
	rm -rf $(BUILD_DIR)/profile_*
	touch ${TAG_FILE}

$(LIBFT): FORCE
	CC=${CC} $(MAKE) -j -C ${LIBFT_DIR}

$(LIBMLX):
	mkdir -p ${MLX_DIR}
	cd ${MLX_DIR} && cmake .. -G "Unix Makefiles"
	$(MAKE) -C ${MLX_DIR} -j

FORCE: ;

build/${BUILD_NAME}/%.o: %.c Makefile
	@mkdir -p $$(dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -rf ${MLX_Dir}

fclean: clean
	rm -rf $(NAME)

re::fclean
re::all

.PHONY: clean fclean re all bonus
-include $(SOURCES:%.c=$(BUILD_DIR)/$(BUILD_NAME)/%.d)

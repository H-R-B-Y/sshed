NAME			:= test_project
CC 				:= gcc
CFLAGS			:= -Wextra -Wall -Werror -g3 -O0 -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=600

MAKEFLAGS		+= --no-print-directory

LIBFLAGS		:= -lnotcurses -lnotcurses-core -lncurses -ltinfo -L./lib/libft -lft

INCLUDES		:= -I$(abspath ./include) -I$(abspath ./lib/libft/include)

LIB_DIR			:= ./lib
LIBFT_DIR		:= $(LIB_DIR)/libft

SRC_DIR			:= ./src

SRCS			:= \
				src/cards/init_cards.c \
				\


BIN				:= \
				\

TEST_SCRIPT		:=

OBJS			:= $(BIN:.bin=.o) ${SRCS:.c=.o}

MAIN			:= $(SRC_DIR)/main.c

all: $(NAME)

$(NAME): $(MAIN) $(OBJS) libft
		$(CC) $(CFLAGS) $(INCLUDES) $(MAIN) $(OBJS) $(LIBFLAGS) -o $(NAME)

libft:
		@$(MAKE) --directory $(LIBFT_DIR) CFLAGS="$(CFLAGS)" 

%.o: %.c
		@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

%.o: %.bin
	@ld -r -b binary -o $@ $<

clean:
		@rm -rf $(OBJS)
		@find . -name '*.gcda' -delete
		@find . -name '*.gcno' -delete

rm:
		@$(MAKE) --directory $(LIBFT_DIR) fclean
		@$(MAKE) --directory $(MLX_DIR) clean
		@$(MAKE) --directory $(MLX_WRAPPER_DIR) fclean
		@$(MAKE) --directory $(TRANSPIPE_DIR) fclean
		@rm -rf $(NAME)
		@rm -rf $(LIB_DIR)/libft.a $(LIB_DIR)/libmlx.a $(LIB_DIR)/libmlxwrap.a $(LIB_DIR)/*.a

fclean: clean rm pre post

re: fclean all

# Check submodule status
submodule-status:
	@echo "=== Transform Pipeline Status ==="
	@cd $(TRANSPIPE_DIR) && git status --porcelain
	@echo "=== Libft Status ==="
	@cd $(LIBFT_DIR) && git status --porcelain
	@echo "=== Parent Repo Submodule Status ==="
	@git status --porcelain lib/

install-submodules:
	@echo "Initializing and updating submodules..."
	@git submodule update --init --recursive
	@echo "Submodules initialized and updated successfully"
	@echo "Patching minilibx for Linux..."
	@cd $(MLX_DIR) && patch -p1 < ../mlx_patch
	@echo "MLX patch applied successfully"
	@cd $(MLX_DIR) && ./configure
	@echo "MLX configured successfully"
	@echo "Recording submodule versions in parent repository..."
	@git add lib/
	@echo "Submodule versions recorded. Run 'git commit' to save the state."

pull-submodules:
	@echo "Pulling latest changes from all submodules..."
	@git submodule update --remote --merge
	@echo "Submodules updated successfully"
	@echo "Recording updated submodule versions in parent repository..."
	@git add lib/
	@echo "Updated submodule versions recorded. Run 'git commit' to save the new state."

coverage:
ifeq ($(TEST_SCRIPT),)
	@printf "There has been an issue running coverage: no script to run program\n"
else
	@rm -rf coverage_reports
	@find . -name '*.gcda' -delete
	@make --directory . re CFLAGS="-fprofile-arcs -ftest-coverage" CC=gcc-10
	@bash -c $(TEST_SCRIPT)
	@sleep 2
	@mkdir -p coverage_reports
	@find . -name '*.gcda' -exec mv -n {} . \;
	@find . -name '*.gcno' -exec mv -n {} . \;
	@find . -name '*.c' -exec gcov-10 -o . {} \;  >> coverage_reports.txt
	@find . -name '*.gcov' -exec mv {} coverage_reports/ \;
	@find . -name '*.gcda' -delete
	@find . -name '*.gcno' -delete
endif

.PHONY: all clean fclean re  submodule-status install-submodules pull-submodules pre post coverage

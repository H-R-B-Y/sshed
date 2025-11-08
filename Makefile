NAME			:= sshed_client.out
CC 				:= gcc
CFLAGS			:= -Wextra -Wall -g3 -O0 \
				-D_DEFAULT_SOURCE -D_XOPEN_SOURCE=600

SHELL			:= /bin/bash

MAKEFLAGS		+= --no-print-directory

LIBFLAGS		:= -lnotcurses -lnotcurses-core -lncurses -ltinfo \
					-L./lib/cards -lcards \
					-L./lib/sf_resources/ -lsf_client \
					-L./lib/libft -lft \
					\

INCLUDES		:= -I$(abspath ./include) -I$(abspath ./lib/libft/include) \
				  -I$(abspath ./lib/cards/include) \
				  -I$(abspath ./lib/sf_resources/include)

LIB_DIR			:= ./lib
LIBFT_DIR		:= $(LIB_DIR)/libft
LIBCARDS_DIR	:= $(LIB_DIR)/cards
LIBCLIENT_DIR	:= $(LIB_DIR)/sf_resources

SRC_DIR			:= ./src

SRCS			:= \
				src/menu/menu.c \
				src/main_menu/main_menu.c \
				src/init/init.c \
				src/init/init_hooks.c \
				src/game_manager/game_manager.c \
				src/game_manager/game_man_run.c \
				src/game_manager/game_man_state.c \
				src/game_setup/game_setup.c \
				src/settings_menu/settings_menu.c \
				src/local_game_logic/game_end_state.c \
				src/local_game_logic/game_local_logic.c \
				src/local_game_logic/local_game_load.c \
				src/local_game_logic/local_game_pause.c \
				src/ai_logic/computer_ai.c \
				src/local_game_logic/game_playing_input_handler.c \
				src/player_action/player_action.c \
				src/local_game_logic/game_handle_player_action.c \
				src/utils/unload_utils.c \
				src/ai_logic/step_thinking.c \
				src/ai_logic/step_idle.c \
				src/ai_logic/step_none.c \
				src/ai_logic/step_playing.c \
				src/local_game_logic/organise_visuals.c \
				src/local_game_logic/swap_phase.c \
				\


BIN				:= \
				\

TEST_SCRIPT		:=

OBJS			:= $(BIN:.bin=.o) ${SRCS:.c=.o}

MAIN			:= $(SRC_DIR)/main.c

all: $(NAME)

$(NAME): $(MAIN) $(OBJS) \
				lib/libft/libft.a \
				images/Small/Diamonds\ 1.png \
				lib/cards/libcards.a \
				lib/sf_resources/libsf_client.a
				$(CC) $(CFLAGS) $(INCLUDES) $(MAIN) $(OBJS) $(LIBFLAGS) -o $(NAME)


move:
				cp -r images /home/gameuser
				chmod -R 777 /home/gameuser
				chown -R gameuser:gameuser /home/gameuser
				cp sshed_client.out /home/gameuser
				chown gameuser:gameuser /home/gameuser
				chmod 777 /home/gameuser/sshed_client.out

images/Small/Diamonds\ 1.png:
				@if [ ! -f "images/Small/Diamonds 1.png" ]; then \
					$(MAKE) cards; \
				fi

lib/libft/libft.a:
				@$(MAKE) -j10 --directory $(LIBFT_DIR) CFLAGS="$(CFLAGS)" || @$(MAKE) --directory $(LIBFT_DIR) fclean && $(MAKE) --directory $(LIBFT_DIR) CFLAGS="$(CFLAGS)"

lib/cards/libcards.a:
				@$(MAKE) -j10 --directory $(LIBCARDS_DIR) CFLAGS="$(CFLAGS)" || @$(MAKE) --directory $(LIBCARDS_DIR) fclean && $(MAKE) --directory $(LIBCARDS_DIR) CFLAGS="$(CFLAGS)"

lib/sf_resources/libsf_client.a:
				@$(MAKE) -j10 --directory $(LIBCLIENT_DIR) CFLAGS="$(CFLAGS)" || @$(MAKE) --directory $(LIBCLIENT_DIR) fclean && $(MAKE) --directory $(LIBCLIENT_DIR) CFLAGS="$(CFLAGS)"

%.o: %.c
				$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

%.o: %.bin
				@ld -r -b binary -o $@ $<

clean:
				@rm -rf $(OBJS)
				@$(MAKE) --directory $(LIBCARDS_DIR) clean
				@$(MAKE) --directory $(LIBCLIENT_DIR) clean
				@find . -name '*.gcda' -delete
				@find . -name '*.gcno' -delete

rm:
				@$(MAKE) --directory $(LIBFT_DIR) fclean
				@$(MAKE) --directory $(LIBCARDS_DIR) fclean
				@$(MAKE) --directory $(LIBCLIENT_DIR) fclean
				@rm -rf $(NAME)

fclean: clean rm pre post

re: fclean all

.cache/cards.zip:
		@mkdir -p .cache
		@cd .cache && curl https://opengameart.org/sites/default/files/Cards%20Pack.zip > cards.zip
		@echo "Cards.zip downloaded to .cache/cards.zip"
		@cd ..

images:
		mkdir -p images

cards: images .cache/cards.zip
		@cp .cache/cards.zip images/
		@unzip images/cards.zip -d images/cards
		@mv images/cards/PNG/* images
		@rm -rf images/cards
		@rm images/cards.zip
		cd images/Small && \
		for i in Diamond*.png; do \
			 mv "$$i" "$${i/Diamond/Diamonds}"; \
		done && \
		cd ../Medium && \
		for i in Diamond*.png; do \
			 mv "$$i" "$${i/Diamond/Diamonds}"; \
		done && \
		cd ../Large && \
		for i in Diamond*.png; do \
			 mv "$$i" "$${i/Diamond/Diamonds}"; \
		done
		@echo "Cards downloaded and extracted successfully"

delete-cards:
		@rm -rf .cache/cards.zip
		@rm -rf images

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

docs:
	@echo "Generating API documentation..."
	@mkdir -p docs
	@doxygen Doxyfile
	@echo "Documentation generated in docs/html/index.html"

clean-docs:
	@rm -rf docs/

.PHONY: all clean fclean \
	re submodule-status \
	install-submodules \
	pull-submodules pre \
	post coverage cards \
	delete-cards docs clean-docs

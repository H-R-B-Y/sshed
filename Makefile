NAME			:= sshed_client.out
CC 				:= gcc
CFLAGS			:= -Wextra -Wall -g3 -O0 -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=600

SHELL			:= /bin/bash

MAKEFLAGS		+= --no-print-directory

LIBFLAGS		:= -lnotcurses -lnotcurses-core -lncurses -ltinfo -L./lib/libft -lft

INCLUDES		:= -I$(abspath ./include) -I$(abspath ./lib/libft/include) \
				  -I$(abspath ./include/card) -I$(abspath ./include/hand) \
				  -I$(abspath ./include/pdisplay)

LIB_DIR			:= ./lib
LIBFT_DIR		:= $(LIB_DIR)/libft

SRC_DIR			:= ./src

SRCS			:= \
				src/pdisplay/pdisplay.c \
				src/init/init.c \
				src/hand/hand.c \
				src/hand/hand_return_to_deck.c \
				src/hand/hand_render.c \
				src/hand/hand_debug.c \
				src/hand/hand_clear.c \
				src/hand/hand_change_display.c \
				src/hand/shed/hand_remove_card_shed.c \
				src/hand/shed/hand_add_card_shed.c \
				src/hand/hand/hand_remove_card.c \
				src/hand/hand/hand_add_card.c \
				src/deck/deck.c \
				src/deck/deck_shuffle.c \
				src/deck/deck_draw.c \
				src/cards/init_cards.c \
				src/card_planes/card_plane_show.c \
				src/card_planes/card_plane_flip.c \
				src/card_planes/card_plane_create.c \
				src/pdisplay/pdisplay_render.c \
				src/pdisplay/pdisplay_shed/pdisplay_add_hand.c \
				src/pdisplay/pdisplay_shed/pdisplay_add_hand_anon.c \
				src/pdisplay/pdisplay_hand/pdisplay_add_shed.c \
				src/pdisplay/pdisplay_hand/pdisplay_add_shed_anon.c \
				src/pdisplay/pdisplay_change_display.c \
				src/pile_display/pile_display.c \
				src/pile_display/pile_display_render.c \
				src/pile_display/pile_display_move.c \
				src/pile_display/pile_display_clear.c \
				src/pile_display/pile_display_add.c \
				src/hand/hand_get_selected.c \
				src/pdisplay/pdisplay_pop_card.c \
				src/deck_anon/deck_anon_add.c \
				src/deck_anon/deck_anon_move.c \
				src/deck_anon/deck_anon_render.c \
				src/deck_anon/deck_anon.c \
				\


BIN				:= \
				\

TEST_SCRIPT		:=

OBJS			:= $(BIN:.bin=.o) ${SRCS:.c=.o}

MAIN			:= $(SRC_DIR)/main.c

all: $(NAME)

$(NAME): $(MAIN) $(OBJS) lib/libft/libft.a images/Small/Diamonds\ 1.png
		$(CC) $(CFLAGS) $(INCLUDES) $(MAIN) $(OBJS) $(LIBFLAGS) -o $(NAME)

images/Small/Diamonds\ 1.png:
		@if [ ! -f "images/Small/Diamonds 1.png" ]; then \
			$(MAKE) cards; \
		fi

lib/libft/libft.a:
		@$(MAKE) -j10 --directory $(LIBFT_DIR) CFLAGS="$(CFLAGS)" || @$(MAKE) --directory $(LIBFT_DIR) fclean && $(MAKE) --directory $(LIBFT_DIR) CFLAGS="$(CFLAGS)"

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
		@rm -rf $(NAME)
		@rm -rf $(LIB_DIR)/libft.a $(LIB_DIR)/*.a

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

.PHONY: all clean fclean \
	re submodule-status \
	install-submodules \
	pull-submodules pre \
	post coverage cards \
	delete-cards

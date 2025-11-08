NAME			:= libcards.a
CC 				:= gcc
CFLAGS			:= -Wextra -Wall -g3 -O0 -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=600
AR				:= ar rcs 
SHELL			:= /bin/bash

MAKEFLAGS		+= --no-print-directory

# LIBFLAGS		:= -lnotcurses -lnotcurses-core -lncurses -ltinfo -L./lib/libft -lft

INCLUDES		:= -I$(abspath ./include) -I$(abspath ../libft/include)

SRC_DIR			:= ./src

SRCS			:= \
				src/pdisplay/pdisplay.c \
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
				src/hand/hand_calc_max.c \
				src/pdisplay/pdisplay_pop_card.c \
				src/deck_anon/deck_anon_add.c \
				src/deck_anon/deck_anon_move.c \
				src/deck_anon/deck_anon_render.c \
				src/deck_anon/deck_anon.c \
				src/utils/make_plane_transparent.c \
				src/deck_display/deck_display_draw.c \
				src/deck_display/deck_display_render.c \
				src/deck_display/deck_display_show.c \
				src/deck_display/deck_display.c \
				src/deck_display/deck_draw_hand.c \
				src/pile_display/pile_display_cards.c \
				src/pile_display/pile_display_return.c \
				src/pdisplay/pdisplay_calc_max.c \
				src/card_planes/card_checks.c \
				src/hand/hand_checks.c \
				src/pdisplay/pdisplay_checks.c \
				\


OBJS			:= ${SRCS:.c=.o}


all: $(NAME)

$(NAME): $(OBJS)
		$(AR) $(NAME) $(OBJS)

move:
	cp -r images /home/gameuser
	chmod -R 777 /home/gameuser
	chown -R gameuser:gameuser /home/gameuser
	cp sshed_client.out /home/gameuser
	chown gameuser:gameuser /home/gameuser
	chmod 777 /home/gameuser/sshed_client.out

# images/Small/Diamonds\ 1.png:
# 		@if [ ! -f "images/Small/Diamonds 1.png" ]; then \
# 			$(MAKE) cards; \
# 		fi

# lib/libft/libft.a:
# 		@$(MAKE) -j10 --directory $(LIBFT_DIR) CFLAGS="$(CFLAGS)" || @$(MAKE) --directory $(LIBFT_DIR) fclean && $(MAKE) --directory $(LIBFT_DIR) CFLAGS="$(CFLAGS)"

%.o: %.c
		@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

# %.o: %.bin
# 	@ld -r -b binary -o $@ $<

clean:
		@rm -rf $(OBJS)
		@find . -name '*.gcda' -delete
		@find . -name '*.gcno' -delete

rm:
		@rm -rf $(NAME)

fclean: clean rm

re: fclean all

# .cache/cards.zip:
# 	@mkdir -p .cache
# 	@cd .cache && curl https://opengameart.org/sites/default/files/Cards%20Pack.zip > cards.zip
# 	@echo "Cards.zip downloaded to .cache/cards.zip"
# 	@cd ..

# images:
# 	mkdir -p images

# cards: images .cache/cards.zip
# 	@cp .cache/cards.zip images/
# 	@unzip images/cards.zip -d images/cards
# 	@mv images/cards/PNG/* images
# 	@rm -rf images/cards
# 	@rm images/cards.zip
# 	cd images/Small && \
# 	for i in Diamond*.png; do \
# 		 mv "$$i" "$${i/Diamond/Diamonds}"; \
# 	done && \
# 	cd ../Medium && \
# 	for i in Diamond*.png; do \
# 		 mv "$$i" "$${i/Diamond/Diamonds}"; \
# 	done && \
# 	cd ../Large && \
# 	for i in Diamond*.png; do \
# 		 mv "$$i" "$${i/Diamond/Diamonds}"; \
# 	done
# 	@echo "Cards downloaded and extracted successfully"

# delete-cards:
# 	@rm -rf .cache/cards.zip
# 	@rm -rf images

.PHONY: all clean fclean \
	re

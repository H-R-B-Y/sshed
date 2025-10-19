#ifndef CARDS2_H
#define CARDS2_H
# define FT_INCLUDE_ALL
# include "libft.h"

typedef enum e_suit
{
	SUIT_CLUBS,
	SUIT_DIAMONDS,
	SUIT_SPADES,
	SUIT_HEARTS,
	SUIT_JOKER,  // Special suit for jokers
	SUIT_COUNT
} t_suit;

typedef enum e_rank
{
	RANK_ACE,
	RANK_TWO,
	RANK_THREE,
	RANK_FOUR,
	RANK_FIVE,
	RANK_SIX,
	RANK_SEVEN,
	RANK_EIGHT,
	RANK_NINE,
	RANK_TEN,
	RANK_JACK,
	RANK_QUEEN,
	RANK_KING,
	RANK_JOKER_BLACK,  // Black joker gets its own rank
	RANK_JOKER_RED,    // Red joker gets its own rank
	RANK_COUNT
} t_rank;

static const char *suit_str[SUIT_COUNT]
__attribute__((unused)) = {
	[SUIT_CLUBS] = "CLUBS",
	[SUIT_DIAMONDS] = "DIAMONDS",
	[SUIT_SPADES] = "SPADES",
	[SUIT_HEARTS] = "HEARTS",
	[SUIT_JOKER] = "JOKER",
};

static const char *suit_image_str[SUIT_COUNT]
__attribute__((unused)) = {
	[SUIT_CLUBS] = "Clubs",
	[SUIT_DIAMONDS] = "Diamonds",
	[SUIT_SPADES] = "Spades",
	[SUIT_HEARTS] = "Hearts",
	[SUIT_JOKER] = "Joker",
};

static const char *rank_str[RANK_COUNT]
__attribute__((unused)) = {
	"ACE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", 
	"EIGHT", "NINE", "TEN", "JACK", "QUEEN", "KING",
	"JOKER_BLACK", "JOKER_RED"
};

static const char *rank_image_string[RANK_COUNT]
__attribute__((unused)) = {
	"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", 
	"11", "12", "13", "Black", "Red"
};

typedef struct s_card_desc
{
	t_rank	rank;
	t_suit	suit;
} t_card_desc;

typedef struct s_card
{
	t_rank	rank;
	t_suit	suit;
	void			*graphic;
} t_card;

// Hash function for cards including jokers
static inline unsigned int	hash_card(t_card_desc *card_desc)
{
	// For jokers, we use a special calculation
	if (card_desc->rank == RANK_JOKER_BLACK)
		return (SUIT_COUNT * RANK_COUNT + 0); // Unique position for black joker
	if (card_desc->rank == RANK_JOKER_RED)
		return (SUIT_COUNT * RANK_COUNT + 1); // Unique position for red joker
	
	// For regular cards, use the normal hash
	return ((unsigned int)card_desc->suit * RANK_COUNT + (unsigned int)card_desc->rank);
}

static inline int			compare_card(t_card_desc *a, t_card_desc *b)
{
	if (a->rank == b->rank && a->suit == b->suit)
		return 0;
	return 1;
}

// Utility functions to check if a card is a joker
static inline int			is_joker(t_card *card)
{
	return (card->rank == RANK_JOKER_BLACK || card->rank == RANK_JOKER_RED);
}

static inline int			is_black_joker(t_card *card)
{
	return (card->rank == RANK_JOKER_BLACK);
}

static inline int			is_red_joker(t_card *card)
{
	return (card->rank == RANK_JOKER_RED);
}

extern t_hashmap	cardmap;

extern void			*cardbacks[2];

#endif


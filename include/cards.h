
#ifndef CARDS_H
#define CARDS_H
#define FT_INCLUDE_ALL
#include "libft.h"

#include "enum_macro.h"

typedef enum e_suit
{
	SUIT_CLUBS,
	SUIT_DIAMONDS,
	SUIT_SPADES,
	SUIT_HEARTS,
	SUIT_COUNT
} t_suit;

static const char *suit_str[SUIT_COUNT]
__attribute__((unused)) = {
	[SUIT_CLUBS] = "CLUBS",
	[SUIT_DIAMONDS] = "DIAMONDS",
	[SUIT_SPADES] = "SPADES",
	[SUIT_HEARTS] = "HEARTS",
};

static const char *suit_image_str[SUIT_COUNT]
__attribute__((unused)) = {
	[SUIT_CLUBS] = "Clubs",
	[SUIT_DIAMONDS] = "Diamonds",
	[SUIT_SPADES] = "Spades",
	[SUIT_HEARTS] = "Hearts",
};

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
	RANK_COUNT
} t_rank;

static const char *rank_str[RANK_COUNT]
__attribute__((unused)) = {
	"ACE",
	"TWO",
	"THREE",
	"FOUR",
	"FIVE",
	"SIX",
	"SEVEN",
	"EIGHT",
	"NINE",
	"TEN",
	"JACK",
	"QUEEN",
	"KING",
};

static const char *rank_image_string[RANK_COUNT]
__attribute__((unused)) = {
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"10",
	"11",
	"12",
	"13"
};

typedef struct s_card_desc
{
	t_rank	rank;
	t_suit	suit;
}	t_card_desc;

typedef struct s_card
{
	t_rank	rank;
	t_suit	suit;
	void	*graphic;
}	t_card;

extern t_hashmap	cardmap;

#endif

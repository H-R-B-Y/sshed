#ifndef CARDS_H
# define CARDS_H
# define FT_INCLUDE_ALL
# include "libft/libft.h"
# include <notcurses/notcurses.h>
# include <stdio.h>
# include <stdlib.h>

# define CARD_LOAD_PATH "images/Small/"

# define DEFAULT_ANON_CARD \
	(struct s_card_desc){ .suit = SUIT_COUNT, .rank = RANK_COUNT }

extern t_hashmap			cardmap;
extern void					*cardbacks[4];
extern unsigned int			card_dimensions[4]; // width, height, h_width, h_height

typedef enum e_suit
{
	/// @brief Clubs suit
	SUIT_CLUBS,
	/// @brief Diamonds suit
	SUIT_DIAMONDS,
	/// @brief Spades suit
	SUIT_SPADES,
	/// @brief Hearts suit
	SUIT_HEARTS,
	/// @brief Joker suit, Special use case
	SUIT_JOKER,
	/// @brief Count of suits
	SUIT_COUNT
} t_suit;

typedef enum e_rank
{
	/// @brief Ace rank
	RANK_ACE,
	/// @brief Two rank
	RANK_TWO,
	/// @brief Three rank
	RANK_THREE,
	/// @brief Four rank
	RANK_FOUR,
	/// @brief Five rank
	RANK_FIVE,
	/// @brief Six rank
	RANK_SIX,
	/// @brief Seven rank
	RANK_SEVEN,
	/// @brief Eight rank
	RANK_EIGHT,
	/// @brief Nine rank
	RANK_NINE,
	/// @brief Ten rank
	RANK_TEN,
	/// @brief Jack rank
	RANK_JACK,
	/// @brief Queen rank
	RANK_QUEEN,
	/// @brief King rank
	RANK_KING,
	/// @brief Joker ranks
	RANK_JOKER_BLACK,
	RANK_JOKER_RED,
	/// @brief Count of ranks
	RANK_COUNT
} t_rank;

/// @brief String representations of suits and ranks
static const char *suit_str[SUIT_COUNT]
__attribute__((unused)) = {
	[SUIT_CLUBS] = "CLUBS",
	[SUIT_DIAMONDS] = "DIAMONDS",
	[SUIT_SPADES] = "SPADES",
	[SUIT_HEARTS] = "HEARTS",
	[SUIT_JOKER] = "JOKER",
};

/// @brief String representations of suits for image file names
static const char *suit_image_str[SUIT_COUNT]
__attribute__((unused)) = {
	[SUIT_CLUBS] = "Clubs",
	[SUIT_DIAMONDS] = "Diamonds",
	[SUIT_SPADES] = "Spades",
	[SUIT_HEARTS] = "Hearts",
	[SUIT_JOKER] = "Joker",
};

/// @brief Short string representations of suits for compact display
static const char *rank_str[RANK_COUNT]
__attribute__((unused)) = {
	"ACE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", 
	"EIGHT", "NINE", "TEN", "JACK", "QUEEN", "KING",
	"JOKER_BLACK", "JOKER_RED"
};

/// @brief Short string representations of ranks for image file names
static const char *rank_image_string[RANK_COUNT]
__attribute__((unused)) = {
	"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", 
	"11", "12", "13", "Black", "Red"
};

typedef struct s_card_desc
{
	/// @brief Rank of the card
	t_rank	rank;
	/// @brief Suit of the card
	t_suit	suit;
} t_card_desc;

/**
 * @brief Check if two card descriptions are equal
 * 
 * @param a First card description
 * @param b Second card description
 * @return 1 if equal, 0 otherwise
 */
static inline int	card_is_equal(t_card_desc a, t_card_desc b)
{
	return (
		a.rank == b.rank && a.suit == b.suit
	);
}

/**
 * @brief Check if a card description is valid
 * 
 * @param desc Card description to check
 * @return 1 if valid, 0 otherwise
 */
static inline int	card_is_valid(t_card_desc desc)
{
	return (
		(desc.rank >= RANK_ACE && desc.rank < RANK_COUNT)
		&& (desc.suit >= SUIT_CLUBS && desc.suit < SUIT_COUNT)
		&& // The suit is joker and only rank joker or the rank is not joker and the suit is not joker
		(
			(desc.suit == SUIT_JOKER && (desc.rank == RANK_JOKER_BLACK || desc.rank == RANK_JOKER_RED))
			||
			(desc.suit != SUIT_JOKER && (desc.rank != RANK_JOKER_BLACK && desc.rank != RANK_JOKER_RED))
		)
	);
}

/**
 * @brief Check if a card description is a joker
 * @param desc Card description to check
 * @return 1 if joker, 0 otherwise
 */
static inline int	card_is_joker(t_card_desc desc)
{
	if (!card_is_valid(desc))
		return (0);
	return (
		(desc.suit == SUIT_JOKER && (desc.rank == RANK_JOKER_BLACK || desc.rank == RANK_JOKER_RED))
	);
}

/**
 * @brief Check if a card description is a face card (J, Q, K)
 * @param desc Card description to check
 * @return 1 if face card, 0 otherwise
 */
static inline int	card_is_face(t_card_desc desc)
{
	return (
		desc.rank == RANK_JACK
		|| desc.rank == RANK_QUEEN
		|| desc.rank == RANK_KING
	);
}

// static inline int	card_is_valid(t_card_desc desc);
// static inline int	card_is_valid(t_card_desc desc);
// static inline int	card_is_valid(t_card_desc desc);

typedef struct s_card
{
	/// @brief Rank of the card
	/// @warning DO NOT CHANGE THE ORDER OF RANK AND SUIT THEY ARE ORDERED FOR PUNNING
	t_rank	rank;
	/// @brief Suit of the card
	/// @warning DO NOT CHANGE THE ORDER OF RANK AND SUIT THEY ARE ORDERED FOR PUNNING
	t_suit	suit;
	/// @brief Graphic representation of the card
	void	*graphic;
	/// @brief Horizontal graphic representation of the card
	void	*graphic_h;
} t_card;

/**
 * @brief Hash function for card descriptions
 * 
 * @param card_desc The card description to hash
 * @return The hash value
 */
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

/**
 * @brief Comparison function for card descriptions
 * @param a First card description
 * @param b Second card description
 * @return 0 if equal, non-zero otherwise
 */
static inline int			compare_card(t_card_desc *a, t_card_desc *b)
{
	if (a->rank == b->rank && a->suit == b->suit)
		return 0;
	return 1;
}

/**
 * @brief Check if a card is a joker
 * @param card The card to check
 * @return 1 if joker, 0 otherwise
 */
static inline int			t_card_is_joker(t_card *card)
{
	return (card->rank == RANK_JOKER_BLACK || card->rank == RANK_JOKER_RED);
}

/**
 * @brief Check if a card is a black joker
 * @param card The card to check
 * @return 1 if black joker, 0 otherwise
 */
static inline int			t_card_is_black_joker(t_card *card)
{
	return (card->rank == RANK_JOKER_BLACK);
}

/**
 * @brief Check if a card is a red joker
 * @param card The card to check
 * @return 1 if red joker, 0 otherwise
 */
static inline int			t_card_is_red_joker(t_card *card)
{
	return (card->rank == RANK_JOKER_RED);
}
/**
 * @brief Initialize the card system
 * 
 * This function will load all card images and prepare
 * the card system for use.
 * Populating the cardmap hashmap and loading cardback images.
 * 
 * @param nc Pointer to the notcurses context
 * @return 0 on success, 1 on error
 */
int							init_cards(void *nc);

/**
 * @brief Destroy the card system
 * 
 * This function will free all resources used by the card system.
 */
void						destroy_cards(void);

#endif


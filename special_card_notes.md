# Special Card Effects - Design Notes

## Problem Statement

The current `_next_card_stacks()` function has hardcoded card effects that make it difficult to:
- Configure house rules
- Add new card behaviors
- Share logic between game engine and AI
- Experiment with custom rule variants

Current hardcoded behaviors:
- **Rank 8**: Transparent (takes on rank of card below)
- **Rank 7**: Forces lower cards only
- **Ace**: Can play on anything (high/low)
- **Jokers**: Can play on anything, burns pile
- **Default**: Must be equal or higher rank

## Design Goals

1. **Single Source of Truth**: Rules configured once, used everywhere
2. **AI Compatibility**: Same rules accessible to AI decision making
3. **Extensibility**: Easy to add new house rules and effects
4. **Modularity**: Clean separation between rules and game logic
5. **Experimentation**: Support for custom/non-standard rule variants

## Proposed Solutions

### 🎯 Approach 1: Rule Configuration System

**Core Concept**: Separate rules from logic with configuration structures.

```c
struct s_card_rule {
    t_card_rank     rank;
    bool            can_play_on_anything;    // Ace, Joker
    bool            is_transparent;          // 8 (or configurable)
    bool            reverses_direction;      // House rule possibility
    bool            burns_pile;              // Joker
    bool            forces_lower;            // 7
    int             special_value;           // For complex rules
};

struct s_game_rules {
    struct s_card_rule  card_rules[RANK_COUNT];
    bool                multiple_cards_allowed;
    bool                ace_high_or_low;
    // ... other game variants
};
```

**Benefits**:
- Single source of truth
- Easy to configure during game setup
- Can be passed to both game logic and AI
- Extensible for new house rules

### 🎯 Approach 2: Function Pointer Strategy Pattern

**Core Concept**: Replace hardcoded checks with configurable function pointers.

```c
typedef bool (*t_can_play_fn)(
    const struct s_card_desc *top_card,
    const struct s_card_desc *play_card,
    const struct s_game_rules *rules
);

typedef void (*t_special_effect_fn)(
    struct s_game_manager *manager,
    struct s_game_local *game,
    struct s_player_action *action
);

struct s_card_behavior {
    t_can_play_fn       can_play_check;
    t_special_effect_fn special_effect;
    bool                burns_pile;
    bool                is_transparent;
};

// Rules table
struct s_card_behavior card_behaviors[RANK_COUNT];
```

**Usage Pattern**:
```c
// Instead of hardcoded checks:
if (card_behaviors[top_card->rank].is_transparent) {
    // Handle transparent card logic
}

if (card_behaviors[play_card->rank].can_play_check(top_card, play_card, rules)) {
    // Can play this card
}
```

### 🎯 Approach 3: Rule Engine with Queries

**Core Concept**: Create functions that answer specific questions about cards.

```c
bool rules_can_play_on_empty(t_card_rank rank, const struct s_game_rules *rules);
bool rules_can_play_on_card(t_card_rank play_rank, t_card_rank top_rank, const struct s_game_rules *rules);
bool rules_is_transparent(t_card_rank rank, const struct s_game_rules *rules);
t_card_rank rules_get_effective_rank(t_card_rank rank, const struct s_pile_display *pile, const struct s_game_rules *rules);
```

**Clean Logic**:
```c
static int _next_card_stacks_configurable(
    struct s_game_manager *manager,
    struct s_game_local *game,
    struct s_player_action *action
) {
    const struct s_game_rules *rules = &game->settings.rules;
    t_card_rank effective_top_rank;
    
    if (game->pile_display->cards->count == 0) {
        return rules_can_play_on_empty(action->cards[0].rank, rules);
    }
    
    effective_top_rank = rules_get_effective_rank(
        top_card->rank, 
        game->pile_display, 
        rules
    );
    
    return rules_can_play_on_card(
        action->cards[0].rank, 
        effective_top_rank, 
        rules
    );
}
```

### 🎯 Approach 4: State Machine for Card Interactions

**Core Concept**: Model card relationships as discrete states.

```c
enum e_card_relationship {
    RELATIONSHIP_BLOCKS,      // Can't play
    RELATIONSHIP_STACKS,      // Normal stacking
    RELATIONSHIP_TRANSPARENT, // See through to card below
    RELATIONSHIP_BURNS,       // Removes pile
    RELATIONSHIP_FORCES_LOW   // Must play lower
};

enum e_card_relationship rules_get_relationship(
    t_card_rank play_rank,
    t_card_rank top_rank,
    const struct s_game_rules *rules
);
```

## 🚀 Recommended Hybrid Approach

**Combine Approaches 1 and 3** for optimal balance of simplicity and extensibility:

### 1. Configuration Structure (Simple & Extensible)
```c
struct s_game_rules {
    // Basic card properties
    t_card_rank     transparent_cards[4];  // Which cards are transparent
    t_card_rank     wild_cards[4];         // Can play on anything
    t_card_rank     burn_cards[4];         // Burn the pile
    t_card_rank     reverse_direction[4];  // Reverse play order
    
    // Special behaviors
    bool            sevens_force_lower;
    bool            aces_high_and_low;
    bool            multiple_same_rank;
    
    // Future extensibility
    void            *custom_rules_data;
};
```

### 2. Query Functions (Clean Interface)
```c
// Core questions your game logic needs to ask:
bool rules_can_play_card(const struct s_card_desc *play_card, 
                        const struct s_pile_display *pile,
                        const struct s_game_rules *rules);

t_card_rank rules_get_effective_top_rank(const struct s_pile_display *pile,
                                        const struct s_game_rules *rules);

bool rules_triggers_special_effect(t_card_rank rank, const struct s_game_rules *rules);
```

### 3. Benefits of This Approach:
- ✅ **Single source of truth** - Rules configured once
- ✅ **AI compatible** - Same rules passed to AI thinking
- ✅ **Extensible** - Easy to add new house rules
- ✅ **Testable** - Can unit test individual rule functions
- ✅ **Clean separation** - Game logic asks questions, rules provide answers

### 4. Implementation Strategy:
1. **Start simple** - Move current hardcoded rules to configuration
2. **Add query functions** - Replace direct checks with rule queries
3. **Extend gradually** - Add new house rules as needed
4. **Share with AI** - Same rule queries work for AI decision making

## Current Issues to Address

### Complex Transparent Card Logic
The current RANK_EIGHT handling is particularly complex:
```c
else if (what_is_on_top->rank == RANK_EIGHT)
{
    if (game->pile_display->cards->count == 1)
        return (action->cards[0].rank >= what_is_on_top->rank);

    // Complex logic to find the card below all consecutive 8s
    size_t idx = 1;
    struct s_cdll_node *node = game->pile_display->cards->head->next;
    // ... more complex traversal logic
}
```

**Solution**: Abstract this into `rules_get_effective_rank()` function that handles transparent card traversal.

### Hardcoded Special Effects
Current `special_actions` array is hardcoded:
```c
static const t_special_action special_actions[RANK_COUNT] = {
    [RANK_JOKER_BLACK] = special_action_burn_pile,
    [RANK_JOKER_RED] = special_action_burn_pile
    // ... rest are NULL
};
```

**Solution**: Make this configurable based on the game rules structure.

### AI Integration
The AI system needs to understand the same rules for decision making. The rule query functions provide a clean interface that both the game engine and AI can use.

## Example House Rules to Support

1. **10s are transparent** instead of 8s
2. **Queens reverse direction** of play
3. **2s force next player to draw 2 cards**
4. **Multiple cards of same rank** can be played together
5. **Custom burn combinations** (e.g., four of a kind burns pile)
6. **Ace high/low flexibility** in same game
7. **Skip cards** that skip the next player's turn

## File Structure Recommendations

```
src/game_rules/
├── rules_config.c/.h     # Rule configuration structures
├── rules_query.c/.h      # Query functions (can_play, get_effective_rank, etc.)
├── rules_effects.c/.h    # Special effect implementations
└── rules_presets.c/.h    # Common rule configurations (standard, house variants)
```

This approach allows experimenting with crazy house rules while keeping the core game logic clean and the AI system consistent with whatever rules are configured!

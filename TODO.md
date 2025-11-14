# TODO

## Game Logic
- **AI logic** - Support swap phase and multi-card plays
- **Card configuration** - Modular rules system for card precedence and special effects

## Architecture
- **Game log system** - Data-driven log entries as source of truth for server/client sync
- **Terminal resize** - Pause game state with "resize terminal" overlay on insufficient size

## API Refactoring
- Replace hashmap with array for O(1) card visual lookup
- Better shuffle algorithm
- Cleanup filespace and API
- Compile to archive with embedded cards

## Future Work
- **Networked play** - Server room/client management, user IDs (focus on single-player first)
- **Lua AI system** - Extensible AI via Lua callbacks, game state representation API
- **libft allocator** - Configurable allocator for libft structs

## Settings menu
- (done not a good idea though, should move to some other model) Refresh timeout (how many cycles before refresh is triggered, refreshes increase flickering but reduce visual anomolise)
- (done)display name (for the multiplayer games)
- (done)Auto sort hand
- (done)AI timeout (how many cycles it takes for the AI to do something)
- Keybindings (arrow / vim)
 key bindings should just be both anyway, it shouldnt be a setting, we can just make a 
 macro for checking if the keys are the right ones.
- background colour ?


env vars:
SSHED_REFRESH - positive int less than INTMAX, 0 means no refresh (default 0)
SSHED_SORT_HAND - 1 or 0 (defualt 0)
SSHED_KEYBIND - 'vim' or 'arrow' (default arrow)
SSHED_AI_TIMEOUT - positive int gt 0 (default 10)
SSHED_BACKGROUND - hex code RGBA (default 00000000)
SSHED_DISPLAY_NAME - display name (default none, prompt user on multiplayer join/create)

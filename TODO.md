# TODO

## Game Logic
- **AI logic** - Support swap phase and multi-card plays
- **Card configuration** - Modular rules system for card precedence and special effects

## Architecture
- **Game log system** - Data-driven log entries as source of truth for server/client sync
- **Terminal resize** - Pause game state with "resize terminal" overlay on insufficient size
- **SSH persistent settings** - Pass env vars via sshd config, provide SSH command string


## API Refactoring
- Replace hashmap with array for O(1) card visual lookup
- Better shuffle algorithm
- Cleanup filespace and API
- Compile to archive with embedded cards

## Future Work
- **Networked play** - Server room/client management, user IDs (focus on single-player first)
- **Lua AI system** - Extensible AI via Lua callbacks, game state representation API
- **libft allocator** - Configurable allocator for libft structs

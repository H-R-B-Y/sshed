# SSHED (shed over ssh)

SSHED is a project to bring the card game shed to the terminal over an ssh connection. It is developmental and only really intended as a practice project.

## Installation

The base project is setup for use within a docker environment, though I have not actually finished the dockerfile in a standalone sense yet, right now development is done through ./devcontainer.json in a VSCode remote container.

The idea is that once the dockerfile is complete, you just run the container and can ssh into it to play the game.

## Usage

None right now, let me finish it :D

## TODO

### Core Game Logic
- **Swap Phase Implementation**: Currently skipped - need to implement card swapping between hand and shed
  - Track selected display element (hand vs shed)
  - Allow players to select one card from hand and one from shed to swap
  - Round-robin through players 3 times for swaps
  - Add UI element to skip swap phase
  
- **Multiple Card Plays**: Implement ability to play multiple cards of same rank
  - Add pile display above hand for queuing cards
  - Reset pile if different rank is selected
  - Add separate UI element to confirm play
  
- **Special Card Effects**:
  - Configurable card precedence and effects from rules menu
  - Modular system for adding new cards and house rules

### AI System
- **AI Architecture Rework**: Simplify AI interface with specific functions
  - `play_card_from_hand()` - play one or multiple cards from hand
  - `play_card_from_face_up_shed()` - play cards from visible shed
  - `play_card_from_face_down_shed()` - play card from hidden shed
  - Design for future LUA environment abstraction
  
- **AI Swap Phase**: Enable AI participation in swap phase
- **AI Multiple Cards**: AI should be able to play multiple cards like human players

### Code Quality & Architecture
- **Fix Duplicated Signatures in Headers**: Remove duplicate function declarations across header files
- **Card Library Rebuild**: Decouple cards from displays
  - Abstract hand/shed to collections of cards
  - Display logic takes pointer to collection and renders
  - Single central struct for players, bots, and server logic
  
- **Game State Management**: 
  - Data-driven game log system (not raw text)
  - Log entries as source of truth for server-client communication
  - Robust and easy-to-produce log format

- **Bug Fixes**:
  - **Card Play Bug**: Sometimes selected card doesn't play - turn logic advances but card not removed from hand

### User Interface
- **In-Game Help Menu**: Display key bindings and controls during gameplay
  - Show current control scheme (VIM vs normal mode)
  - Context-sensitive help for different game phases
  - Toggle overlay for quick reference

- **Control Schemes**: VIM vs normal key modes in settings menu
  - Normal mode: Arrow keys for navigation
  - VIM mode: hjkl for navigation
  - Persistent setting across sessions

- **Terminal Size Handling**: 
  - Check terminal size on start and each frame
  - Pause game with overlay if terminal too small
  - State machine must save previous state during pause

### Networking & Multiplayer
- **Multiplayer Backend Integration**: Hook up with existing multiplayer backend
  - Connect game logic to server infrastructure
  - Implement client-server communication protocol
  - Handle multiplayer game state synchronization

- **Server Implementation**: Core server code for multiplayer functionality
  - Game session management
  - Player connection handling
  - Game state distribution
  - Turn management and validation

- **SSH Environment Variables**: 
  - Pass env vars through sshd config
  - Provide SSH command strings for persistent settings
  - Allow configuration without reconnection

### Build System & Infrastructure
- **Docker Build System**: 
  - Compile binary during Docker build
  - Set up sshd server on specific port
  - Container deployment automation

- **Asset Pipeline**:
  - Better card spreading algorithm (best fit but not spread too far)
  - Compile to archive (include card assets)

- **Performance Optimization**:
  - Move hashmap to array of card visuals for O(1) lookup
  - Shared memory for multiple clients
  - Minimize per-client memory footprint

### Future Features
- **Animations** *(tentative)*: Terminal animations using sixel graphics
  - Card movement animations
  - Visual feedback for game actions
  - Note: Complex implementation, feasibility uncertain
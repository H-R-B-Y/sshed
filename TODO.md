# todo

- When connecing over ssh, we can allow some env vars to be passed in via the sshd config
 - then when the client changes their settings through the interface we can provide them with
	 the string they need to add to their ssh command when they connect next time to keep those settings.
	 this allows users to have persistent settings or to configure their settings without having to
	 re-configure them every time they connect.

- Need to rebuild the card library, right now cards are tied to the displays,
this limits the ability for the game logic to manage the cards independently, ideally
we can abstract the hand/shed to just be a collection of cards, and the display logic
just takes a pointer to a collection and renders it, this way we have one central struct that can be used
for both players, bots, and server logic, and the rendering just sits on top of that.

- Need to figure out swap phase, right now we just skip it because I dont know the best way to approach it.
I think one way to approach it would be, for the game_local_logic, we keep track of which element on the display is selected, and the stdin handler depends on this being set, if the element is the hand then left and right move the card selection, if the element is something else we do something else.
Then in swap phase we can just use a pile to display the shed cards, and allow the user to switch between them, they can select one card from their hand and one from the shed, and that will swap them.
Swaps typically happen all at the same time, but the logic I have written just takes case of one user action at a time, so we can just round robin through the players 3 times to allow them to swap cards, there should be a display element to skip the swap phase in cases where a player does not want to swap.

- Need to implement a way to queue up multiple cards in the play phase, for cards of the same rank,
In shed you can play multiple cards of the same rank at once, so I am thinking we keep a pile display above the hand and when a user selects a card from their hand it gets moved to the pile, if a card of a different rank is added to the pile it gets reset, and previous cards are added back to the hand.
Then we need a seperate UI element to confirm the play.

- AI logic.
The AI needs to be able to participate in the swap phase, and needs to be able to play multiple cards just like the user, this should be taken into account in the AI logic.

- Jokers need to burn the play pile.

- Need to come up with a good way to enable card configuration from the rules menu.
Some clean way to configure the play precendence of cards, and which cards have special effects.
Modular and extensible so that we can add new cards and effects later and allow them to be configured from the menu for house rules etc.

- There is a bug where sometimes playing the selected card without changing the selection causes the card to not be played, need to figure out what is failing here, the turn logic seems to tick over to the next player but the card does not get removed from hand or added to the play pile.

- Terminal size:
On start and on each frame we need to check if the terminal size has changed (this is done for us in the input handler, notcurses sends a resize event) but we need to make sure that the terminal is large enough to display the game, if not we need to pause the game (this could be a seperate state in the main state machine, that MUST SAVE THE PREVIOUS STATE) and display a message to the user to resize their terminal.

- Game log
Need to figure out a good way to store the game log, it should not store raw text it should be entirely data driven, but each log state maps directly to a text representation. The log should express the entire game state and possibly be the main source of truth that can be used for the server messages to the client? like the server just sends log entries to the client and the client re-builds the game state from that? that sounds like a good idea we just need to ensure the logs are robust and easy to produce.


## API

- Deck display (re-build the logic, dont worry about anon cards);


- Instead of a array of pointers to card descriptions, the deck should just contain an array of card descriptions (later)

- Move the hashmap of cards to an array of card visuals, this way we actually have O(1) lookup times. (later)

- Better shuffle logic (later)


- wrappers for adding and removing cards
Need to re-work this, the server needs to be able to address card by index in
a manner that is deterministic, specifically for the shed, for the hand it doesn't matter that much
because we never see the cards in the hand anyway.

- Prototype for having pdisplay that do not contain the cards that they hold, just work off the number of cards in the struct.


- REMOVE the anon card options for the pdisplay, we will build out a pdisplay_anon later for networked games


- Game setup

- cleanup the filespace
- use rotations from notcurses instead of rotating them with pillow
- cleanup the api

- Figure out a better way to spread the cards

- Compile to archive (perhaps we can include the cards as well)

## game

- Min terminal size (should be an overlay that pauses the game)

- Figure it out

# done


- When pdisplay or hand has too many cards and tries to draw them off the screen it causes wrapping, this is horrible because of the scrolling that happens.


- Split out the makefile targets for deleting:
  - images
  - libft
  - main binary

- Figure out a better way to store the cards

- Deck shuffling (simple shuffle)
- Build out hand display logic
- Build out pdisplay display logic

- Adjust pdisplay RIGHT padding from the edge
- Play pile display

- hand seleted card pointer

- test adding and removing cards to the pdisplay
- test adding and removing cards to the pdisplay shed

- Api for select next card / select prev card 

- Prototypes for having deck that does not contain the cards that it hold, just work off the number of cards in the struct.

- Figure out why last card in the shed is not being hidden
was to do with the number of cards being used as an array limit when that is not what it is for.
should still probably figure out a better way to represent the shed. TODO: better way to represent the shed.
Should probably re-work how the shed is done, I am not sure I like the way it is done.
Also sometimes the shed face downs cant be played.


- Move card description pointers to embedded structs, this will save us some memory safety.

# other

- TODO: add a way to change the allocator for libft structs


# Thoughts on the next steps

- Networked play
Since I have written the domain socket server and client model code, I want to make use of this
but I will need to add some stuff to the server to be able to manage rooms, and clients (like with user ids and what-not), as this is the ultimate end game for this project i think it would be best to focus on the single player version first.

- Single player version
For the single player version we want to built out something simple to start with, 
Main game states:
- Title
- Game setup (number of bots, maybe rule-set)
- Game playing (will probably have to have some substates or work through a seperate state machine)
- Menu (pause, exit, etc..)
- Game win/lose

Then we need to come up with a competent AI code for the player to play against, 
this is probably going to be the hardest part, we want to make it extensible, ideally with lua,
but that means we will need to come up with someway of representing the game state, and what "playing a card" looks like in terms of the lua api.

The best way to approach this would be to build out the state machine for the game first,
so thats like moving between the states, changing the display to match the state, figuring out the best way to do io with notcurses + epoll (future proofing for networked play).
Then we can do the game logic, probably good to have a generic function that handles a turn,
runs a callback to lua/ai or awaits input from the player, then decides the result of the turn, 
then callee deals with updating the displays.

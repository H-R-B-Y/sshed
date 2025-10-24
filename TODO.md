# todo

- Move card description pointers to embedded structs, this will save us some memory safety.

- Move the hashmap of cards to an array of card visuals, this way we actually have O(1) lookup times.

- Instead of a array of pointers to card descriptions, the deck should just contain an array of card descriptions

- wrappers for adding and removing cards
Need to re-work this, the server needs to be able to address card by index in
a manner that is deterministic, specifically for the shed, for the hand it doesn't matter that much
because we never see the cards in the hand anyway.

- Prototypes for having pdisplay and deck that do not contain the cards that they hold, just
work off the number of cards in the struct.s


- test adding and removing cards to the pdisplay
- test adding and removing cards to the pdisplay shed


- Deck display
Need to think about this, maybe it would be better to create a deck display
and a deck_online display, where one works off of the cards contained within it,
and the other works off a count of cards variable.

- Better shuffle logic

- Game setup

- cleanup the filespace
- use rotations from notcurses instead of rotating them with pillow
- cleanup the api

- Figure out a better way to spread the cards

# done

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

- Api for select next card / select prev card 


- Figure out why last card in the shed is not being hidden
was to do with the number of cards being used as an array limit when that is not what it is for.
should still probably figure out a better way to represent the shed. TODO: better way to represent the shed.
Should probably re-work how the shed is done, I am not sure I like the way it is done.
Also sometimes the shed face downs cant be played.


# other

- TODO: add a way to change the allocator for libft structs

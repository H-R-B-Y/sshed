# todo

- Figure out why last card in the shed is not being hidden
It is just the last card in the shed, not sure why it is happening,
last time this happened was because I was not cleaning up after myself correctly
this is probably a re-occurence of that.
And it only happens after playing a card.

Should probably re-work how the shed is done, I am not sure I like the way it is done.
Also sometimes the shed face downs cant be played.


- wrappers for adding and removing cards


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

decouple cards from displays


move hand display and pdisplay into one central (display)

architecture:

hand contianer
shed container
stack container

deck display
- contains one stack container

pile display
- contains one stack container

player display
- can contain mulitple containers
- only one visible at a time
- use array of containers

need some form of container union.

In the cards data there should be some
rules, that can be modified during runtime
that allow us to change how some cards work.

Like an effective max and min value
Is special flag
Stuff like that

all cards that are part of a set should have the same back.

card desc should be modifiable to fit user implementation,
loading the card visuals should be pluggable.

so:

card:
 - has a card descriptor
 - has a card visual for (
	up down left right
 )
 - Pointer to card set

card set:
 - contains a lookup of card descriptors
 - stores the card backs
 - contains an array of desc that is the master set of cards

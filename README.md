##About

This is a turn based strategy game engine built from scratch using the Simple DirectMedia Layer library.
Rather than use a traditional square based board, I've opted for a hexagonal approach to introduce more
strategic depth.

This is my first big project. I will, for the intent of learning and helping others learn with me, keep
the core gameplay engine open source. Once the actual game engine is finished, I will be moving this to
a private repository to continue work on my game. There is still lots of work to do!

Please bear with me; I know my code is a huge mess. I will go back once everything is in place and try
to optimize things/remove unneeded code.

##What's to come

- Structures and capturing
- Resource management
- Building units from said structures using said resources
- More in-depth damage formula (current one is a placeholder)
- Supply vehicles
- Transport vehicles
- End turn animated screen
- Dynamic fog of war

##What's been done as of now

- Hex tiled board
- Framework for a primitive map editor
- Units with customizable loadouts (customize aircraft/vehicles/infantry)
- On screen animated cursor
- Idle animation states
- Combat animation states and damage indicator numbers
- Modular on screen menu with action buttons
- End turn
- Unit movement tiles and primary/support attack tiles highlights
- Combat system
- Zone of control
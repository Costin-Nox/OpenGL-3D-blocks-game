BY: Costin Ghiocel
SN: 301027183
------------------
requires: C++11
------------------

To run:

> make
> ./FruitTetris

Features:

- added the square block with full rotation range.
- added full rotation range to all other blocks that only had 2 (to make it more intuitive)
- blocks respect physics: if blocks underneath get removed, things will shift down to compensate
- in the case of supports being removed, there will be no floating blocks.
 	-the program detects object clusters, if it finds one without support, ie: floating, it will move the entire cluster down until one of the bottom tiles for the cluster is supported by something
 		- this looks a bit odd as it's done instantly (did not finish figuring out animations) and this can also chain other events such as a row clear or more color clears.
- if more than 4 blocks of the same color are in a row, it will clear all, not just 4 (seems more interesting this way)

- Sorry for some lack of comments, was running out of time.
 
ncurses-selection
=================

An ncurses menu for selecting a subset of given lines.

Reads newline-separated choices from standard input, allows the user
to choose a subset of them, and writes what's selected to standard
output.

Keys
----
- arrow keys — move the cursor
- `Space` — (un)select current choice
- `Return` or `q` — accept the selection and quit

Example usage
-------------
Select files from `~` and delete them
```
ls -1d ~/* | ./sel | xargs -d$'\n' rm
```

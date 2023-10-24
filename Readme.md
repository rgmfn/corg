# corg
An emacs org-file reader written with ncurses in C.

I personally use org files mostly for their list, todo, and agenda functionalities, so the app is focused around those specific features.

# Quick start
* Dependencies
 - `ncurses`
 - `clang`
 - `make`
## Compilation
In corg/src, run make.
```
> make
```

## Running
Run command corg with the file path of an org file as the argument.
```
> ./corg /path/to/file.org
```

Additionally, you can run corg with no arguments to open a new, blank, unnamed file.
```
> ./corg
```

Press ? in the app for a list of commands.

## Current Features
- Can create, move, edit and delete org headings
- Can give descriptions and dates to these headings
- Can set the TODO status of headings
- Can open and save org files

## Planned Features
- Support #+title and #+startup
- A selection mode to modify multiple lines at once
- The ability to add web links to nodes and open them from inside the program
- A better help menu (with a search bar)

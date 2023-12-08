# corg
An emacs org-file reader written with ncurses in C.

I personally use org files mostly for their list, todo, and agenda functionalities, so the app is focused around those specific features.

Currently it only runs on Mac because Mac allows for the C POSIX regex library to use non-greedy quantifiers. Once I figure out another solution for non-greedy quantifiers, it should work on linux.

# Quick start
## Dependencies
  - `ncurses`
  - `clang`
  - `make`
## Compilation
In corg/src, run make.
```
make
```

## Running
Run command corg with the file path of an org file as the argument.
```
./corg /path/to/file.org
```

Additionally, you can run corg with no arguments to open a new, blank, unnamed file.
```
./corg
```

Press ? in the app for a list of commands.

# Features

## Current Features
- Can create, move, edit and delete org headings
- Can give descriptions and dates to these headings
- Can set the TODO status of headings
- Can open and save org files

https://www.youtube.com/watch?v=QgYhuPgbp3s

## Planned Features
- Support #+title and #+startup
- A selection mode to modify multiple lines at once
- The ability to add web links to nodes and open them from inside the program
- A better help menu (with a search bar)
- Linux support

## Known Issues
- Headings break past 18 indents (head node is 0 indents)
    - or 19 stars in an org file
- Headings can only be 100 charactes and descriptions can only be 200 characters
    - this works for my workflow, may expand them in the future
- Can't write a description past 50 characters
    - will be fixed along with a multiline text input box

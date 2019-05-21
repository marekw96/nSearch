# nSearch
Simple TUI (terminal user interface) adapter for grep command with preview feature.

## How to 
*	`git clone --recursive https://github.com/marekw96/nSearch`
* `cd nSearch`
* `mkdir build`
* `cd build`
* `cmake ..`
* `make`
* ` ./nSearch`

## How it looks
[![Main Window](https://github.com/marekw96/nSearch/blob/master/doc/mainwindow.png "Main Window")](https://github.com/marekw96/nSearch/blob/master/doc/mainwindow.png "Main Window")

## Settings

In *~/.local/.nSearch* you can create your config file. Suppored properties so far:
* OPEN_PREVIEW true/false - when false do not open file preview automatilcy [default: true]

In example:
```
OPEN_PREVIEW false
```

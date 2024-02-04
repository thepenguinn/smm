# Simple(Stupid) Matrix Multiplier

Simple matrix multiplier is a tui C program for all things matrix.
(At least that's what its trying to be)

# What's done ?

Currently the UI is almost done, the matrix window and the main window are
finished. We can navigate between the matrix window and main window and between
different matrices. But currently `smm` don't have any way to take user input to
modify the values in each matrices.

## Demo

![](./demo/demo.gif)

Yes, the UI is heavily inspired by the tui markdown reader `glow`.
Check them out [glow](https://github.com/charmbracelet/glow)!

# Building smm

In order to build `smm` you need `ncurses` library, `git` and `make`
installed. Then run these commands.

```
git clone https://github.com/thepenguinn/smm
cd smm
make
```

This will produce a binary named `smm` at the root of the project directory

# Disclaimer

`smm` is still in #wip, so as the rest of the README

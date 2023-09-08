# Build
```sh
cc -o open_pty printfd.c main.c
```

# Usage
```sh
./open_pty
```

You will see `set inferior-tty /path/to/a/pty`. Paste this into gdb or whatever. This sets both `stdout` and `stderr` of the underlying program of gdb to that pty.

Then redirect stderr of your running program to the stderr pty which is suggested by `2>/path/to/another/pty`.

This is the last argument given to `run` on gdb (as if you provided command line arguments to your program).


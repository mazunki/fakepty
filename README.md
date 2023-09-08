# Build
```sh
cc -o open_pty printfd.c main.c
```

# Usage
```sh
./open_pty [name]
```

`name` defaults to `default`, and will generate two symlinks to ptys in `/tmp/openpty/`, as `/tmp/openpty/${name}_{stdout,stderr}`.

If compiled with `-DVERBOSE=1`, you will see `set inferior-tty /dev/pty/somenum`. You generally don't need to know the actual pty path (which you would be able to see through an `ls -l /tmp/openpty/` anyway.

# Motive

`gdb` allows you to `set inferior-tty /path/to/some/pty`, which sets both stdout and stderr of the executed program to that pty. It doesn't accept a regular or socket files, though.

Because of this, we open a pair of `pty`s, and create symlinks to well-known paths: `/tmp/openpty/gdb_std{out,err}`, which we can then use in our scripts.

It's also nice to be able to run `open_pty silly` in one terminal, and just `exec 2>&/tmp/openpty/silly_stderr` in another terminal to push all its subsequent stderr into the other terminal.

Anyway, regardin gdb, you may put `set inferior-tty /tmp/openpty/gdb_stdout` in your `~/.gdbinit`, or similar. Note that this will send both the stdout and the stderr of the executed program to the fakepty's standard output (fd=1).

To deal with this, and be able to separate stderr from stdout, you may pass `2>/tmp/openpty/gdb_stderr` to gdb's `run` arguments. Note that you do not want to set `2>` when invoking gdb from the shell (unless you want gdb's error output printed on the fakepty too).


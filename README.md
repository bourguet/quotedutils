# quotedutils

Two small utilities which display information using C string syntax.

## ccat

Behaves like cat, but quote non printable characters with C syntax.  It has
an option to print C strings allowing the output to be fed to a C compiler.

## showargs

Use the C string syntax to display its arguments.  It is usefull to debug
some situation, like Makefiles, where various level of quoting are
interacting.

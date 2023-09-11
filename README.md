# Quick C Compiler

The Quick C Compiler (qcc) is a retargetable, optimizing and self-hosting
C compiler for the ISO C99 standard written from scratch.

For more information on the internal architecture and implementation of the
compiler, please view the [documentation](/docs). The eventual goal of this
project is to be capable of compiling itself (self-hosting) as well as large
scale and relevant code bases such as [sqlite](https://github.com/sqlite/sqlite)
and [lua](https://github.com/lua/lua).

This is a personal project of mine, which in large part implies the fact that
it isnt nearly as robust or mature as a decently capable compiler would be. Keep
in mind that this project is likely riddled with bugs en masse.

## Status

Current status of the project. Update once we have a fully working skeleton
implemented and update this to keep track of progress + todo.

* Currently working on implementing the incremental lexer, preprocessor and
  the parser. These frontend components all execute in the same pass due to
  the lexer and preprocessors incremental nature.

## Build

To build the project run the following command.

```sh
make
```

If you want to compile and install qcc, run the following command with sudo
in the root directory of the repository.

```sh
sudo ./scripts/install.sh
```

You can uninstall the project by passing the `--uninstall` flag to `install.sh`.

```sh
sudo ./scripts/install.sh --uninstall
```

For the unit tests, there is a tool provided which automatically tests the
project. To do so run the following command.

```sh
./tools/run_tests.sh
```

## License

This project is licensed under the [MIT](/LICENSE) license.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

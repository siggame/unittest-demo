# 0 - unittests & doxygen

This repo demonstrates a simple framework for unit testing and documentation.
We will go over the repo dependencies, how it works, its features, and how to
use it.

# Dependencies

```bash
sudo apt install gcov       # Generate line data
sudo apt install gcovr      # Report generator for gcovr
sudo apt install doxygen    # Documentation generator
```

## How does it work?
In the root directory, there is a makefile and a unittests directory. Invoking

```
make
```

This compiles all .cpp files into .o files twice. The first compilation simply builds
the file with the specified flags. The second time builds with the `--coverage`
flag and places it in the directory.

```
make test
```

This compiles everything `make` does and it runs the test suite.

## Unit Testing Features

Invoking `make test` does 4 things

1. Unit testing - This repo uses catch.hpp to run unit testing. To read more about
catch.hpp, go here.

2. Integrated memory checking with valgrind - Unit tests are run using valgrind
and a leak summary is shown afterwards.

3. Collecting coverage and branch info - Because the unit test is compiled with
`--coverage` flag, when the test is executed, coverage data is generated.

4. Reporting - Taking data generated from executing the unit test, the `gcovr`
binary creates html report and places it in `./unittests/coverage`

## Doxygen

Invoking doxygen with the given Doxyfile creates a doc folder

```
doxygen
```

# How to use it?

First generate the data

1. Run the tests with `make test` command
2. Generate documentation with `doxygen` command
3. Depending on where the repo is located, there are several options
    1. Local Machine - Open a browser and navigate to `file:///<REPO-DIRECTORY>`
    2. WSL / Remote SSH and using VSCode: <b>
        To view the files remotely, we need to serve the files over http and forward the port. VSCode does this automatically
        In the root repo directory, run `python3 -m http.server` <b>
        If port 8000 is already occupied, specify a port. `python3 -m http.server <unused port number>` <b>
    3. WSL / Remote SSH without using VSCode:
        Same steps above, but we need to manually forward the port
        [TODO] tutorial on port forwarding over SSH

4. Start writing code and unit tests. Read more [HERE](..TODOLINK)


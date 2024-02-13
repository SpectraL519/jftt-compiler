# jftt-compiler

This is a final project as part of the **Formal languages ​​and translation techniques** course at the **Wrocław University of Science and Technology**.

### Project overview

Simple imperative language compiler. The language specification in Polish can be found [here](specification.pdf).

This compiler performs a naive translation (without any intermediate code representation stages).

<br />

### Used tools
* GNU Make 4.3
* GNU G++ 12.3.0
* GNU Flex 2.6.4
* GNU Bison 3.8.2
* System: WSL Ubuntu 22.04.3 LTS
* C++ standard: 17

<br />

### Compiler usage

* Build the compiler: `make compiler`

    This will create a `compiler` executable in the project root directory.

* Compile a source file:

    ```shell
    ./compiler <input-file-path> <output-file-path>
    ```

    > **NOTE:** The convention is to name the input files with the `.imp` and the output files with the `.mr` extensions.

* Build the virtual machine: `make vm`

    This will create a `run-vm` and `cln-run-vm` executables.

    To run a compiled source file run the following command:

    ```shell
    ./run-vm <input-file-path>
    ./cln-run-vm <input-file-path>
    ```

    > **NOTE:** The virtual machine expects a file with the **machine readable** code (by convention files with `.mr` extension) as an input.

    > **NOTE:** The `cln-run-vm` executable is built with the [libcln-dev 1.3.6](https://packages.ubuntu.com/en/focal/libcln-dev) library.

<br />

### Examples

The example imperative programs can be found in the `test/` directory.

To compile all files inside a directory you can use the `compile_dir.py` script.

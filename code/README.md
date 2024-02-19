# Project Setup Guide

## Prerequisites

This guide assumes you are using the following:

- macOS operating system with Apple Silicon (M1, M2, etc.)
- Homebrew installed ([https://brew.sh](https://brew.sh))

## Installation

### Step 1: Install LLVM with Homebrew

OpenMP requires LLVM with `clang` that supports the OpenMP library. macOS's default compiler does not include OpenMP support, so you'll need to install LLVM via Homebrew:

```sh
brew install llvm
```

### Step 2: Compiling Project

The Makefiles will specify the Homebrew-installed `clang` compiler and necessary flags for OpenMP as follows:

```makefile
CC=/opt/homebrew/opt/llvm/bin/clang
CXX=/opt/homebrew/opt/llvm/bin/clang++
CFLAGS=-fopenmp
LDFLAGS=-L/opt/homebrew/opt/llvm/lib -Wl,-rpath,/opt/homebrew/opt/llvm/lib
CPPFLAGS=-I/opt/homebrew/opt/llvm/include
```

### (Optional) Step 3: Configure Your Environment  (Again, only if it's not your first time!)

To use the Homebrew-installed LLVM, you need to add it to your `PATH`, and set `LDFLAGS` and `CPPFLAGS` to help the compiler and linker find the correct libraries and include files:

Open your terminal and run (replace `.zshrc` with `.bashrc` if you use Bash):

```sh
echo 'export PATH="/opt/homebrew/opt/llvm/bin:$PATH"' >> ~/.zshrc
echo 'export LDFLAGS="-L/opt/homebrew/opt/llvm/lib"' >> ~/.zshrc
echo 'export CPPFLAGS="-I/opt/homebrew/opt/llvm/include"' >> ~/.zshrc
```

If your project requires the bundled `libc++`:

```sh
echo 'export LDFLAGS="-L/opt/homebrew/opt/llvm/lib/c++ -Wl,-rpath,/opt/homebrew/opt/llvm/lib/c++"' >> ~/.zshrc
```

After adding these lines, apply the changes:

```sh
source ~/.zshrc
```


## Usage

Follow the Makefile's valid targets.

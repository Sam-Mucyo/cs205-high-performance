CC=/opt/homebrew/opt/llvm/bin/clang
CXX=/opt/homebrew/opt/llvm/bin/clang++
CFLAGS=-fopenmp
LDFLAGS=-L/opt/homebrew/opt/llvm/lib -Wl,-rpath,/opt/homebrew/opt/llvm/lib
CPPFLAGS=-I/opt/homebrew/opt/llvm/include

.PHONY: clean

pi: pi.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o pi pi.cpp

clean:
	rm -f pi

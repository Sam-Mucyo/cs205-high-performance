CC=/opt/homebrew/opt/llvm/bin/clang
CXX=/opt/homebrew/opt/llvm/bin/clang++
CFLAGS=-fopenmp
LDFLAGS=-L/opt/homebrew/opt/llvm/lib -Wl,-rpath,/opt/homebrew/opt/llvm/lib
CPPFLAGS=-I/opt/homebrew/opt/llvm/include

.PHONY: clean

counter: counter.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o counter counter.cpp

clean:
	rm -f counter

CC=/opt/homebrew/opt/llvm/bin/clang
CXX=/opt/homebrew/opt/llvm/bin/clang++
CFLAGS=-fopenmp -Wall -Wextra
LDFLAGS=-L/opt/homebrew/opt/llvm/lib -Wl,-rpath,/opt/homebrew/opt/llvm/lib
CPPFLAGS=-I/opt/homebrew/opt/llvm/include

.PHONY: clean

%: %.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $< -o $@

# %: %.cpp is a pattern rule that tells make how to build any target % from a dependency with the same name but with a .cpp extension. For example, if you run make program_name, it looks for program_name.cpp.
# $< is an automatic variable that represents the first dependency of the rule, which in this case is the source file (%.cpp).
# $@ is an automatic variable that represents the target name (%).

clean:
	rm -f *.o *~ core

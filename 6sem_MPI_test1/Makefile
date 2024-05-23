CXXFLAGS = -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format
OPT = -O3 #-ffast-math -march=native #-fsanitize=address
all: a.out

a.out: main.o input.o sol.o
	mpicxx main.o input.o sol.o -o a.out

input.o: input.cpp input.hpp
	mpicxx -c $(CXXFLAGS) $(OPT) input.cpp

sol.o: sol.cpp sol.hpp
	mpicxx -c $(CXXFLAGS) $(OPT) sol.cpp

main.o: main.cpp input.hpp sol.hpp
	mpicxx -c $(CXXFLAGS) $(OPT) main.cpp

clean:
	rm -f *.o a.out

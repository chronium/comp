CC=clang-12
CXX=clang++-12
LD=$(CXX)

CFLAGS=$(shell llvm-config-12 --cflags)
CXXFLAGS=$(shell llvm-config-12 --cxxflags --ldflags --libs core executionengine mcjit interpreter analysis native bitwriter --system-libs)

all: comp

run: all
	valgrind ./comp test.c
	lli-12 out.bc
	# llvm-dis-12 comp.bc

tests: comp
	$(MAKE) --directory=$@

comp: comp.o
	$(CXX) $(CFLAGS) $(CXXFLAGS)  $^ -o $@

comp.o: comp.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f comp *.bc *.ll *.o

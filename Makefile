CC=clang-12
CXX=clang++-12
LD=$(CXX)

CFLAGS=$(shell llvm-config-12 --cflags)
CXXFLAGS=$(shell llvm-config-12 --cxxflags --ldflags --libs core executionengine mcjit interpreter analysis native bitwriter --system-libs)

all: comp

run: ARGS=$(filter-out $@,$(MAKECMDGOALS))
run: OUT=$(lastword $(ARGS:.c=.bc))
run: all 
	valgrind ./comp $(ARGS) $(OUT)
	#lli-12 $(OUT) $(ARGS) $(OUT)
	lli-12 $(OUT) straptest.c straptest.bc

tests: comp
	$(MAKE) --directory=$@

comp: comp.o
	$(CXX) $(CFLAGS) $(CXXFLAGS)  $^ -o $@

comp.o: comp.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f comp *.bc *.ll *.o

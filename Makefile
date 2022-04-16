CC=clang-12
CXX=clang++-12
LD=$(CXX)

CFLAGS=$(shell llvm-config-12 --cflags)
CXXFLAGS=$(shell llvm-config-12 --cxxflags --ldflags --libs core executionengine mcjit interpreter analysis native bitwriter --system-libs)

all: bcomp

run: ARGS=$(filter-out $@,$(MAKECMDGOALS))
run: OUT=$(lastword $(ARGS:.c=.bc))
run: all 
	valgrind ./bcomp $(ARGS) $(OUT)
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(OUT) -o $(OUT:.bc=)
	valgrind ./$(OUT:.bc=) comp.c stage3.bc
	lli-12 stage3.bc comp.c out.bc

self: ARGS=$(filter-out $@,$(MAKECMDGOALS))
self: OUT=$(lastword $(ARGS:.aat=.bc))
self: all
	valgrind ./comp comp.c tmpcomp.bc
	$(CXX) $(CFLAGS) $(CXXFLAGS) tmpcomp.bc -o tmpcomp
	valgrind ./tmpcomp $(ARGS) $(OUT)

tests: comp
	$(MAKE) --directory=$@

bcomp: bcomp.o
	$(CXX) $(CFLAGS) $(CXXFLAGS)  $^ -o $@

bcomp.o: comp.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f comp *.bc *.ll *.o bcomp *.out

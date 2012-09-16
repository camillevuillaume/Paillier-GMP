CC = gcc
CFLAGS = -Wall -lgmp -lpthread -DPAILLIER_THREAD
DEPS = src/paillier.h src/tools.h
OBJ_DEBUG = debug/tools.o debug/paillier.o debug/paillier_manage_keys.o debug/paillier_io.o debug/main.o 
OBJ_LIB = release/tools.o release/paillier.o release/paillier_manage_keys.o release/paillier_io.o
OBJ_RELEASE = $(OBJ_LIB) release/main.o 

#debug recipes
debug/%.o: src/%.c $(DEPS)
	mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS) -ggdb -DPAILLIER_DEBUG

debug/paillier: $(OBJ_DEBUG)
	$(CC) -o $@ $^ $(CFLAGS) -ggdb -DPAILLIER_DEBUG

#release recipes
release/%.o: src/%.c $(DEPS)
	mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)

release/paillier: $(OBJ_RELEASE)
	$(CC) -o $@ $^ $(CFLAGS)

#static library recipe	
release/libpaillier.a: $(OBJ_LIB)
	$(AR) rvs $@ $^

#documentation recipe
.PHONY: doc
doc:
	mkdir -p doc
	doxygen

#clean project
.PHONY: clean
clean:
	rm -fr release/*.o debug/*.o doc/*
	rm -fr release/Paillier debug/Paillier

debug: debug/paillier
release: release/paillier
lib: release/libpaillier.a
all: release doc lib

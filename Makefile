CC = gcc
CFLAGS = -Wall -Werror -c -lpthread -DPAILLIER_THREAD -fpic
DEPS = include/paillier.h src/tools.h
OBJ_LIB = build/tools.o build/paillier.o build/paillier_manage_keys.o build/paillier_io.o
OBJ_INTERPRETER = build/main.o 

#standaloine command interpreter executable recipe	
build/paillier_standalone: build/main.o $(OBJ_LIB)
	$(CC) -Wall -o $@ $^ -lgmp

#command interpreter executable recipe	
build/paillier: build/main.o lib/libpaillier.so
	$(CC) -Wall -o $@ $< -Llib -l:libpaillier.so -lgmp

#shared library recipe	
lib/libpaillier.so: $(OBJ_LIB)
	$(CC) -shared -o $@ $^

#release recipes
build/%.o: src/%.c $(DEPS)
	mkdir -p $(@D)
	$(CC) -o  $@ $< $(CFLAGS)

#documentation recipe
.PHONY: doc
doc:
	mkdir -p doc
	doxygen

#clean project
.PHONY: clean
clean:
	rm -fr build/* doc/* lib/* test/*.txt

debug: build/paillier
debug: CFLAGS += -ggdb -DPAILLIER_DEBUG
release: build/paillier
standalone: build/paillier_standalone
lib: lib/libpaillier.so
all: release doc lib

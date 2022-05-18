CC = gcc
#CFLAGS = -Wall -lgmp -lpthread -DPAILLIER_THREAD
CFLAGS = -Wall -Werror -c -DPAILLIER_THREAD -fpic
DEPS = include/paillier.h src/tools.h
OBJ_LIB = build/tools.o build/paillier.o build/paillier_manage_keys.o build/paillier_io.o
OBJ_INTERPRETER = build/main.o 

#command interpreter executable recipe	
build/paillier: build/main.o lib/libpaillier.so
	$(CC) -Wall -o $@ $< -Llib -l:libpaillier.so -lgmp

#static library recipe	
lib/libpaillier.so: $(OBJ_LIB)
#	$(AR) rvs $@ $^
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
lib: lib/libpaillier.so
all: release doc lib

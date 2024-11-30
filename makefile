OUT     = trypt
INSTALL = /usr/bin/$(OUT)
SRC     = $(wildcard src/*.c)
DEPS    = $(wildcard src/*.h)
OBJ     = $(addsuffix .o,$(subst src/,bin/,$(basename $(SRC))))

CFLAGS = -pedantic -Wpedantic -Wshadow -Wvla -Wuninitialized -Wundef -Wno-deprecated-declarations \
         -Wall -Wextra -std=c99
LDFLAGS = -lgcrypt

.PHONY: debug release install uninstall clean all

debug: CFLAGS  += -Werror -DDEBUG -g -Og
debug: LDFLAGS += -fsanitize=address
debug: $(OUT)

release: CFLAGS += -DRELEASE -g0 -O2
release: $(OUT)

$(OUT): bin $(OBJ) $(SRC)
	$(CC) -o $(OUT) $(CFLAGS) $(OBJ) $(LDFLAGS)

bin/%.o: src/%.c $(DEPS)
	$(CC) -c $< $(CFLAGS) -o $@

bin:
	mkdir -p bin

install: $(OUT)
	cp $(OUT) $(INSTALL)

uninstall: $(INSTALL)
	rm $(INSTALL)

clean: bin
	rm -f -r bin/*
	rm -f $(OUT)

all:
	@echo debug, release, install, uninstall, clean

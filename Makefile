EXEC=bsk

CFLAGS=-Wall -Wextra -Werror -pedantic -Wshadow -D_POSIX_C_SOURCE=200809L -std=c11 -fstack-protector-all -fpie -O3 -D_FORTIFY_SOURCE=2
LDFLAGS=-fpie
SOURCES=$(wildcard *.c)
DEPENDS=$(patsubst %.c,.%.depends,$(SOURCES))
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

all: $(EXEC)

$(EXEC): $(EXEC).o $(OBJECTS)

.%.depends: %.c
	$(CC) $(CFLAGS) -MM $< -o $@

.PHONY: clean
clean:
	$(RM) *.o $(EXEC) $(DEPENDS)

-include $(DEPENDS)

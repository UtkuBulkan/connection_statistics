CFLAGS := -fPIC -O3 -g -Wall -DLOG_LEVEL=0
CC := gcc
LDFLAGS = -shared  # linking flags
RM = rm -f  # rm command
MAJOR := 1
MINOR := 0
NAME := conn_stats
VERSION := $(MAJOR).$(MINOR)
INC = -I/usr/include/x86_64-linux-gnu/curl -I.
LOG_LEVEL=0

lib: lib$(NAME).so.$(VERSION)

test: $(NAME)_test
	LD_LIBRARY_PATH=. ./$(NAME)_test -n 5

$(NAME)_test: lib$(NAME).so
	$(CC) test/$(NAME)_test.c -I./src -L. -l$(NAME) -lcurl -o $@

lib$(NAME).so: lib$(NAME).so.$(VERSION)
	ldconfig -v -n .
	ln -sf lib$(NAME).so.$(VERSION) lib$(NAME).so

lib$(NAME).so.$(VERSION): src/median.c src/connect_manager.c src/connect_manager_curl.c src/connect_statistics.c
	$(CC) $(LDFLAGS) $(CFLAGS) $(INC) $^ -o $@

clean:
	$(RM) $(NAME)_test *.o *.so*

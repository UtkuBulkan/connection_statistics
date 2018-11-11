CFLAGS := -fPIC -O3 -g -Wall
CC := gcc
LDFLAGS = -shared  # linking flags
RM = rm -f  # rm command
MAJOR := 1
MINOR := 0
NAME := conn_stats
VERSION := $(MAJOR).$(MINOR)
INC = -I/usr/include/x86_64-linux-gnu/curl

lib: lib$(NAME).so.$(VERSION)

test: $(NAME)_test
	LD_LIBRARY_PATH=. ./$(NAME)_test -n 2

$(NAME)_test: lib$(NAME).so
	$(CC) $(NAME)_test.c -L. -l$(NAME) -lcurl -o $@

lib$(NAME).so: lib$(NAME).so.$(VERSION)
	ldconfig -v -n .
	ln -sf lib$(NAME).so.$(VERSION) lib$(NAME).so

lib$(NAME).so.$(VERSION): median.c connect_manager.c connect_manager_curl.c connect_statistics.c
	$(CC) $(LDFLAGS) $(CFLAGS) $(INC) $^ -o $@

clean:
	$(RM) $(NAME)_test *.o *.so*

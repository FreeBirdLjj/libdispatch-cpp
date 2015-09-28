INC = -I.
OBJS = dispatch-c++.o
LIB_BIN = libdispatch-c++.a
TEST_BIN = test_main

MY_CPPFLAGS ?=
MY_CXXFLAGS ?= -Wall -Ofast -funroll-loops -flto

AR = ar
ARFLAGS = rcu
RANLIB = ranlib
CPPFLAGS  = $(INC) -Dtest_main=main $(MY_CPPFLAGS)
CXXFLAGS  = -std=c++1y $(MY_CXXFLAGS)
LOADLIBES = -L.
LDLIBS = -ldispatch-c++

.PHONY: all clean test

all: $(LIB_BIN)

$(LIB_BIN): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	$(RM) $(LIB_BIN) $(OBJS) $(TEST_BIN)

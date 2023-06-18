.PHONY: all install uninstall clean dvi dist launch tests style valgrind

CFLAGS = -Wall -Werror -Wextra -std=c++17 -pedantic -g
LDFLAGS = $(shell pkg-config --cflags --libs gtest)
APP_NAME = 3DViewer

OS := $(shell uname -s)
ifeq ($(OS), Linux)
  OPEN = ./build/$(APP_NAME)
endif
ifeq ($(OS), Darwin)
	OPEN = open build/$(APP_NAME).app
endif

all: install launch

install:
	cmake -B build
	make -C build 3DViewer

uninstall:
	$(MAKE) clean
	rm -rf build

clean:
	rm -rf valgrind.txt 3DViewer.tar.gz
	rm -rf test

dvi:
	open ../README.md

dist:
	$(MAKE) install
	tar -czf $(APP_NAME).tar.gz --directory=build/ .

launch:
	$(OPEN)

tests:
	g++ tests/*.cc model/*.cc $(CFLAGS) $(LDFLAGS) -o test
	./test

style:
	clang-format -style=Google -n controller/*.cc controller/*.h model/*.cc model/*.h
	clang-format -style=Google -n view/*.cc view/*.h tests/*.cc tests/*.h
	clang-format -style=Google -n view/QtGifImage/gifimage/* view/QtGifImage/giflib/*
	clang-format -style=Google -n main.cc

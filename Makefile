CC := g++
CC_FLAGS += -DPLATFORM_PC --std=c++98 -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused -Wmaybe-uninitialized -Wuninitialized

.PHONY: all clean
all: post-build

pre-build:
	@echo PREBUILD
	mkdir -p obj
	cat sample7 main.cc > combined.cc

post-build: main-build
	@echo POSTBUILD

main-build: pre-build
	@$(MAKE) --no-print-directory main-target

main-target: obj/complexis

obj/complexis: obj/combined.o
	$(CC) $(CC_FLAGS) obj/combined.o -o obj/complexis -lSDL2 -lSDL2_gfx

obj/combined.o: main.cc
	$(CC) $(CC_FLAGS) -c combined.cc -o obj/combined.o

clean:
	rm obj/combined.o obj/complexis


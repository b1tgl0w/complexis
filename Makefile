CC := g++
ifeq ($(MAKELEVEL),0)
export CC_FLAGS = -DPLATFORM_PC --std=c++98 -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused -Wmaybe-uninitialized -Wuninitialized -D_REENTRANT -I/home/cy/lib/include/SDL2/
endif

.PHONY: all clean
all: post-build

debug-mode: CC_FLAGS += -DDEBUG_MODE
debug-mode: all

pre-build:
	@echo PREBUILD
	mkdir -p obj

post-build: main-build
	@echo POSTBUILD

main-build: pre-build
	@$(MAKE) --no-print-directory main-target

main-target: obj/complexis

#$(CC) $(CC_FLAGS) obj/main.o -o obj/complexis -lSDL2 -lSDL2_gfx -lSDL2_image
#$(CC) $(CC_FLAGS) obj/main.o -o obj/complexis -Wl,-Bstatic /home/cy/elib/lib/libSDL2.a /home/cy/elib/lib/libSDL2main.a /home/cy/elib/lib/libSDL2_image.a -Wl,-Bdynamic -lpthread -ldl -lm

obj/complexis: obj/main.o
	$(CC) $(CC_FLAGS) obj/main.o -o obj/complexis -lSDL2 -lSDL2_gfx -lSDL2_image
	#$(CC) $(CC_FLAGS) obj/main.o -o obj/complexis -Wl,-Bstatic /home/cy/elib/lib/libSDL2.a /home/cy/elib/lib/libSDL2main.a /home/cy/elib/lib/libSDL2_image.a -Wl,-Bdynamic -lpthread -ldl -lm

obj/main.o: main.cc
	$(CC) $(CC_FLAGS) -c main.cc -o obj/main.o

clean:
	rm obj/main.o obj/complexis


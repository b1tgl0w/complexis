ENVP= MACOSX_DEPLOYMENT_TARGET=10.6
ifneq (,$(findstring /cygdrive/,$(PATH)))
	UNAME := Cygwin
else
	ifneq (,$(findstring WINDOWS,$(PATH)))
		UNAME := Windows
	else
		UNAME := $(shell uname -s)
	endif
endif
ifeq ($(UNAME),Cygwin)
	CC := i686-w64-mingw32-gcc-11
else
	CC := gcc
endif
CC := gcc
ifeq ($(MAKELEVEL),0)
export CC_FLAGS = -DPLATFORM_PC -ferror-limit=300 -std=c17 -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused -Wuninitialized
endif

ifeq ($(UNAME),Darwin)
else
  export CC_FLAGS += -Wlogical-op -Wnoexcept -Wstrict-null-sentinel -Wmaybe-uninitialized
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

#$(ENVP) $(CC) $(CC_FLAGS) obj/main.o -o obj/complexis -lSDL2 -lSDL2_gfx -lSDL2_image
#$(ENVP) $(CC) $(CC_FLAGS) obj/main.o -o obj/complexis -Wl,-Bstatic /home/cy/elib/lib/libSDL2.a /home/cy/elib/lib/libSDL2main.a /home/cy/elib/lib/libSDL2_image.a -Wl,-Bdynamic -lpthread -ldl -lm

obj/complexis: obj/main.o
	$(ENVP) $(CC) $(CC_FLAGS) obj/main.o -o obj/complexis -lSDL2 -lSDL2_gfx -lSDL2_image -lpthread -ldl -lm
	#$(ENVP) $(CC) $(CC_FLAGS) obj/main.o -o obj/complexis -Wl,-Bstatic /home/cy/elib/lib/libSDL2.a /home/cy/elib/lib/libSDL2main.a /home/cy/elib/lib/libSDL2_image.a -Wl,-Bdynamic -lpthread -ldl -lm

obj/main.o: main.c
	$(ENVP) $(CC) $(CC_FLAGS) -c main.c -o obj/main.o

clean:
	rm obj/main.o obj/complexis


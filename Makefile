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
export CC_FLAGS = -DPLATFORM_PC -std=c89 -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-declarations -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused -Wuninitialized
endif
ifdef ASANMODE
CC_FLAGS += -ggdb -fno-omit-frame-pointer -fsanitize=undefined -fno-common -O0
EXTRA_LD := -lasan -lSDL2 -lSDL2main -lSDL2_gfx -lSDL2_mixer -lSDL2_image -lSDL2_net -lpthread -ldl -lm
else
EXTRA_LD := -lSDL2 -lSDL2main -lSDL2_gfx -lSDL2_mixer -lSDL2_image -lSDL2_net -lpthread -ldl -lm
endif

ifeq ($(UNAME),Darwin)
else
  export CC_FLAGS += -Wlogical-op -Wmaybe-uninitialized
endif

.PHONY: all clean
all: post-build

debug-mode: CC_FLAGS += -ggdb -DDEBUG_MODE
debug-mode: all

asanmode: 
	$(MAKE) ASANMODE=1

pre-build:
	@echo PREBUILD
	mkdir -p obj
	cp -rp graphics obj

post-build: main-build
	@echo POSTBUILD

main-build: pre-build
	@$(MAKE) --no-print-directory main-target

main-target: obj/complexis

obj/complexis: obj/main.o
	$(ENVP) $(CC) $(CC_FLAGS) obj/main.o -o obj/complexis $(EXTRA_LD)

obj/main.o: main.c
	$(ENVP) $(CC) $(CC_FLAGS) -c main.c -o obj/main.o

clean:
	rm obj/main.o obj/complexis


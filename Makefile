
#
# Libdream Examples
# (c)2000-2001 Dan Potter
#

CC := g++
CC_FLAGS := --std=c++98 -DPLATFORM_PC

.PHONY: all clean dc
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

strict: CC_FLAGS += -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused -Wmaybe-uninitialized -Wuninitialized
strict: obj/complexis

obj/complexis: obj/combined.o
	$(CC) $(CC_FLAGS) obj/combined.o -o obj/complexis -lSDL2 -lSDL2_gfx

obj/combined.o: main.cc
	$(CC) $(CC_FLAGS) -c combined.cc -o obj/combined.o

BIN = complexis
OBJS = combined.o

TARGET = $(BIN).elf

dc: dc-post-build

dc-pre-build:
	@echo PREBUILD
	cat sample7 main.cc > combined.cc

dc-post-build: dc-main-build
	@echo POSTBUILD

dc-main-build: dc-pre-build
	@$(MAKE) --no-print-directory dc-main-target

dc-main-target: $(TARGET)

include $(KOS_BASE)/Makefile.rules

clean:
	rm -f $(TARGET) $(OBJS) $(EXTRA_CLEAN)
	rm obj/main.o obj/combined.o obj/complexis


rm-elf:
	rm -f $(TARGET)

CFLAGS := -DPLATFORM_DC

$(TARGET): $(OBJS)
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) -o $(TARGET) $(KOS_START) \
		$(OBJS) $(DATAOBJS) $(OBJEXTRA) \
		$(EXTRA_LIBS) $(KOS_LIBS)

run: $(TARGET)
	$(KOS_LOADER) $(TARGET)

dist:
	rm -f $(OBJS) $(EXTRA_CLEAN)
	$(KOS_STRIP) $(TARGET)



CC := clang++
CFLAGS := -std=c++11 -I -Isrc/ -Iinclude/
LDFLAGS := -Wall -Wpedantic 
LDLIBS  := 
TARGET := text-renderer
BINEXT :=

# Detect the operating system

UNAME_S := $(shell uname -s)

DEBUG := 0

ifeq ($(DEBUG), 1)
CFLAGS += -g -O0
LDFLAGS += -g
else
LDFLAGS += -O2
endif

ifeq ($(CC), emcc)
BINEXT := .html
LDFLAGS += -all --embed-file assets@assets -s WASM=1 -s ALLOW_MEMORY_GROWTH=1
CFLAGS += -s USE_SDL=2 -s -s USE_SDL_TTF=2
LDLIBS += -s USE_SDL=2 -s -s SDL2_IMAGE_FORMATS='["png"]'
else ifeq ($(UNAME_S), Linux)
LDFLAGS += 
LDLIBS  += -lSDL2 -lSDL2_ttf -lGLESv2
endif

BUILDDIR := bin
SOURCEDIR := src
OBJDIR := obj

# Makes sure the target dir exists
MKDIR = if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi

include src/makefile

OBJS := $(addprefix $(OBJDIR)/, $(OBJS))

all: build-engine

build-engine: $(BUILDDIR)/$(TARGET)$(BINEXT)
$(BUILDDIR)/$(TARGET)$(BINEXT): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@ $(LDLIBS)

# For engine source files
$(OBJDIR)/%.o: $(SOURCEDIR)/%.c
	@$(MKDIR)
	@echo Compiling $<
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SOURCEDIR)/%.cpp
	@$(MKDIR)
	@echo Compiling $<
	@$(CC) $(CFLAGS) -c $< -o $@

clean-engine:
	rm -f $(OBJS) $(BUILDDIR)/$(TARGET)$(BINEXT) \
	$(BUILDDIR)/$(TARGET).wasm \
	$(BUILDDIR)/$(TARGET).wast \
	$(BUILDDIR)/$(TARGET).js
 

clean:
	$(MAKE) clean-engine
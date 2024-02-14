CC = g++
CFLAGS = -Wall
LIB = -lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf -lpthread
SDIR = src
HDIR = include
BUILDDIR = lib
BINARY = game
CUSTOMLIBDIR = lib


# ==========================================================================

# listing all .cpp file inside SDIR and his child folder
SOURCES := $(foreach dir, $(wildcard $(SDIR)/*),$(wildcard $(dir)/*.cpp)) $(wildcard $(SDIR)/*.cpp)

# listing all .o file from all .cpp file from SDIR
OBJECTS := $(patsubst $(SDIR)/%.cpp, $(BUILDDIR)/%.o, $(SOURCES))


# ==========================================================================

all: $(BINARY) 

$(BINARY): $(OBJECTS) $(CUSTOMLIBS)/%.o
	$(CC) -std=c++0x $(CFLAGS) $(OBJECTS) -L$(CUSTOMLIBDIR) -o $@ $(LIB)

$(BUILDDIR)/%.o: $(SDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(HDIR) -c $< -o $@

$(CUSTOMLIBS)/%.o:
	$(foreach lib, $(CUSTOMLIBS),  (cd $(lib) && make))

full-clean: clean
	$(foreach lib, $(CUSTOMLIBS),  (cd $(lib) && make clean))

clean:
	rm -rf $(foreach dir,$(wildcard $(BUILDDIR)/*), $(dir)) $(BINARY)

# ==========================================================================
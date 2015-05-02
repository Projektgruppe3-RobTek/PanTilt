CC=clang++ #Compiler
DEFINES=
CFLAGS= -c -std=c++11 -O0 -I/usr/include/wiic/ -DFPS=60#Compiler Flags
LDFLAGS= -lwiicpp -lpthread -lSDL2 -lSDL2_ttf -lSDL2_gfx -lboost_system #Linker options
SOURCES= WiiController.cpp EventHandler.cpp controller_com.cpp TextDrawer.cpp main.cpp  Drawer.cpp Primitives.cpp   #cpp files
OBJECTS=$(SOURCES:.cpp=.o)  #Object files
EXECUTEABLE=PT_Controller #Output name

all: $(SOURCES) $(EXECUTEABLE)

$(EXECUTEABLE): $(OBJECTS)
	$(CC)    $(OBJECTS) -o $(EXECUTEABLE) $(LDFLAGS)

.cpp.o:
	$(CC)  $(CFLAGS) $(DEFINES)   $< -o $@


clean:  ; rm *.o $(EXECUTEABLE)

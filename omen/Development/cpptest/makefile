CXX = g++
CPP_FILES = $(wildcard source/*.cpp)
RC_FILES = $(wildcard resource/*.rc)
OBJ_FILES = $(addprefix object/,$(notdir $(CPP_FILES:.cpp=.o) $(RC_FILES:.rc=.o)))
INCLUDES = -I "include" -I "C:/Libraries/SDL2~2.0.5/i686-w64-mingw32/include"
LIBRARIES = -L "C:/Libraries/SDL2~2.0.5/i686-w64-mingw32/lib"
DEFINES =
LFLAGS = -s -lmingw32 -lSDL2main -lSDL2
CFLAGS = -std=c++11 -Wall -Wzero-as-null-pointer-constant -pedantic -Os
OUTPUT = raycast.exe

$(OUTPUT): $(OBJ_FILES)
	$(CXX) -o $@ $^ $(LIBRARIES) $(LFLAGS)

object/%.o : resource/%.rc
	windres $^ -o $@

object/%.o: source/%.cpp 
	$(CXX) $(DEFINES) $(INCLUDES) $(CFLAGS) -c -o $@ $<

clean:
	-rm edit $(OUTPUT)
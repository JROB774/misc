CXX = g++
CPP_FILES = $(wildcard source/*.cpp)
RC_FILES = $(wildcard resource/*.rc)
OBJ_FILES = $(addprefix object/,$(notdir $(CPP_FILES:.cpp=.o) $(RC_FILES:.rc=.o)))
INCLUDES = -I "include"
LIBRARIES =
DEFINES =
LFLAGS = -s
CFLAGS = -std=c++17 -Wzero-as-null-pointer-constant -Wall -pedantic -Os
OUTPUT = texpacker.exe

$(OUTPUT): $(OBJ_FILES)
	$(CXX) -o $@ $^ $(LIBRARIES) $(LFLAGS)

object/%.o : resource/%.rc
	windres $^ -o $@

object/%.o: source/%.cpp
	$(CXX) $(DEFINES) $(INCLUDES) $(CFLAGS) -c -o $@ $<
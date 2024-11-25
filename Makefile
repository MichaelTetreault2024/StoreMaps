# Compiler and flags
CXX := g++
CXXFLAGS := -IC:\SFML-2.6.2\include
LDFLAGS := -LC:\SFML-2.6.2\lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-main

# Target names
TARGET := app.exe
OBJECTS := main.o item.o store.o gui.o

# Default rule
all: $(TARGET)

# Linking
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

# Compilation rules
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Cleanup
clean:
	del /f /q $(OBJECTS) $(TARGET)

# Phony targets
.PHONY: all clean

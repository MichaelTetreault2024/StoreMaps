# Compiler and flags
CXX := g++
CXXFLAGS := -IC:\SFML-2.6.2\include
LDFLAGS := -LC:\SFML-2.6.2\lib -lsfml-graphics -lsfml-window -lsfml-system

# Target names
TARGET := app.exe
OBJECTS := main.o item.o store.o gui.o
EXTERNAL_FILES := TextFiles/SparkyStonesRegular-BW6ld.ttf
SFML_DLLS := sfml-graphics-2.dll sfml-window-2.dll sfml-system-2.dll

# Default rule
all: $(TARGET) package

# Linking
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

# Compilation rules
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Package rule
package: $(TARGET)
	@echo Copying necessary files...
	xcopy /y TextFiles\SparkyStonesRegular-BW6ld.ttf .
	copy /y C:\SFML-2.6.2\bin\sfml-graphics-2.dll .
	copy /y C:\SFML-2.6.2\bin\sfml-window-2.dll .
	copy /y C:\SFML-2.6.2\bin\sfml-system-2.dll .

# Cleanup
clean:
	del /f /q $(OBJECTS) $(TARGET) $(SFML_DLLS)

# Phony targets
.PHONY: all clean package

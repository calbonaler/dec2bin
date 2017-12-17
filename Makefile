CXXFLAGS = -std=c++11 -Wall -Wextra
TARGET = bin/dec2bin

$(TARGET): src/dec2bin.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	$(RM) $(TARGET)

.PHONY: clean

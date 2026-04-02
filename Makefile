CXX= g++
CXXFLAGS = -O3 -Wall -Wextra -std=c++14

OBJS = main.o Map.o Search.o ColorMap.o


TARGET = busqueda

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LIBS)

all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)


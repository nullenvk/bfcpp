CFLAGS := -fstack-protector-strong -O2 -pipe
CXXFLAGS := $(CFLAGS) -std=c++17
LDFLAGS := -lm 

BIN := bf 
OBJ := src/main.o src/bf.o

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJ) 

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	-rm -r $(BIN) $(OBJ)

.PHONY: clean
.SUFFIXES: .cpp .o

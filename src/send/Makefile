PROGRAM = Program
SOURCES = send.cpp u82w1251.cpp base64.cpp test.cpp
HEADERS = send.hpp u82w1251.hpp base64.h
all: $(HEADERS) $(SOURCES)
	g++ -o$(PROGRAM) $(SOURCES) -lws2_32
static: $(HEADERS) $(SOURCES)
	
#Author: Jakub Olejnik
#olejnjak@fit.cvut.cz

#allowed commands: rm, gcc, g++, mkdir, doxygen, cp, mv, cd, ar, make
CXXFLAGS=-Wall -pedantic -Wno-long-long -O0 -ggdb
CLIBS=
CXX=g++
RM=rm -rf
OBJECTS=bin/objects/main.o bin/objects/reader.o bin/objects/commenttag.o bin/objects/tag.o bin/objects/tagnode.o bin/objects/doctypetag.o bin/objects/textnode.o bin/objects/container.o bin/objects/endtag.o bin/objects/inlinetag.o bin/objects/blocktag.o bin/objects/node.o bin/objects/document.o bin/objects/functions.o
DOXY=Doxyfile
BIN=./olejnjak
#end of macros ------------------------------------------------------------------------------------------

#generates final binary and documentation
all: $(OBJECTS) $(DOXY)
	make compile
	make doc

#build all objects and merge them into final binary
compile: $(BIN)

#run program
run: $(BIN)
	$(BIN) test

#delete all folders with binary files (including documentation)
clean:
	$(RM) bin doc $(BIN)


#generate documentation in '<login>/doc' folder
doc: $(DOXY) src/*
	( cd src | doxygen $(DOXY) 2> /dev/null > /dev/null )
#rules to make objects ----------------------------------------------------------------------------------
$(BIN): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(BIN) $(CLIBS)

#bin/objects/main.o: src/main.cpp src/ctag.h src/ctagstack.h src/cstack.h
bin/objects/main.o: src/main.cpp
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o bin/objects/main.o $(CLIBS)

bin/objects/reader.o: src/reader.cpp src/reader.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/reader.cpp -o bin/objects/reader.o $(CLIBS)

bin/objects/commenttag.o: src/commenttag.cpp src/commenttag.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/commenttag.cpp -o bin/objects/commenttag.o $(CLIBS)

bin/objects/tag.o: src/tag.cpp src/tag.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/tag.cpp -o bin/objects/tag.o $(CLIBS)

bin/objects/tagnode.o: src/tagnode.cpp src/tagnode.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/tagnode.cpp -o bin/objects/tagnode.o $(CLIBS)

bin/objects/doctypetag.o: src/doctypetag.cpp src/doctypetag.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/doctypetag.cpp -o bin/objects/doctypetag.o $(CLIBS)

bin/objects/textnode.o: src/textnode.cpp src/textnode.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/textnode.cpp -o bin/objects/textnode.o $(CLIBS)

bin/objects/container.o: src/container.cpp src/container.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/container.cpp -o bin/objects/container.o $(CLIBS)

bin/objects/endtag.o: src/endtag.cpp src/endtag.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/endtag.cpp -o bin/objects/endtag.o $(CLIBS)

bin/objects/inlinetag.o: src/inlinetag.cpp src/inlinetag.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/inlinetag.cpp -o bin/objects/inlinetag.o $(CLIBS)

bin/objects/blocktag.o: src/blocktag.cpp src/blocktag.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/blocktag.cpp -o bin/objects/blocktag.o $(CLIBS)

bin/objects/node.o: src/node.cpp src/node.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/node.cpp -o bin/objects/node.o $(CLIBS)

bin/objects/document.o: src/document.cpp src/document.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/document.cpp -o bin/objects/document.o $(CLIBS)

bin/objects/functions.o: src/functions.cpp src/functions.h
	mkdir -p bin/objects
	$(CXX) $(CXXFLAGS) -c src/functions.cpp -o bin/objects/functions.o $(CLIBS)
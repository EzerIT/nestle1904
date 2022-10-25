HEADERS=nodeid2monad.hpp findfiles.hpp objects.hpp walker.hpp
CPPFILES=find_sentences.cpp nodeid2monad.cpp findfiles.cpp objects.cpp walker.cpp
CPPFILES2=../pugixml/src/pugixml.cpp maketext.cpp
OBJFILES=$(CPPFILES:.cpp=.o) pugixml.o
DEPFILES=$(CPPFILES:.cpp=.d) pugixml.d maketext.d

CXX=c++-11.1.0
CXXFLAGS=-std=c++20 -MMD -g -I ../pugixml/src

all:	 add_sentences.mql

pugixml.o:	../pugixml/src/pugixml.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

find_sentences: $(OBJFILES)
	$(CXX) $(CXXFLAGS) $(LDLIBS) -o $@ $+ $(LDFLAGS)

maketext:	maketext.o findfiles.o pugixml.o ../oxia2tonos.o
	$(CXX) $(CXXFLAGS) $(LDLIBS) -o $@ $+ $(LDFLAGS)

maketext.o:	maketext.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

xmlWithNode.txt:	maketext
	./maketext -o $@

add_sentences.mql:	find_sentences xmlWithNode.txt
	./find_sentences -o $@

clean:
	rm -f $(OBJFILES) $(DEPFILES) add_sentences.mql find_sentences maketext xmlWithNode.txt

-include $(DEPFILES)



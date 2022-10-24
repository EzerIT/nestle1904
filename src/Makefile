HEADERS=mql_item.hpp mql_word.hpp morph.hpp util.hpp strip.hpp mql.hpp pugixml/src/pugixml.hpp
CPPFILES=mql_item.cpp mql_word.cpp nestle2mql.cpp morph.cpp util.cpp strip.cpp mql.cpp read_inflection.cpp 
CPPFILES2=pugixml/src/pugixml.cpp
OBJFILES=$(CPPFILES:.cpp=.o) pugixml.o
DEPFILES=$(CPPFILES:.cpp=.d) pugixml.d

CXX=c++-11.1.0
CXXFLAGS=-std=c++20 -MMD -g -I pugixml/src

all:	nestle1904

pugixml.o:	pugixml/src/pugixml.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


nestle2mql:	$(OBJFILES)
	$(CXX) $(CXXFLAGS) $(LDLIBS) -o $@ $+ $(LDFLAGS)

nestle.mql:	nestle2mql
	./nestle2mql -o $@ ../nestle1904-1.2/nestle1904.csv

add_sentences/add_sentences.mql:
	make -C add_sentences add_sentences.mql

nestle1904:	nestle.mql add_sentences/add_sentences.mql
	rm -f $@
	mql nestle.mql
	mql -d $@ add_sentences/add_sentences.mql

nestledump.mql:	nestle1904
	mqldump --batch-create-objects -o $@ $+

nestle.tar.bz2:	$(CPPFILES) $(HEADERS) Makefile nestle.mql nestledump.mql
	tar cfj $@ $+

nestle2csv.o:	nestle2csv.cpp
	$(CXX) -I frequency_info -c -o $@ $<

nestle2csv:	nestle2csv.o frequency_info/emdros_iterators.o -lemdros_amalgamation -lpthread -ldl
	$(CXX) $(LDLIBS) -o $@ $+ $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(DEPFILES) nestle2mql nestle.mql nestle1904 nestledump.mql nestle.tar.bz2
	make -C add_sentences clean

-include $(DEPFILES)
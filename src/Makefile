# Copyright © 2023 Claus Tøndering.
# Released under an MIT License.

HEADERS=mql_item.hpp mql_word.hpp morph.hpp util.hpp strip.hpp mql.hpp pugixml/src/pugixml.hpp oxia2tonos.hpp

CPPFILES1=mql_item.cpp mql_word.cpp nestle2mql.cpp morph.cpp util.cpp strip.cpp mql.cpp read_inflection.cpp 
CPPFILES2=oxia2tonos.cpp
CPPFILES3=hintsdb.cpp emdros_iterators.cpp

OBJFILES1=$(CPPFILES1:.cpp=.o) pugixml.o
OBJFILES2=$(CPPFILES2:.cpp=.o) o2t.o
OBJFILES3=$(CPPFILES3:.cpp=.o)

DEPFILES1=$(CPPFILES1:.cpp=.d) pugixml.d
DEPFILES2=$(CPPFILES2:.cpp=.d)
DEPFILES3=$(CPPFILES3:.cpp=.d)


CXX=c++
CXXFLAGS=-std=c++20 -MMD -O3 -I pugixml/src
CXXFLAGS2=-std=c++20 -MMD -O3

EMDROS_LIBS = $(shell pkg-config --libs emdros)


all:	nestle1904 t2o nestle1904_hints.db

pugixml.o:	pugixml/src/pugixml.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


nestle2mql:	$(OBJFILES1)
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

oxia2tonos.o:	oxia2tonos.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

o2t.o:	oxia2tonos.cpp
	$(CXX) $(CXXFLAGS) -D HAS_MAIN -c -o $@ $<

o2t:	o2t.o
	$(CXX) $(LDLIBS) -o $@ $+ $(LDFLAGS)

t2o:	o2t
	ln -s o2t t2o


hintsdb: $(OBJFILES3)
	$(CXX) $(CXXFLAGS2)  -o $@ $+ $(EMDROS_LIBS) -lpthread -ldl

hintsdb.sql: hintsdb nestle1904
	./hintsdb nestle1904 $@

nestle1904_hints.db: hintsdb.sql
	rm -f $@
	sqlite3 $@ < $+


clean:
	rm -f $(OBJFILES1) $(OBJFILES2) $(OBJFILES3) $(DEPFILES1) $(DEPFILES2) $(DEPFILES3) nestle2mql nestle.mql nestle1904 nestledump.mql nestle.tar.bz2 o2t t2o
	make -C add_sentences clean

-include $(DEPFILES1)
-include $(DEPFILES2)
-include $(DEPFILES3)

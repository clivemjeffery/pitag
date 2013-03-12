CXX=g++
INCLUDES=-I/usr/include/sigc++-2.0 -I/usr/lib/arm-linux-gnueabihf/sigc++-2.0/include
CPPFLAGS=-Wall $(INCLUDES)

objects = CTagReader.o CTagData.o
libraries = -lwiringPi -lsigc-2.0

all : tagmap tagtimer

tagmap : tagmap.o $(objects)
	$(CXX) $(CPPFLAGS) -o tagmap tagmap.o $(objects) $(libraries)
	
tagtimer : tagtimer.o $(objects)
	$(CXX) $(CPPFLAGS) -o tagtimer tagtimer.o $(objects) $(libraries)

tagmap.o : CTagReader.h CTagData.h
tagtimer.o : CTagReader.h CTagData.h
CTagReader.o : CTagReader.h
CTagData.o : CTagData.h

.PHONY : clean
clean :
	-rm -f tagmap tagmap.o tagtimer tagtimer.o $(objects)

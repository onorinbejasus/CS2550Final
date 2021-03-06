# $Id: gfx-config.in 343 2008-09-13 18:34:59Z garland $

CXX = g++
CXXFLAGS = -g -O2 -Wall -Wno-sign-compare -Iinclude -DHAVE_CONFIG_H 

OBJS = myPAGE.o myDM.o myScheduler.o myPTM.o main.o 

main: $(OBJS)
	$(CXX) -g -pthread -mmacosx-version-min=10.6 -o $@ $^ 
clean:
	rm $(OBJS) main

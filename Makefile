CXXFLAGS = -O0 -g -Wall -std=c++14

prog5: mapraview.o GreyScale.o unit.o
	$(CXX) $(CXXFLAGS) -o $@ $^

GreyScale.o: GreyScale.cpp GreyScale.h
	$(CXX) $(CXXFLAGS) -c $<

mapraview.o: mapraview.cpp
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean
clean:
	rm -f test mapraview.o GreyScale.o

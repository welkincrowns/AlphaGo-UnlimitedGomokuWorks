CXXFLAGS = -std=c++11 -O2
EXE = main
SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
DEP = $(OBJ:.o=.d)

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ) $(DEP)
	$(CXX) -o $@ $(OBJ)

-include $(DEP)

%.d: %.cpp
	@set -e; $(RM) $@; \
	 $(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM $< > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@: ,g' < $@.$$$$ > $@; \
	 $(RM) $@.$$$$

clean:
	@$(RM) $(EXE) $(OBJ) $(DEP)

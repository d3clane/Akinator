CXX = g++-13
CXXFLAGS = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations	  \
		   -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts 		  \
		   -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal      \
		   -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op \
		   -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self \
		   -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel 		  \
		   -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods 				  \
		   -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand 		  \
		   -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix   \
		   -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs 			  \
		   -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow 	  \
		   -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie  \
		   -fPIE -Werror=vla -I/Library/Frameworks/SDL2.framework/Headers -F/Library/Frameworks	  	  \
		   -framework SDL2

TARGET = akinator
DOXYFILE = Others/Doxyfile

HEADERS  = ../Common/Colors.h ../Common/Errors.h ../Common/Log.h  						\
		   ../Common/InputOutputFuncs.h ../Common/StringFuncs.h							\
		   ../Tree/Tree.h															    \
		   ../Stack/Stack.h ../Stack/ArrayFuncs.h ../Stack/HashFuncs.h ../Stack/Types.h \
		   ../FastInput/InputOutput.h ../FastInput/StringFuncs.h 						\
		   Akinator.h

FILESCPP = main.cpp ../Common/Errors.cpp ../Common/Log.cpp 		  					\
		   ../Common/InputOutputFuncs.cpp ../Common/StringFuncs.cpp 				\
		   ../Tree/Tree.cpp  													    \
		   ../Stack/Stack.cpp ../Stack/ArrayFuncs.cpp ../Stack/HashFuncs.cpp 		\
		   ../FastInput/InputOutput.cpp ../FastInput/StringFuncs.cpp				\
		   Akinator.cpp

objects = $(FILESCPP:%.cpp=%.o)

.PHONY: all docs clean buildDirs

all: $(TARGET)

$(TARGET): $(objects) 
	$(CXX) $^ -o $(TARGET) $(CXXFLAGS)

%.o : %.cpp $(HEADERS)
	$(CXX) -c $< -o $@ $(CXXFLAGS) 

docs: 
	doxygen $(DOXYFILE)

clean:
	rm -rf $(OBJECTDIR)/*.o

buildDirs:
	mkdir $(OBJECTDIR)
	mkdir $(PROGRAMDIR)
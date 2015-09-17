ifeq ($(shell uname), Darwin)
  PLATFORM=mac
endif

ifeq ($(shell uname), Linux)
  PLATFORM=linux
endif
ifeq ($(PLATFORM),mac)
#Mac frameworks
LDFLAGS = -framework GLUT -framework OpenGL -framework Cocoa
MICRO = ../common/Mac/MicroGlut.o

endif

ifeq ($(PLATFORM),linux)
LDFLAGS = -lGL -lm -DGL_GLEXT_PROTOTYPES -lXt -lX11
MICRO = ../common/Linux/MicroGlut.c
endif


#Compiler flags, enable warnings and c99 standard
CLANGFLAGS = -Wall -std=c99 -I../common -I../common/Linux
#Binaries that are needed for all sub-compilations
BINARIES = ../common/GL_utilities.c ../common/VectorUtils3.c ../common/LoadTGA.c ../common/loadobj.c $(MICRO)


all: skinning skinning2
#target: dependencies
#’tab’ 	compilation commands


skinning: skinning.c $(BINARIES)
	clang $(CLANGFLAGS) $(BINARIES) skinning.c -o skinning.out $(LDFLAGS)

skinning2: skinning2.c $(BINARIES)
	clang $(CLANGFLAGS) $(BINARIES) skinning2.c -o skinning2.out $(LDFLAGS)


#compile MicroGlut on it’s own and ignore warnings.
MicroGlut.o: $(MICRO)
	clang -w -c $(MICRO)

clean:
	rm skinning2
	rm skinning

system:
	printf $(shell uname)

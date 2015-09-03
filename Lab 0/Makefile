all :  lab0

lab0: lab0.c ../common/GL_utilities.c ../common/VectorUtils3.c ../common/LoadTGA.c ../common/loadobj.c ../common/Linux/MicroGlut.c
	clang -std=c99 -Wall -o lab0 -lGL -lm -DGL_GLEXT_PROTOTYPES -lXt -lX11 -DGL_GLEXT_PROTOTYPES lab0.c ../common/GL_utilities.c ../common/VectorUtils3.c ../common/LoadTGA.c ../common/loadobj.c ../common/Linux/MicroGlut.c -I../common -I../common/Linux

clean :
	rm lab0

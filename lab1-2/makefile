all :  lab1-2

lab1-2: lab1-2.c ../common/GL_utilities.c ../common/VectorUtils3.c ../common/LoadTGA.c ../common/loadobj.c ../common/zpr.c ../common/Linux/MicroGlut.c
	clang -Wall -o lab1-2 -lm -lGL -lXt -lX11 -DGL_GLEXT_PROTOTYPES lab1-2.c ../common/GL_utilities.c ../common/VectorUtils3.c ../common/LoadTGA.c ../common/loadobj.c ../common/zpr.c ../common/Linux/MicroGlut.c -I../common -I../common/Linux

clean :
	rm lab1-2

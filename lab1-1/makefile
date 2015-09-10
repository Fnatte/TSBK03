all :  lab1-1

lab1-1: lab1-1.c ../common/GL_utilities.c ../common/VectorUtils3.c ../common/LoadTGA.c ../common/loadobj.c ../common/zpr.c ../common/Linux/MicroGlut.c
	clang -Wall -o lab1-1 -lm -lGL -DGL_GLEXT_PROTOTYPES -lXt -lX11 lab1-1.c ../common/GL_utilities.c ../common/VectorUtils3.c ../common/LoadTGA.c ../common/loadobj.c ../common/zpr.c ../common/Linux/MicroGlut.c -I../common -I../common/Linux

clean :
	rm lab1-1

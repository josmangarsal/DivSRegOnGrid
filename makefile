DEBUG  =  -g
FLAGS  = -Wall -march=x86-64
#FLAGS  = -Wall -pg
CC     = gcc

default : DivSRegOnGrid

#-------------------------------------------------------------------------------
DivSRegOnGrid-1.0 : makefile DivSRegOnGrid.c argshand.o \
	getmem.o utils.o Queue.o gengridpoints.o btvertex.o CDSimplex.o listCDSimplex.o divide.o \
	btCDSimplex.o
	$(CC) $(DEBUG) $(FLAGS) -o DivSRegOnGrid \
	DivSRegOnGrid.c argshand.o getmem.o utils.o Queue.o gengridpoints.o btvertex.o \
	CDSimplex.o listCDSimplex.o divide.o btCDSimplex.o -L/usr/lib64 -lm

btCDSimplex.o : btCDSimplex.c btCDSimplex.h listCDSimplex.o CDSimplex.o
	$(CC) $(DEBUG) $(FLAGS) -c -o btCDSimplex.o btCDSimplex.c

divide.o : divide.c divide.h
	$(CC) $(DEBUG) $(FLAGS) -c -o divide.o divide.c

listCDSimplex.o : listCDSimplex.c listCDSimplex.h CDSimplex.o
	$(CC) $(DEBUG) $(FLAGS) -c -o listCDSimplex.o listCDSimplex.c

CDSimplex.o : CDSimplex.c CDSimplex.h
	$(CC) $(DEBUG) $(FLAGS) -c -o CDSimplex.o CDSimplex.c

btvertex.o : btvertex.c btvertex.h
	$(CC) $(DEBUG) $(FLAGS) -c -o btvertex.o btvertex.c

utils.o : utils.c utils.h
	$(CC) $(DEBUG) $(FLAGS) -c -o utils.o utils.c

gengridpoints.o : gengridpoints.c gengridpoints.h
	$(CC) $(DEBUG) $(FLAGS) -c -o gengridpoints.o gengridpoints.c

Queue.o : Queue.c Queue.h
	$(CC) $(DEBUG) $(FLAGS) -c -o Queue.o Queue.c

getmem.o : getmem.c getmem.h
	$(CC) $(DEBUG) $(FLAGS) -c -o getmem.o getmem.c

argshand.o : argshand.c argshand.h
	$(CC) $(DEBUG) $(FLAGS) -c -o argshand.o argshand.c

#-------------------------------------------------------------------------------
clean:
	rm -f *.o *~ *.bck *.bak DivSRegOnGrid

#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------

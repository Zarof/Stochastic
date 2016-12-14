SYSTEM     = x86-64_linux
LIBFORMAT  = static_pic

#------------------------------------------------------------
#
# When you adapt this makefile to compile your CPLEX programs
# please copy this makefile and set CPLEXDIR and CONCERTDIR to
# the directories where CPLEX and CONCERT are installed.
#
#------------------------------------------------------------

#CPLEXDIR = /opt/ibm/ILOG/CPLEX_Studio1263/cplex/
CPLEXDIR      = /opt/local/stow/cplex126/CPLEX_Studio/cplex/
#CPLEXDIR = /opt/local/CPLEX_Studio/cplex
#/cm/shared/apps/cplex/12.6/cplex
#CONCERTDIR = /opt/ibm/ILOG/CPLEX_Studio1263/concert/
CONCERTDIR    = /opt/local/stow/cplex126/CPLEX_Studio/concert/
#CONCERTDIR    =/opt/local/CPLEX_Studio/concert
#/cm/shared/apps/cplex/12.6/concert

# ---------------------------------------------------------------------
# Compiler selection 
# ---------------------------------------------------------------------

CCC = g++

# ---------------------------------------------------------------------
# Compiler options 
# ---------------------------------------------------------------------

CCOPT = -m64 -O -fPIC -fexceptions -DNDEBUG -DIL_STD
# CCOPT = -m64 -O -fPIC -fexceptions -g -DIL_STD

# ---------------------------------------------------------------------
# Link options and libraries
# ---------------------------------------------------------------------

#CPLEXBINDIR   = $(CPLEXDIR)/bin/$(BINDIST)
CPLEXBINDIR   = $(CPLEXDIR)bin/$(SYSTEM)
CPLEXLIBDIR   = $(CPLEXDIR)lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)lib/$(SYSTEM)/$(LIBFORMAT)

CCLNFLAGS = -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -lm -lpthread

CONCERTINCDIR = $(CONCERTDIR)include
CPLEXINCDIR   = $(CPLEXDIR)include

CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR)

all:	optiStoch

optiStoch: main.o instance.o
	$(CCC) $(CCFLAGS) -g main.o instance.o -g -o optiStoch $(CCLNFLAGS)

main.o: main.cpp instance.hpp
	$(CCC) -g -c $(CCFLAGS) main.cpp -o main.o
instance.o: instance.cpp instance.hpp
	g++ -g -c $(CCFLAGS) instance.cpp

clean:
	rm -f *.o

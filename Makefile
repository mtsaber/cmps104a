
# $Id: Makefile,v 1.19 2016-08-18 15:05:42-07 - - $

DEPSFILE  = Makefile.deps
CPP       = g++ -g -O0 -Wall -Wextra -std=gnu++14
MKDEPS    = g++ -MM -std=gnu++14

MODULES   = string_set auxlib cppstrtok
HDRSRC    = ${MODULES:=.h}
CPPSRC    = ${MODULES:=.cpp} main.cpp
EXECBIN   = oc
ALLCSRC   = ${CPPSRC}
OBJECTS   = ${ALLCSRC:.cpp=.o}
MODSRC    = ${foreach MOD, ${MODULES}, ${MOD}.h ${MOD}.cpp}
MISCSRC   = ${filter-out ${MODSRC}, ${HDRSRC} ${CPPSRC}}
ALLSRC    = README ${MODSRC} ${MISCSRC} Makefile

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${CPP} -o${EXECBIN} ${OBJECTS}

%.o : %.cpp
	${CPP} -c $<

ci : 
	git add -A *
	git commit -a

clean :
	- rm ${OBJECTS} ${DEPSFILE}

spotless : clean
	- rm ${EXECBIN} List.*.ps List.*.pdf *.str *.oc *.err *.log *.oh *.out

deps : ${ALLCSRC}
	@ echo "# ${DEPSFILE} created `date` by ${MAKE}" >${DEPSFILE}
	${MKDEPS} ${ALLCSRC} >>${DEPSFILE}

${DEPSFILE} :
	@ touch ${DEPSFILE}
	${MAKE} --no-print-directory deps

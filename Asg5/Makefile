# Connor Thurmond (cthurmon) Rami Alrwais (raalrwai)

GPP  = g++ -std=gnu++17 -g -O0 -Wall -Wextra -Wold-style-cast
LIBS = -lglut -lGLU -lGL -lX11 -lrt -lm -ldrm

SOURCES  = rainbow.cpp circles.cpp chessboard.cpp
BINARIES = ${SOURCES:.cpp=}

all : ${BINARIES}

% : %.cpp
	${GPP} $< -o $@ ${LIBS}	

ci : ${SOURCES} Makefile
	cpplint.py.perl ${SOURCES}
	checksource ${SOURCES} Makefile
	cid + ${SOURCES} Makefile

clean :
	- rm ${BINARIES}	

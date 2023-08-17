CXX = g++
CXXFLAGS = -g -std=c++14 -Wall -MMD
EXEC = chess
OBJECTS = pawn.o rook.o bishop.o knight.o king.o queen.o tile.o board.o piece.o window.o computer.o scoreboard.o main.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lX11

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}


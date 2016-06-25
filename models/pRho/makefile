atest: ArenaTest
ptest: PlayerpTest

clean:
	rm *.o PlayerpTest

PlayerpTest: PlayerpTest.cpp Playerp.o Chessboard.o ForbiddenPointFinder.o Oracle.o
	g++ --std=c++11 PlayerpTest.cpp Playerp.o Chessboard.o ForbiddenPointFinder.o Oracle.o -o PlayerpTest `python2.7-config --cflags --ldflags`

ArenaTest: ArenaTest.cpp Arena.o Playerp.o Chessboard.o ForbiddenPointFinder.o Oracle.o
	g++ --std=c++11 ArenaTest.cpp Arena.o Playerp.o Chessboard.o ForbiddenPointFinder.o Oracle.o -o ArenaTest `python2.7-config --cflags --ldflags`

Arena.o: Arena.cpp Arena.h Chessboard.h ForbiddenPointFinder.h Playerp.h Move.h Oracle.h
	g++ --std=c++11 -c Arena.cpp -o Arena.o `python2.7-config --cflags --ldflags`

Playerp.o: Playerp.cpp Playerp.h Oracle.h ForbiddenPointFinder.h Chessboard.h Move.h
	g++ --std=c++11 -c Playerp.cpp -o Playerp.o `python2.7-config --cflags --ldflags`

Oracle.o: Oracle.cpp Oracle.h Move.h
	g++ -c Oracle.cpp -o Oracle.o `python2.7-config --cflags --ldflags`
	
Chessboard.o: Chessboard.cpp Chessboard.h ForbiddenPointFinder.h
	g++ -c Chessboard.cpp -o Chessboard.o

ForbiddenPointFinder.o: ForbiddenPointFinder.cpp ForbiddenPointFinder.h
	g++ -c ForbiddenPointFinder.cpp -o ForbiddenPointFinder.o
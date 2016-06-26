v: vTheta
atest: ArenaTest
ptest: PlayerpTest

clean:
	rm *.o vTheta

CC = g++ -O3

PlayerpTest: PlayerpTest.cpp Playerp.o Chessboard.o ForbiddenPointFinder.o Oracle.o
	$(CC)  --std=c++11 PlayerpTest.cpp Playerp.o Chessboard.o ForbiddenPointFinder.o Oracle.o -o PlayerpTest `python2.7-config --cflags --ldflags`

ArenaTest: ArenaTest.cpp Arena.o Playerp.o Chessboard.o ForbiddenPointFinder.o Oracle.o
	$(CC)  --std=c++11 ArenaTest.cpp Arena.o Playerp.o Chessboard.o ForbiddenPointFinder.o Oracle.o -o ArenaTest `python2.7-config --cflags --ldflags`

vTheta: vTheta.cpp Playerp.o Chessboard.o ForbiddenPointFinder.o Oracle.o
	$(CC) --std=c++11 vTheta.cpp Playerp.o Chessboard.o ForbiddenPointFinder.o Oracle.o -o vTheta `python2.7-config --cflags --ldflags`

Arena.o: Arena.cpp Arena.h Chessboard.h ForbiddenPointFinder.h Playerp.h Move.h Oracle.h
	$(CC)  --std=c++11 -c Arena.cpp -o Arena.o `python2.7-config --cflags --ldflags`

Playerp.o: Playerp.cpp Playerp.h Oracle.h ForbiddenPointFinder.h Chessboard.h Move.h
	$(CC)  --std=c++11 -c Playerp.cpp -o Playerp.o `python2.7-config --cflags --ldflags`

Oracle.o: Oracle.cpp Oracle.h Move.h
	$(CC)  -c Oracle.cpp -o Oracle.o `python2.7-config --cflags --ldflags`
	
Chessboard.o: Chessboard.cpp Chessboard.h ForbiddenPointFinder.h
	$(CC)  -c Chessboard.cpp -o Chessboard.o

ForbiddenPointFinder.o: ForbiddenPointFinder.cpp ForbiddenPointFinder.h
	$(CC)  -c ForbiddenPointFinder.cpp -o ForbiddenPointFinder.o

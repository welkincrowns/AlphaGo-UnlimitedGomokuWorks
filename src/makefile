all: AccuracyCalculator PlayerpTest ArenaTest QinAI PlayGTP

# CC = clang++ -g -fsanitize=address -fno-omit-frame-pointer
CC = g++ -O2 --std=c++11

.PHONY: acc test clean

acc:
	./AccuracyCalculator

test:
	./PlayerpTest

clean:
	rm -rf *.o *.pyc *.al *.ar *.dSYM PlayerpTest AccuracyCalculator ArenaTest PlayGTP

QinAI: QinAI.cpp
	$(CC) QinAI.cpp -o QinAI

PlayGTP: PlayGTP.cpp Playerp.o Chessboard.o ForbiddenPointFinder.o Oracle.o
	$(CC) PlayGTP.cpp Playerp.o Chessboard.o ForbiddenPointFinder.o Oracle.o -o PlayGTP -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -fno-strict-aliasing -fno-common -dynamic -g  -DNDEBUG -g -fwrapv  -Wall -Wstrict-prototypes -L/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config -lpython2.7 -ldl -framework CoreFoundation

ArenaTest: ArenaTest.cpp Arena.o Playerp.o Chessboard.o ForbiddenPointFinder.o Oracle.o
	$(CC) ArenaTest.cpp Arena.o Playerp.o Chessboard.o ForbiddenPointFinder.o Oracle.o -o ArenaTest `python2.7-config --cflags --ldflags`

Arena.o: Arena.cpp Arena.h Chessboard.h ForbiddenPointFinder.h Playerp.h Move.h Oracle.h
	$(CC) -c Arena.cpp -o Arena.o `python2.7-config --cflags --ldflags`

AccuracyCalculator: AccuracyCalculator.cpp Playerp.o Chessboard.o ForbiddenPointFinder.o Oracle.o
	$(CC) AccuracyCalculator.cpp Playerp.o Chessboard.o ForbiddenPointFinder.o Oracle.o -o AccuracyCalculator -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -fno-strict-aliasing -fno-common -dynamic -g  -DNDEBUG -g -fwrapv  -Wall -Wstrict-prototypes -L/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config -lpython2.7 -ldl -framework CoreFoundation

PlayerpTest: PlayerpTest.cpp Playerp.o Chessboard.o ForbiddenPointFinder.o Oracle.o
	$(CC) PlayerpTest.cpp Playerp.o Chessboard.o ForbiddenPointFinder.o Oracle.o -o PlayerpTest -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -fno-strict-aliasing -fno-common -dynamic -g  -DNDEBUG -g -fwrapv  -Wall -Wstrict-prototypes -L/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config -lpython2.7 -ldl -framework CoreFoundation

Playerp.o: Playerp.cpp Playerp.h Oracle.h ForbiddenPointFinder.h Chessboard.h Move.h AI.h Tree.h
	$(CC) -c Playerp.cpp -o Playerp.o -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -fno-strict-aliasing -fno-common -dynamic -g  -DNDEBUG -g -fwrapv  -Wall -Wstrict-prototypes -L/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config -lpython2.7 -ldl -framework CoreFoundation

Oracle.o: Oracle.cpp Oracle.h Move.h
	$(CC) -c Oracle.cpp -o Oracle.o -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -fno-strict-aliasing -fno-common -dynamic -g  -DNDEBUG -g -fwrapv  -Wall -Wstrict-prototypes -L/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config -lpython2.7 -ldl -framework CoreFoundation

Chessboard.o: Chessboard.cpp Chessboard.h ForbiddenPointFinder.h
	$(CC) -c Chessboard.cpp -o Chessboard.o

ForbiddenPointFinder.o: ForbiddenPointFinder.cpp ForbiddenPointFinder.h
	$(CC) -c ForbiddenPointFinder.cpp -o ForbiddenPointFinder.o
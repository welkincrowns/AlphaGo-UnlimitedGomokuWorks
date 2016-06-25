all: AccuracyCalculator PlayerpTest

.PHONY: acc test clean

acc:
	./AccuracyCalculator

test:
	./PlayerpTest

clean:
	rm *.o PlayerpTest

AccuracyCalculator: AccuracyCalculator.cpp Playerp.o Chessboard.o ForbiddenPointFinder.o Oracle.o
	g++ AccuracyCalculator.cpp Playerp.o Chessboard.o ForbiddenPointFinder.o Oracle.o -o AccuracyCalculator -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -fno-strict-aliasing -fno-common -dynamic -g -O2 -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes -L/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config -lpython2.7 -ldl -framework CoreFoundation

PlayerpTest: PlayerpTest.cpp Playerp.o Chessboard.o ForbiddenPointFinder.o Oracle.o
	g++ PlayerpTest.cpp Playerp.o Chessboard.o ForbiddenPointFinder.o Oracle.o -o PlayerpTest -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -fno-strict-aliasing -fno-common -dynamic -g -O2 -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes -L/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config -lpython2.7 -ldl -framework CoreFoundation

Playerp.o: Playerp.cpp Playerp.h Oracle.h ForbiddenPointFinder.h Chessboard.h Move.h AI.h
	g++ -c Playerp.cpp -o Playerp.o -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -fno-strict-aliasing -fno-common -dynamic -g -O2 -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes -L/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config -lpython2.7 -ldl -framework CoreFoundation

Oracle.o: Oracle.cpp Oracle.h Move.h
	g++ -c Oracle.cpp -o Oracle.o -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -fno-strict-aliasing -fno-common -dynamic -g -O2 -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes -L/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config -lpython2.7 -ldl -framework CoreFoundation

Chessboard.o: Chessboard.cpp Chessboard.h ForbiddenPointFinder.h
	g++ -c Chessboard.cpp -o Chessboard.o

ForbiddenPointFinder.o: ForbiddenPointFinder.cpp ForbiddenPointFinder.h
	g++ -c ForbiddenPointFinder.cpp -o ForbiddenPointFinder.o
atest: ArenaTest

ptest: PlayerpTest

clean:
	rm *.o PlayerpTest

PlayerpTest: PlayerpTest.cpp Playerp.o Chessboard.o ForbiddenPointFinder.o Oracle.o
	g++ --std=c++11 PlayerpTest.cpp Playerp.o Chessboard.o ForbiddenPointFinder.o Oracle.o -o PlayerpTest -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -fno-strict-aliasing -fno-common -dynamic -g -O2 -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes -L/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config -lpython2.7 -ldl -framework CoreFoundation

ArenaTest: ArenaTest.cpp Arena.o Playerp.o Chessboard.o ForbiddenPointFinder.o Oracle.o
	g++ --std=c++11 ArenaTest.cpp -o ArenaTest -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -fno-strict-aliasing -fno-common -dynamic -g -O2 -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes -L/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config -lpython2.7 -ldl -framework CoreFoundation

Arena.o: Arena.cpp Arena.h Chessboard.h ForbiddenPointFinder.h Playerp.h Move.h Oracle.h
	g++ --std=c++11 -c Arena.cpp -o Arena.o -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -fno-strict-aliasing -fno-common -dynamic -g -O2 -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes -L/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config -lpython2.7 -ldl -framework CoreFoundation

Playerp.o: Playerp.cpp Playerp.h Oracle.h ForbiddenPointFinder.h Chessboard.h Move.h
	g++ --std=c++11 -c Playerp.cpp -o Playerp.o -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -fno-strict-aliasing -fno-common -dynamic -g -O2 -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes -L/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config -lpython2.7 -ldl -framework CoreFoundation

Oracle.o: Oracle.cpp Oracle.h Move.h
	g++ -c Oracle.cpp -o Oracle.o -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -I/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/include/python2.7 -fno-strict-aliasing -fno-common -dynamic -g -O2 -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes -L/usr/local/Cellar/python/2.7.11/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config -lpython2.7 -ldl -framework CoreFoundation

Chessboard.o: Chessboard.cpp Chessboard.h ForbiddenPointFinder.h
	g++ -c Chessboard.cpp -o Chessboard.o

ForbiddenPointFinder.o: ForbiddenPointFinder.cpp ForbiddenPointFinder.h
	g++ -c ForbiddenPointFinder.cpp -o ForbiddenPointFinder.o
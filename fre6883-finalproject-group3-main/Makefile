CC = g++
CFLAGS = -Wall -ggdb3 -std=c++11 -I"$HOME/vcpkg/installed/x64-linux/include"
# Compilation flags
LFLAGS = -L"$HOME/vcpkg/installed/x64-linux/lib"
# Linking flags
LDLIBS = -lcurl -lpthread

main: main.o Ticker.o HistoricalPriceFetcher.o Matrix.o ModelCalculation.o Menu.o Bootstrap.o
	$(CC) main.o Ticker.o HistoricalPriceFetcher.o Matrix.o ModelCalculation.o Menu.o Bootstrap.o $(LFLAGS) $(LDLIBS) -o main

main.o: main.cpp Ticker.h RandomDraw.h ModelCalculation.h HistoricalPriceFetcher.h Matrix.h Menu.h
	$(CC) $(CFLAGS) -c main.cpp

Menu.o: Menu.h Menu.cpp Ticker.h HistoricalPriceFetcher.h Matrix.h Bootstrap.h plotGnuplot.h
	$(CC) $(CFLAGS) -c Menu.cpp

Ticker.o: Ticker.h Ticker.cpp Matrix.h
	$(CC) $(CFLAGS) -c Ticker.cpp

ModelCalculation.o: ModelCalculation.h ModelCalculation.cpp Matrix.h Ticker.h
	$(CC) $(CFLAGS) -c ModelCalculation.cpp

HistoricalPriceFetcher.o: HistoricalPriceFetcher.h HistoricalPriceFetcher.cpp Matrix.h Ticker.h
	$(CC) $(CFLAGS) -c HistoricalPriceFetcher.cpp

Matrix.o: Matrix.h Matrix.cpp
	$(CC) $(CFLAGS) -c Matrix.cpp

Bootstrap.o: Bootstrap.h Bootstrap.cpp RandomDraw.h ModelCalculation.h Ticker.h Matrix.h
	$(CC) $(CFLAGS) -c Bootstrap.cpp

clean:
	rm -rf  main  *.o

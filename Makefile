CC = g++ -O3 -lrt -fopenmp -DNDEBUG -DSEQAN_ENABLE_DEBUG=0 -DSEQAN_ENABLE_TESTING=0 -I/home/dbryant/development/seqan-trunk/core/include -I/home/dbryant/development/seqan-trunk/extras/include

all: element

clean:
	rm -f *.o *~ core* element

element: Sequences.o Words.o Parameters.o ElementParameters.o FilterParameters.o WordStats.o WordStatsObserved.o Partitioner.o Partitions.o Element.o Elementer.o Count.o Counter.o Filter.o Filterer.o main.o
	$(CC) Sequences.o Words.o Parameters.o ElementParameters.o FilterParameters.o WordStats.o WordStatsObserved.o Partitioner.o Partitions.o Element.o Elementer.o Count.o Counter.o Filter.o Filterer.o main.o -o element

Sequences.o: Sequences.cpp Sequences.h Parameters.h
	$(CC) -c Sequences.cpp

Words.o: Words.cpp Words.h Parameters.h stdafx.h
	$(CC) -c Words.cpp

Parameters.o: Parameters.cpp Parameters.h
	$(CC) -c Parameters.cpp

ElementParameters.o: ElementParameters.cpp Parameters.h
	$(CC) -c ElementParameters.cpp

FilterParameters.o: FilterParameters.cpp Parameters.h
	$(CC) -c FilterParameters.cpp

WordStats.o: WordStats.cpp WordStats.h
	$(CC) -c WordStats.cpp

WordStatsObserved.o: WordStatsObserved.cpp WordStats.h
	$(CC) -c WordStatsObserved.cpp

Partitioner.o: Partitioner.cpp Partitioner.h
	$(CC) -c Partitioner.cpp

Partitions.o: Partitions.cpp Partitions.h Partitioner.h
	$(CC) -c Partitions.cpp

Element.o: Element.cpp Element.h Parameters.h Words.h Sequences.h Counter.h Elementer.h
	$(CC) -c Element.cpp

Elementer.o: Elementer.cpp Elementer.h Words.h WordStats.h Sequences.h Counter.h Partitions.h
	$(CC) -c Elementer.cpp

Count.o: Count.cpp Count.h Parameters.h Words.h Sequences.h Counter.h
	$(CC) -c Count.cpp

Counter.o: Counter.cpp Counter.h Words.h WordStats.h Sequences.h
	$(CC) -c Counter.cpp

Filterer.o: Filterer.cpp Filterer.h Parameters.h Elementer.h
	$(CC) -c Filterer.cpp

Filter.o: Filter.cpp Filter.h Parameters.h Filterer.h
	$(CC) -c Filter.cpp

main.o: main.cpp main.h Count.h Element.h Filter.h
	$(CC) -c main.cpp


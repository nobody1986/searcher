CC=gcc

	
searcher: searcher.c searcher.h
	$(CC) -I. searcher.c -lz -o searcher 

.PHONY :clean
clean:
	rm -f *.o compiled.cd.gz searcher

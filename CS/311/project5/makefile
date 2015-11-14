CC=icc
CFLAGS=-std=c99 -Wall -pthread -lrt
LC=pdflatex
LFLAGS=-f

INCLUDES=
SOURCE=bitmask.c
TARGET=-o primes

TEX=primes.tex
PS=timing.ps
P2P=epstopdf
EXTRA=primes.log primes.aux primes.toc timing.pdf

default: primes tex

all: primes tex

primes: bitmask.c
	$(CC) $(CFLAGS) $(SOURCE) $(INCLUDES) $(TARGET)

tex: primes.tex
#	run pdflatex once to generate info for the contents
	$(P2P) $(PS)
	$(LC) $(TEX)
#	run pdflatex again for the actual pdf
	$(LC) $(TEX)
	rm -rf $(EXTRA)

clean:
	rm -rf *.o primes primes.pdf
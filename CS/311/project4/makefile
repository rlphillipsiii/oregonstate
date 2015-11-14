CC=icc
CFLAGS=-std=c99 -Wall
LC=pdflatex
LFLAGS=-f

INCLUDES=parser.c parser.h merger.c merger.h
SOURCE=main.c
TARGET=-o uniq

TEX=uniquify.tex
PS=timing.ps
P2P=epstopdf
EXTRA=uniquify.log uniquify.aux uniquify.toc timing.pdf

default: uniq tex

all: uniq tex

uniq: main.c
	$(CC) $(CFLAGS) $(SOURCE) $(INCLUDES) $(TARGET)

tex: uniquify.tex
#	run pdflatex once to generate info for the contents
	$(P2P) $(PS)
	$(LC) $(TEX)
#	run pdflatex again for the actual pdf
	$(LC) $(TEX)
	rm -rf $(EXTRA)

clean:
	rm -rf *.o uniq uniquify.pdf timing.pdf
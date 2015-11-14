CC=icc
CFLAGS=-std=c99 -Wall -pthread
LC=pdflatex
LFLAGS=-f

INCLUDES=
SOURCE=compute.c
SRC_R=report.c
TARGET=-o compute
TAR_R=-o report

TEX=sockets.tex
EXTRA=sockets.log sockets.aux sockets.toc timing.pdf

default: compute tex report

all: compute tex report

compute: compute.c
	$(CC) $(CFLAGS) $(SOURCE) $(INCLUDES) $(TARGET)

report: report.c
	$(CC) $(CFLAGS) $(SRC_R) $(INCLUDES) $(TAR_R)

tex: sockets.tex
#	run pdflatex once to generate info for the contents
	$(LC) $(TEX)
#	run pdflatex again for the actual pdf
	$(LC) $(TEX)
	rm -rf $(EXTRA)

clean:
	rm -rf *.o compute sockets.pdf report
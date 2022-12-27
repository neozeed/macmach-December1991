#TROFF= ptroff
TROFF= psroff
#TROFF= ditroff -Tps

all: protocol encoding

protocol: protocol.ms
	eqn protocol.ms | ${TROFF} -ms 2> prototoc.out > protocol.PS
	make TROFF=${TROFF} prototoc

prototoc: prototoc.out
	cat toc.hdr > contents
	echo ".Bg 1" >> contents
	echo "PEX Protocol Specification" >> contents
	echo ".Ed 1" >> contents
	cat prototoc.out >> contents
	${TROFF} -ms contents > proto_toc.PS

encoding: encoding.ms
	tbl encoding.ms | ${TROFF} -ms 2> enctoc.out > encoding.PS
	make TROFF=${TROFF} enctoc

enctoc:	enctoc.out
	cat toc.hdr > contents
	echo ".Bg 1" >> contents
	echo "PEX Protocol Encoding" >> contents
	echo ".Ed 1" >> contents
	cat toc.out >> contents
	${TROFF} -ms contents > enc_toc.PS

clean:
	rm -f *~ \#* *.bak errs prototoc.out enctoc.out contents

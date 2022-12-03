include config.mk

clean:
	@echo cleaning older executable...
	@rm ${PREFIX}/bin/${OUT}

build:
	@echo building...
	${CC} ${SRC} -o ${OUT} ${CFLAGS}

install: build
	@echo installing...
	@mkdir -p ${PREFIX}/bin
	@mv ${OUT} ${PREFIX}/bin

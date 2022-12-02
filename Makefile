include config.mk

clean:
	@echo cleaning older executable...
	@rm ${PREFIX}/bin/${OUT}

build:
	@echo building...
	@cc ${SRC} -o ${OUT} ${CFLAGS}

install: build
	@echo installing...
	@mkdir -p ${PREFIX}/bin
	@mv ${OUT} ${PREFIX}/bin

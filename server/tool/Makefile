CC?=            gcc
DEST?=          ./bin
SRC?=           ./src
SCRIPT?=        ./script
INSTALL?=       /usr/bin/install
MKDIR?=         /bin/mkdir -p
RMDIR?=         /bin/rmdir
RM?=            /bin/rm -f
BINDIR?=        /usr/sbin
DAEMONDIR?=     /etc/init.d

INSTALL_PROGRAM=${INSTALL} -m ${BINMODE}
BINMODE=        755

all: ${DEST}/doorknob-run-impl ${DEST}/doorknob-impl

${DEST}/doorknob-common.o: ${SRC}/doorknob-common.h ${SRC}/doorknob-common.c
	${CC} ./src/doorknob-common.c -c -o ./bin/doorknob-common.o

${DEST}/doorknob-run-impl: ${DEST}/doorknob-common.o ${SRC}/doorknob-run-impl.c
	${CC} ${DEST}/doorknob-common.o ${SRC}/doorknob-run-impl.c -o ${DEST}/doorknob-run-impl

${DEST}/doorknob-impl: ${DEST}/doorknob-common.o ${SRC}/doorknob-impl.c
	${CC} ${DEST}/doorknob-common.o ${SRC}/doorknob-impl.c -o ${DEST}/doorknob-impl

clean:
	${RM} ${DEST}/doorknob-common.o
	${RM} ${DEST}/doorknob-run-impl
	${RM} ${DEST}/doorknob-impl

install:
	sudo ${INSTALL_PROGRAM} ${DEST}/doorknob-impl ${BINDIR}/doorknob-impl
	sudo ${INSTALL_PROGRAM} ${DEST}/doorknob-run-impl    ${BINDIR}/doorknob-run-impl
	sudo ${INSTALL_PROGRAM} ${SCRIPT}/doorknob   ${BINDIR}/doorknob
	sudo ${INSTALL_PROGRAM} ${SCRIPT}/doorknob-run      ${BINDIR}/doorknob-run
	sudo ${INSTALL_PROGRAM} ${SCRIPT}/doorknobd           ${DAEMONDIR}/doorknobd
	sudo update-rc.d doorknobd defaults

uninstall:
	sudo rm ${BINDIR}/doorknob-impl
	sudo rm ${BINDIR}/doorknob-run-impl
	sudo rm ${BINDIR}/doorknob
	sudo rm ${BINDIR}/doorknob-run
	sudo rm ${DAEMONDIR}/doorknobd
	sudo update-rc.d -f doorknobd remove

deinstall: uninstall

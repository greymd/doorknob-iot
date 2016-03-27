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

all: ${DEST}/door-service-impl ${DEST}/door-controller-impl

${DEST}/door-common.o: ${SRC}/door-common.h ${SRC}/door-common.c
	${CC} ./src/door-common.c -c -o ./bin/door-common.o

${DEST}/door-service-impl: ${DEST}/door-common.o ${SRC}/door-service-impl.c
	${CC} ${DEST}/door-common.o ${SRC}/door-service-impl.c -o ${DEST}/door-service-impl

${DEST}/door-controller-impl: ${DEST}/door-common.o ${SRC}/door-controller-impl.c
	${CC} ${DEST}/door-common.o ${SRC}/door-controller-impl.c -o ${DEST}/door-controller-impl

clean:
	${RM} ${DEST}/door-common.o
	${RM} ${DEST}/door-service-impl
	${RM} ${DEST}/door-controller-impl

install:
	sudo ${INSTALL_PROGRAM} ${DEST}/door-controller-impl ${BINDIR}/door-controller-impl
	sudo ${INSTALL_PROGRAM} ${DEST}/door-service-impl    ${BINDIR}/door-service-impl
	sudo ${INSTALL_PROGRAM} ${SCRIPT}/door-controller   ${BINDIR}/door-controller
	sudo ${INSTALL_PROGRAM} ${SCRIPT}/door-service      ${BINDIR}/door-service
	sudo ${INSTALL_PROGRAM} ${SCRIPT}/doornob           ${DAEMONDIR}/doornob
	sudo update-rc.d doornob defaults

uninstall:
	sudo rm ${BINDIR}/door-controller-impl
	sudo rm ${BINDIR}/door-service-impl
	sudo rm ${BINDIR}/door-controller
	sudo rm ${BINDIR}/door-service
	sudo rm ${DAEMONDIR}/doornob
	sudo update-rc.d -f doornob remove

deinstall: uninstall

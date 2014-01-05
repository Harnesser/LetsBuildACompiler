#! /usr/bin/env sh

PRJDIR=/home/harnesser/Projects/LetsBuildACompiler
TUTORIAL=12
FILENAME=main.c

# can't use command as terminal exits once it's done :(
#		--command="make"
#		--command="vi ${FILENAME}" 

gnome-terminal \
	--tab-with-profile=DEFAULT \
	--geometry=90x100+900+10 \
		--title=SRC \
		--working-directory=${PRJDIR}/${TUTORIAL} \
	--tab-with-profile=DEFAULT \
		--title=Test \
		--working-directory=${PRJDIR}/${TUTORIAL} &

tut=$(expr $TUTORIAL + 0)
gedit \
	--background \
	--geometry=60x120+100+10 \
	--new-window \
	${PRJDIR}/tutorials/tutor${tut}.txt \
	${PRJDIR}/test_data/kiss/assignments.txt \
	${PRJDIR}/${tut}/test.py &



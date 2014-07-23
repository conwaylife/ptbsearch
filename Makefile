CC = gcc
CFLAGS = -O3 -m64
NEEDED = gen.o rmo.o file.o analyze.o components.o lifelist.o gen2x2.o

gen:	genmain.o $(NEEDED)
	$(CC) $(CFLAGS) -o gen genmain.o $(NEEDED)

cumu:	cumu.o $(NEEDED)
	$(CC) $(CFLAGS) -o cumu cumu.o $(NEEDED)

touchpat:	touchpat.o $(NEEDED)
	$(CC) $(CFLAGS) -o touchpat touchpat.o $(NEEDED)

pos: pos.o $(NEEDED)
	$(CC) $(CFLAGS) -o pos pos.o $(NEEDED)

ptb2: ptb2.o ptbsearch2.o $(NEEDED)
	$(CC) $(CFLAGS) -o ptb2 ptb2.o ptbsearch2.o $(NEEDED)

normalstr: normalstr.o $(NEEDED)
	$(CC) $(CFLAGS) -o normalstr normalstr.o $(NEEDED)

stable:	stable.o $(NEEDED)
	$(CC) $(CFLAGS) -o stable stable.o $(NEEDED)

survive:	survive.o $(NEEDED)
	$(CC) $(CFLAGS) -o survive survive.o $(NEEDED)

lifeps: lifeps.o $(NEEDED)
	$(CC) $(CFLAGS) -o lifeps lifeps.o $(NEEDED)

lifefig: lifefig.o $(NEEDED)
	$(CC) $(CFLAGS) -o lifefig lifefig.o $(NEEDED)

accum: accum.o $(NEEDED)
	$(CC) $(CFLAGS) -o accum accum.o $(NEEDED)

makesigs: makesigs.o $(NEEDED)
	$(CC) $(CFLAGS) -o makesigs makesigs.o $(NEEDED)

watchsig: watchsig.o $(NEEDED)
	$(CC) $(CFLAGS) -o watchsig watchsig.o $(NEEDED)

repair: repair.o $(NEEDED)
	$(CC) $(CFLAGS) -o repair repair.o $(NEEDED)

newstable:	newstable.o $(NEEDED)
	$(CC) $(CFLAGS) -o newstable newstable.o $(NEEDED)

stablecomps:	stablecomps.o $(NEEDED)
	$(CC) $(CFLAGS) -o stablecomps stablecomps.o $(NEEDED)

testinteract:	testinteract.o quiescent.o $(NEEDED)
	$(CC) $(CFLAGS) -o testinteract testinteract.o quiescent.o $(NEEDED)

safestable:	safestable.o quiescent.o $(NEEDED)
	$(CC) $(CFLAGS) -o safestable safestable.o quiescent.o $(NEEDED)

quadstable:	quadstable.o $(NEEDED)
	$(CC) $(CFLAGS) -o quadstable quadstable.o $(NEEDED)

countmatch:	countmatch.o $(NEEDED)
	$(CC) $(CFLAGS) -o countmatch countmatch.o $(NEEDED)

omino:	omino.o $(NEEDED)
	$(CC) $(CFLAGS) -o omino omino.o $(NEEDED)

kings:	kings.o $(NEEDED)
	$(CC) $(CFLAGS) -o kings kings.o $(NEEDED)

lifecluster:	lifecluster.o $(NEEDED)
	$(CC) $(CFLAGS) -o lifecluster lifecluster.o $(NEEDED)

enumcluster:	enumcluster.o $(NEEDED)
	$(CC) $(CFLAGS) -o enumcluster enumcluster.o $(NEEDED) 

stablelist:	stablelist.o $(NEEDED)
	$(CC) $(CFLAGS) -o stablelist stablelist.o $(NEEDED)

osclist:	osclist.o $(NEEDED)
	$(CC) $(CFLAGS) -o osclist osclist.o $(NEEDED)

successor:	successor.o $(NEEDED)
	$(CC) $(CFLAGS) -o successor successor.o $(NEEDED)

poposc:	poposc.o $(NEEDED)
	$(CC) $(CFLAGS) -o poposc poposc.o $(NEEDED)

testpair:	testpair.o $(NEEDED)
	$(CC) $(CFLAGS) -o testpair testpair.o $(NEEDED)

vanish:	vanish.o $(NEEDED)
	$(CC) $(CFLAGS) -o vanish vanish.o $(NEEDED)

makejunk:	makejunk.o $(NEEDED)
	$(CC) $(CFLAGS) -o makejunk makejunk.o $(NEEDED)

testosc:	testosc.o $(NEEDED)
	$(CC) $(CFLAGS) -o testosc testosc.o $(NEEDED)

color:	gen.o rmo.o XmainColor.o file.o colors.o safe.o lifelist.o
	$(CC) $(CFLAGS) -o color gen.o rmo.o XmainColor.o file.o colors.o safe.o lifelist.o -lXm -lXt -lX11

viewca:	gen.o rmo.o Xmain.o file.o colors.o safe.o lifelist.o
	$(CC) $(CFLAGS) -o viewca gen.o rmo.o Xmain.o file.o colors.o safe.o lifelist.o -lXm -lXt -lX11

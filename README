This directory includes most of C source code I use currently for
Life analysis (with the exception of the files containing main() in
the tools not included here).  Very little of it is documented, but I 
will describe the components I use to look for still-life perturbations.  
Some data files and some perl scripts are also included.

------

The two executables needed for perturbation search are ptb2 and survive.
(Edit the Makefile if necessary and type make ptb2; make survive.)

ptb2:

ptb2 is invoked as:

ptb2 <test pattern> <list of perturbers> [maximum generation]
                                         [#perturbers-1]
                                         [minimum generation]
                                         [#perturbers that can be destroyed
                                          completely without halting search]

For example:
ptb2 picol.life ptb.list 80 2 20 1 > ptbpicol.out

This will try to add three perturbers to the collision between a glider
and block that results in a Pi heptomino.  The perturbation has to start
after generation 20 and both perturbers must be touched by step 80.

The last argument requires some explanation.  The cells of the perturbers
are labeled '*' or 'z' depending on whether they can ever disappear.  In
an eater, there are certain cells that persist in all known cases, so they
are not allowed to disappear (these are labeled) '*'.  Once they are
destroyed, the perturbation is considered a failure.  The others can 
disappear, but in some cases such as the block, all cells can potentially
disappear, and the last argument above limits the number of such
perturbers, since their presence can greatly increase search time.

Note that in picol.life, the glider cells are labeled 'a' and the
block cells are labeled 'b' to distinguish between them.  The
survive program uses this labeling.

survive:

survive tests which stable cells continue to exist after the pattern
has stabilized.  To use it, some cells in the initial pattern are
excluded from this test, while the others (assumed to be part of
a still life) are tested at each generation to see if they are
present.  These cells may disappear for some time, but must be restored
before the end of the testing period (hard-coded to 500 generations in
the program).  Since cells can be either *, a-z, or A-Z, the one argument
of survive specifies which cells to ignore.

For picol.life, 'survive a' only outputs patterns that restore the block,
while 'survive ab' will output any pattern such that the perturbers added
by ptb2 are not destroyed.

Pattern format:

   Like xlife picture format, except ! acts as a linefeed.
   Also, live cells can be represented by letters a-z A-Z
   in order to distinguish between parts of the pattern that survive

Order of fields in output

from ptb2:

   <pattern> <generation of first interaction>

from survive:

  <input line> <max #generations fixed part is "damaged" and then restored> \
               <#generations during last "damaged" period> \
               <last generation fixed part was intact> \
               <#cells - fixed part> \
               <final pattern - fixed part>

makematrix is a perl script that converts this output to an xlife
picture format file.

Example:

To find the initial pertubation in the original stable reflector, use
the command

   ptb2 picol.life ptb2.list 80 2 20 0 > ptbs.out

to generate a list of candidates, and

   survive a < ptbs.out > restore.out

to find the two that restore the block.

Note that this is not the same arguments to ptb2 given earlier.  In this
case, four different block patterns are used:

  zz   **   z*   *z
  **   zz   z*   *z

These indicate the different ways a block can be involved in a fast
perturbation, where just two cells are destroyed.  Then the last argument
is 0, because none of the perturbers have the potential to be destroyed
completely.  This makes the enumeration much faster.

To get an xlife file containing the two patterns in viewable form, use

   perl makematrix.pl < restore.out

----

ptb2 works in depth-first fashion without any capability of avoiding
recursion on effectively identical branches (e.g. the eater 2 has the
same effect as a fishhook eater).  To carry out longer searches, I
found it useful to add just one perturber at a time using some additional
scripts:

ptblist.pl is a perl script that runs ptb2 on a list of patterns to
be perturbed.  It uses the second column on each line as the first
step when the perturber can be added.

uniq.pl is a perl script that only prints one example of each final pattern
based on the last column of output from survive (but this could eliminate
results that are not really duplicates for reasons of spacing).

runall.sh is a shell script that adds one perturber at a time to
a list of patterns and outputs to ph1.out, ph2.out, ..., ph7.out

You can try 'runall.sh picol.life' for example.  This requires a lot
of disk space.
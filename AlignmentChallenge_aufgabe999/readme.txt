Implementieren Sie eine AlignmentChallenge.cpp und AlignmentChallenge.h und checken Sie diese zwei Dateien unter
<GITGROUP>/aufgabe999/ ins GIT ein.
Termin: Do, 18. Januar 2024, 23:59 Uhr


Implement an AlignmentChallenge.cpp and compile using the given Makefile, i.e.

$ make bench_AC

to generate your executable!
Before submission, compile it on the PCPool machines to check if its really working!
Your output should look something like this:

Usage: bench_AC filename.fa [threads=1 match=3 mismatch=-1 gap=-2]
./bench_AC mason_illumina_hg38_chr10_1K_150bp.fa 16 3 -1 -2
score: 65880477
time (wall): 2.58061 sec

Make sure the summed score is correct (using the above scoring parameters, threads notwithstanding), otherwise your code is doing something weird.


To know where you are standing (speedwise), run the original SeqAn code (Linux binary 'align_bench_par' provided) on your machine or the PC pool (the numbers below are for the XEON System):

change the number of threads (-t), and SSE4 (-v) according to your system.

####################################################################################################################
####################################################################################################################
####################################################################################################################

Parallel execution example:
./align_bench_par mason_illumina_hg38_chr10_1K_150bp.fa mason_illumina_hg38_chr10_1K_150bp.fa  -a dna --alignment-mode olc  -t 16

Reading sequences ...    done.
Generating Sequences ...         done.
Invoke Alignment...                             done.

================================================================================

2.5291 sec

================================================================================

Policy,State,#Sequences,#Alignments,sorted,banded#GCells,Dist,BitsPerScore,Alphabet,Mode,Method,Time,GCUPS,BlockSize,#Threads,#Instances,VectorSize,#SerialBlocks,#SimdBlocks
parallel,done,499500,499500,no,no,1.13891e+10,,int32_t,dna,olc,global,2.8291,4.0257,n/a,16,n/a,n/a,n/a,n/a

####################################################################################################################
####################################################################################################################
####################################################################################################################

Parallel + SSE4 (using -v):
./align_bench_par mason_illumina_hg38_chr10_1K_150bp.fa mason_illumina_hg38_chr10_1K_150bp.fa  -a dna --alignment-mode olc  -t 16 -v

Reading sequences ...    done.
Generating Sequences ...         done.
Invoke Alignment...                             done.

================================================================================

0.22536 sec

================================================================================

Policy,State,#Sequences,#Alignments,sorted,banded#GCells,Dist,BitsPerScore,Alphabet,Mode,Method,Time,GCUPS,BlockSize,#Threads,#Instances,VectorSize,#SerialBlocks,#SimdBlocks
parallel_vec,done,499500,499500,no,no,1.13891e+10,,int32_t,dna,olc,global,0.442594,25.7326,n/a,16,n/a,4,n/a,n/a




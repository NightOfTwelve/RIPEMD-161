ripemd160: ripemd-impl.c ripemd-consts.c ripemd.h
	gcc -o ripemd ripemd-impl.c ripemd-consts.c -I.

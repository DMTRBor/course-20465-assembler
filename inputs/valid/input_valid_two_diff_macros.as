MAIN: mov M1[r2][r7],LENGTH
      add r2,STR
LOOP: jmp END
      prn #-5
	  mcro a_mc
	  mov M1[r3][r3],r3
	  bne LOOP
	  mcroend
	  sub r1, r4
      mcro b_mc
	  mov M1[r4][r4],r4
	  bne LOOP
	  mcroend
	  inc K

	  a_mc
END: stop
STR: .string "abcdef"
LENGTH: .data 6,-9,15
LOOB: jmp END
	  sub r1, r4
	  b_mc
K: .data 22
M1: .mat [2][2] 1,2,3,4
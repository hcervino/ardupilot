;******************************************************************************
;* PRU C/C++ Codegen                                            Unix v2.0.0B2 *
;* Date/Time created: Wed Mar  5 00:14:13 2014                                *
;******************************************************************************
	.compiler_opts --abi=eabi --endian=little --hll_source=on --object_format=elf --silicon_version=3 --symdebug:none 
	.global	__PRU_CREG_C0
	.global	__PRU_CREG_C26
	.global	__PRU_CREG_C4
	.global	||C0||
||C0||:	.usect	".creg.C0.far",768,1
	.global	||C26||
||C26||:	.usect	".creg.C26.near",256,1
	.global	||C4||
||C4||:	.usect	".creg.C4.near",256,1
;	optpru /tmp/01129Ebd07y /tmp/01129uWjh3X 
;	acpiapru -@/tmp/01129autqNq 
	.sect	".text:main"
	.clink
	.global	||main||

;***************************************************************
;* FNAME: main                          FR SIZE:   0           *
;*                                                             *
;* FUNCTION ENVIRONMENT                                        *
;*                                                             *
;* FUNCTION PROPERTIES                                         *
;*                            0 Auto,  0 SOE     *
;***************************************************************

||main||:
;* --------------------------------------------------------------------------*
;*** 31	-----------------------    *((C$2 = &C4[0])+4) &= 0xffffffefu;
        LDI32     r0, ||C4||            ; [] |31| $O$C2,C4
        ADD       r1, r0, 0x04          ; [] |31| $O$C2
        LBBO      &r14, r1, 0, 4        ; [] |31| 
        CLR       r14, r14, 0x00000004  ; [] |31| 
        SBBO      &r14, r1, 0, 4        ; [] |31| 
;*** 32	-----------------------    ((volatile unsigned *)C4)[1] = ((volatile unsigned *)C4)[1]|5u;
        LBCO      &r1, __PRU_CREG_C4, $CSBREL(||C4||+4), 4 ; [] |32| C4
        OR        r1, r1, 0x05          ; [] |32| 
        SBCO      &r1, __PRU_CREG_C4, $CSBREL(||C4||+4), 4 ; [] |32| C4
;*** 37	-----------------------    ((volatile unsigned *)C$2)[13] |= 1u;
        LBBO      &r1, r0, 52, 4        ; [] |37| $O$C2
        SET       r1, r1, 0x00000000    ; [] |37| 
        SBBO      &r1, r0, 52, 4        ; [] |37| $O$C2
;*** 41	-----------------------    *(C$1 = &((volatile unsigned *)C26)[0]) = 272u;
        LDI32     r0, ||C26||           ; [] |41| $O$C1,C26
        LDI       r1, 0x0110            ; [] |41| 
        SBBO      &r1, r0, 0, 4         ; [] |41| $O$C1
;*** 43	-----------------------    *((unsigned char *)C$1+68) = 2u;
        LDI       r1, 0x0002            ; [] |43| 
        SBBO      &r1, r0, 68, 4        ; [] |43| $O$C1
;*** 44	-----------------------    *((unsigned char *)C$1+76) = 0u;
        LDI       r1, 0x0000            ; [] |44| 
        SBBO      &r1, r0, 76, 4        ; [] |44| $O$C1
;*** 45	-----------------------    *((unsigned char *)C$1+64) |= 4u;
        LBBO      &r1, r0, 64, 4        ; [] |45| $O$C1
        SET       r1, r1, 0x00000002    ; [] |45| 
        SBBO      &r1, r0, 64, 4        ; [] |45| $O$C1
;*** 46	-----------------------    ((volatile unsigned *)C26)[0] |= 1u;
        LBCO      &r0, __PRU_CREG_C26, $CSBREL(||C26||+0), 4 ; [] |46| C26
        SET       r0, r0, 0x00000000    ; [] |46| 
        SBCO      &r0, __PRU_CREG_C26, $CSBREL(||C26||+0), 4 ; [] |46| C26
;***  	-----------------------    #pragma LOOP_FLAGS(4096u)
;* --------------------------------------------------------------------------*
;*   BEGIN LOOP ||$C$L1||
;*
;*   Loop source line                : 50
;*   Loop closing brace source line  : 52
;*   Known Minimum Trip Count        : 1
;*   Known Maximum Trip Count        : 4294967295
;*   Known Max Trip Count Factor     : 1
;* --------------------------------------------------------------------------*
||$C$L1||:    
;***	-----------------------g2:
;*** 22	-----------------------    cnt = ((volatile unsigned *)C26)[3];  // [0]
        LBCO      &r0, __PRU_CREG_C26, $CSBREL(||C26||+12), 4 ; [] |22| cnt,C26
;*** 52	-----------------------    if ( !(next-cnt&0x80000000u) ) goto g2;
        SUB       r0, r1, r0            ; [] |52| next,cnt
        QBBC      ||$C$L1||, r0, 0x1f   ; [] |52| 
;* --------------------------------------------------------------------------*
;*** 53	-----------------------    return 0;
        ZERO      &r14, 4               ; [] |53| 
        JMP       r3.w2                 ; [] 
;* Inlined function references:
;* [  0] read_PIEP_COUNT

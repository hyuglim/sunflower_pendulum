# 1 "init.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "init.S"
# 1 "asm.h" 1
# 2 "init.S" 2

        .global _getpsr
        .global _getssr
        .global _getspc
        .global _getgbr
        .global _getvbr
        .global _getsp
        .global _getmach
        .global _getmacl
        .global _getpr
 .global _sleep
 .global _vec_stub_begin
 .global _vec_stub_end
 .global _splhi
 .global _spllo
 .global _spldone


 .align 2
start:

 AND #0, r0
 LDC r0, sr


 MOV.L app_stack_addr, r15
 MOV.L start_addr, r0
 JSR @r0
 NOP


        mov #1, r4
        trapa #34


_tas:
 TAS.B @r4
 STC sr, r0
 AND #1, r0
 RTS
 NOP


_splhi:
 MOV #(1<<4), r0
 SHLL16 r0
 SHLL8 r0
 STC sr, r1
 OR r0, r1
 LDC r1, sr
        RTS
 NOP


_spllo:
 MOV #(1<<4), r0
 SHLL16 r0
 SHLL8 r0
 NOT r0, r0
 STC sr, r1
 AND r0, r1
 LDC r1, sr
        RTS
 NOP


_spldone:

 LDC r4, sr
        RTS
 NOP


_getpsr:
        STC sr, r0
        RTS
 NOP


_getssr:
        STC ssr, r0
        RTS
 NOP


_getspc:
        STC spc, r0
        RTS
 NOP


_getgbr:
        STC gbr, r0
        RTS
 NOP


_getvbr:
        STC vbr, r0
        RTS
 NOP


_getsp:
        MOV r15, r0
        RTS
 NOP


_getpr:
        STS pr, r0
        RTS
 NOP


_getmach:
        STS mach, r0
        RTS
 NOP


_getmacl:
        STS macl, r0
        RTS
 NOP



 .align 4
_vec_stub_begin:

 MOV.L savestack_addr, r0
 ADD #36, r0
 STS.L pr, @-r0


 BSR saveregs
 NOP


 MOV.L intr_stack_addr, r15
 MOV.L hdlr_addr, r0
 JSR @r0
 NOP


 BSR restoreregs
 NOP


 MOV.L savestack_addr, r0
 ADD #32, r0
 LDS.L @r0+, pr


 RTE
 NOP







saveregs:

 MOV.L savestack_addr, r0


 ADD #32, r0


 MOV.L r15, @-r0
 MOV.L r14, @-r0
 MOV.L r13, @-r0
 MOV.L r12, @-r0
 MOV.L r11, @-r0
 MOV.L r10, @-r0
 MOV.L r9, @-r0
 MOV.L r8, @-r0

 RTS
 NOP





restoreregs:

 MOV.L savestack_addr, r0


 MOV.L @r0+, r8
 MOV.L @r0+, r9
 MOV.L @r0+, r10
 MOV.L @r0+, r11
 MOV.L @r0+, r12
 MOV.L @r0+, r13
 MOV.L @r0+, r14
 MOV.L @r0+, r15

 RTS
 NOP

 .align 4
 hdlr_addr:
 .long _intr_hdlr

 .align 4
 savestack_addr:
 .long _REGSAVESTACK

 .align 4
 intr_stack_addr:
 .long (0x8000000 + (1 << 20))
_vec_stub_end:



_sleep:
 SLEEP
 RTS
 NOP



 .align 2
app_stack_addr:
 .long (0x8000000 + (1 << 17))

 .align 2
start_addr:
 .long _startup

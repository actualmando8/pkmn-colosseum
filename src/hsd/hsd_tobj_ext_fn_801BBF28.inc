    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0xa
    stw r0, 0x14(r1)
    bgt @L_801BBFAC
    lis r4, jumptable_8036D5A4@ha
    rlwinm r0, r3, 2, 0, 29
    addi r4, r4, jumptable_8036D5A4@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    li r3, 0x1e
    b @L_801BBFD4
    li r3, 0x21
    b @L_801BBFD4
    li r3, 0x24
    b @L_801BBFD4
    li r3, 0x27
    b @L_801BBFD4
    li r3, 0x2a
    b @L_801BBFD4
    li r3, 0x2d
    b @L_801BBFD4
    li r3, 0x30
    b @L_801BBFD4
    li r3, 0x33
    b @L_801BBFD4
    li r3, 0x36
    b @L_801BBFD4
    li r3, 0x39
    b @L_801BBFD4
    li r3, 0x3c
    b @L_801BBFD4
@L_801BBFAC:
    lis r5, lbl_8027565C@ha
    mr r4, r3
    addi r3, r5, lbl_8027565C@l
    crxor 6, 6, 6
    bl OSReport
    addi r3, r2, -22512
    li r4, 0x7e1
    addi r5, r2, -22476
    bl fn_80196D78
    li r3, 0x3c
@L_801BBFD4:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr


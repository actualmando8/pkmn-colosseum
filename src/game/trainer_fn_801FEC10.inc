    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr. r30, r3
    bne @L_801FEC30
    li r3, 0x0
    b @L_801FECC0
    @L_801FEC30:
    li r4, 0x0
    li r5, 0xd5
    li r6, 0x0
    bl fn_8012640C
    mr r31, r3
    mr r3, r30
    li r4, 0x0
    li r5, 0xd7
    li r6, 0x0
    bl fn_8012640C
    cmplwi r31, 0x0
    mr r7, r3
    bne @L_801FEC6C
    li r3, 0x0
    b @L_801FECC0
    @L_801FEC6C:
    cmplwi r7, 0x0
    bne @L_801FEC7C
    li r3, 0x0
    b @L_801FECC0
    @L_801FEC7C:
    li r0, 0x2a
    subi r5, r7, 0x4
    subi r4, r31, 0x4
    mtctr r0
    @L_801FEC8C:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_801FEC8C
    lwz r0, 0x4(r4)
    mr r3, r30
    li r4, 0x0
    li r6, 0x0
    stw r0, 0x4(r5)
    li r5, 0xd6
    bl fn_801254B4
    li r3, 0x1
    @L_801FECC0:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

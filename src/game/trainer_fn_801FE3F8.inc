    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r5, 0xd6
    li r6, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r31, r4
    mr r30, r3
    li r4, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_801FE430
    li r3, 0x0
    b @L_801FE440
    @L_801FE430:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    @L_801FE440:
    cmplwi r3, 0x0
    beq @L_801FE450
    mr r4, r31
    bl fn_801204A8
    @L_801FE450:
    stw r30, 0x40(r31)
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r5, 0xff
    li r6, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r31, r4
    mr r30, r3
    li r4, 0x0
    bl fn_8012640C
    rlwinm r3, r3, 0, 16, 31
    rlwinm r0, r31, 0, 16, 31
    cmplw r0, r3
    beq @L_80207B3C
    mr r3, r30
    li r4, 0x0
    li r5, 0xff
    li r6, 0x1
    bl fn_8012640C
    rlwinm r3, r3, 0, 16, 31
    rlwinm r0, r31, 0, 16, 31
    cmplw r0, r3
    bne @L_80207B44
@L_80207B3C:
    li r3, 0x1
    b @L_80207B48
@L_80207B44:
    li r3, 0x0
@L_80207B48:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

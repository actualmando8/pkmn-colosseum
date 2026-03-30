    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    li r30, 0x0
    li r31, 0x1
    b @L_801EF2A8
@L_801EF290:
    mr r3, r31
    bl fn_801EEAD0
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801EF2A4
    addi r30, r30, 0x1
@L_801EF2A4:
    addi r31, r31, 0x1
@L_801EF2A8:
    lwz r3, -30904(r13)
    rlwinm r4, r31, 0, 16, 31
    lwz r0, 0x0(r3)
    cmplw r4, r0
    blt @L_801EF290
    mr r3, r30
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

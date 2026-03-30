    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r31, r4
    mr r30, r3
    li r3, 0x4
    mr r5, r31
    li r4, 0x0
    bl fn_801F02AC
    mr. r4, r3
    bne @L_801F0108
    li r3, 0x0
    b @L_801F0120
    @L_801F0108:
    mr r3, r30
    mr r5, r31
    bl fn_801F02AC
    cmplwi r3, 0x0
    bne @L_801F0120
    li r3, 0x0
    @L_801F0120:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r30, r4
    mr r29, r3
    li r3, 0x4
    mr r5, r30
    li r4, 0x0
    bl fn_801F02AC
    mr. r31, r3
    bne @L_801F0090
    li r3, 0x0
    b @L_801F00BC
    @L_801F0090:
    mr r4, r29
    mr r5, r30
    li r3, 0x2
    bl fn_801F02AC
    cmplwi r3, 0x0
    bne @L_801F00B0
    li r3, 0x0
    b @L_801F00BC
    @L_801F00B0:
    subf r0, r3, r31
    cntlzw r0, r0
    srwi r3, r0, 5
    @L_801F00BC:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

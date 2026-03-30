    nofralloc
    mflr r0
    addi r7, r5, 0x0
    stw r0, 0x4(r1)
    li r0, 0x0
    li r5, 0x0
    stwu r1, -0x20(r1)
    li r6, 0x7
    li r8, 0x0
    stw r0, 0x8(r1)
    li r9, 0x0
    li r10, 0x0
    stw r0, 0xc(r1)
    bl fn_800BB780
    lwz r0, 0x24(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr

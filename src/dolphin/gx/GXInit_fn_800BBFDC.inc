    nofralloc
    mflr r0
    li r4, 0x0
    stw r0, 0x4(r1)
    li r0, 0x0
    li r5, 0x0
    stwu r1, -0x18(r1)
    li r6, 0x0
    li r7, 0x0
    stw r0, 0x8(r1)
    li r8, 0x6
    li r9, 0x6
    stw r0, 0xc(r1)
    li r10, 0x1
    bl fn_800BB780
    lwz r0, 0x1c(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr

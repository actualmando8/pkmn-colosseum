    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x1a
    stw r0, 0x14(r1)
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r0, r3, 16
    subfic r0, r0, 0x10
    cntlzw r0, r0
    extrwi r3, r0, 8, 19
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

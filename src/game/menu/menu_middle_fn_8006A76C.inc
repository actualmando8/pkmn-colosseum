    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_801D04E8
    clrlwi r0, r3, 24
    cntlzw r0, r0
    srwi r0, r0, 5
    clrlwi r3, r0, 24
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

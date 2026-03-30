    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x10(r1)
    addi r3, r1, 0xc
    addi r4, r1, 0x8
    bl fn_800BE164
    lwz r3, 0xc(r1)
    lwz r0, 0x14(r1)
    addi r1, r1, 0x10
    mtlr r0
    blr

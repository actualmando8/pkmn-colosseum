    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r5, r4
    stw r0, 0x14(r1)
    lfs f1, 0x0(r4)
    mr r4, r3
    lfs f2, 0x4(r5)
    lfs f3, 0x8(r5)
    bl fn_800A32E8
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

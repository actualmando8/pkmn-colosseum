    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r6, r3
    mr r3, r4
    stw r0, 0x14(r1)
    mr r0, r5
    mr r5, r6
    mr r4, r0
    bl fn_800A3D3C
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

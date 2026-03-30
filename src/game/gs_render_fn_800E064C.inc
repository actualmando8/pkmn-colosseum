    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, lbl_80315568@ha
    li r5, 0x30
    stw r0, 0x14(r1)
    addi r4, r4, lbl_80315568@l
    bl memcpy
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

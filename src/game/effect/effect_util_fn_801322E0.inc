    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lis r3, lbl_80427170@ha
    lwz r5, lbl_8047AE68(r13)
    addi r3, r3, lbl_80427170@l
    li r4, 0x10
    li r6, 0x0
    bl fn_80132834
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

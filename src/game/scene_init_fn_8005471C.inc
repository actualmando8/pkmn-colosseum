    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r3, 0x8f
    li r4, 0x2
    li r5, 0x0
    bl fn_80102568
    li r3, 0x8f
    li r4, 0x1
    bl fn_80102428
    lis r3, lbl_803A9720@ha
    addi r3, r3, lbl_803A9720@l
    bl fn_8010A420
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

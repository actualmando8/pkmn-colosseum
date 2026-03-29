    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r3, 0xaa
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x7a
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x7f
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    lwz r3, lbl_8047A35C(r13)
    bl fn_801CB9D8
    lis r3, lbl_803A1FF8@ha
    addi r3, r3, lbl_803A1FF8@l
    bl fn_8010A420
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

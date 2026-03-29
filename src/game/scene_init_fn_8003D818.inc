    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    lis r3, lbl_803A6818@ha
    li r30, 0x0
    mr r29, r30
    addi r31, r3, lbl_803A6818@l
    b @L_8003D868
    @L_8003D844:
    mr r4, r29
    li r3, 0x0
    bl fn_8025FE84
    clrlwi r0, r29, 16
    lwz r4, lbl_8047A4E4(r13)
    slwi r0, r0, 1
    addi r30, r30, 0x1
    sthx r3, r4, r0
    addi r29, r29, 0x1
    @L_8003D868:
    li r3, 0x0
    bl fn_8025FEE4
    clrlwi r3, r3, 16
    clrlwi r0, r29, 16
    cmplw r0, r3
    blt @L_8003D844
    li r0, 0x0
    sth r30, lbl_8047A4E8(r13)
    stb r0, 0x158(r31)
    stb r0, 0x159(r31)
    stb r0, 0x15b(r31)
    stb r0, 0x15a(r31)
    stb r0, 0x15c(r31)
    bl fn_8003E394
    lis r4, lbl_803A6818@ha
    lhz r3, lbl_8047A4E8(r13)
    addi r4, r4, lbl_803A6818@l
    stw r3, 0x10(r4)
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

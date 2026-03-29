    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    lis r4, lbl_80267120@ha
    stw r3, 0x8(r1)
    addi r6, r4, lbl_80267120@l
    lwz r5, 0x0(r6)
    lwz r4, 0x4(r6)
    lwz r3, 0x8(r6)
    lwz r0, 0xc(r6)
    stw r5, 0xc(r1)
    stw r4, 0x10(r1)
    stw r3, 0x14(r1)
    stw r0, 0x18(r1)
    bl fn_801046B8
    mr r4, r3
    addi r5, r1, 0x8
    li r3, 0x24
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    mr r31, r3
    li r3, 0x24
    bl fn_80102510
    li r3, 0x24
    li r4, 0x1
    bl fn_80102428
    cmpwi r31, 0x0
    blt @L_80039520
    cmpwi r31, 0x4
    blt @L_80039528
    @L_80039520:
    li r3, 0x4
    b @L_80039534
    @L_80039528:
    slwi r0, r31, 2
    addi r3, r1, 0xc
    lwzx r3, r3, r0
    @L_80039534:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

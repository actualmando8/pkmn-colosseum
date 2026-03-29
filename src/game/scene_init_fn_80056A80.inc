    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    addi r30, r1, 0x14
    li r29, 0x0
    lis r3, lbl_8026768C@ha
    addi r31, r3, lbl_8026768C@l
    @L_80056AA8:
    lwz r4, 0x0(r31)
    cmpwi r29, 0x0
    lwz r3, 0x4(r31)
    lwz r0, 0x8(r31)
    stw r4, 0x14(r1)
    stw r3, 0x18(r1)
    stw r0, 0x1c(r1)
    blt @L_80056AD0
    cmpwi r29, 0x3
    blt @L_80056AD8
    @L_80056AD0:
    li r3, -0x1
    b @L_80056ADC
    @L_80056AD8:
    lwz r3, 0x0(r30)
    @L_80056ADC:
    cmpwi r3, 0x0
    blt @L_80056AF0
    li r4, 0x2
    li r5, 0x0
    bl fn_80102568
    @L_80056AF0:
    addi r30, r30, 0x4
    addi r29, r29, 0x1
    cmpwi r29, 0x3
    blt @L_80056AA8
    lis r3, lbl_8026768C@ha
    lwz r6, lbl_8047A584(r13)
    addi r5, r3, lbl_8026768C@l
    lwz r4, 0x0(r5)
    cmpwi r6, 0x0
    lwz r3, 0x4(r5)
    lwz r0, 0x8(r5)
    stw r4, 0x8(r1)
    stw r3, 0xc(r1)
    stw r0, 0x10(r1)
    blt @L_80056B34
    cmpwi r6, 0x3
    blt @L_80056B3C
    @L_80056B34:
    li r3, -0x1
    b @L_80056B48
    @L_80056B3C:
    slwi r0, r6, 2
    addi r3, r1, 0x8
    lwzx r3, r3, r0
    @L_80056B48:
    cmpwi r3, 0x0
    blt @L_80056B58
    li r4, 0x1
    bl fn_80102428
    @L_80056B58:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

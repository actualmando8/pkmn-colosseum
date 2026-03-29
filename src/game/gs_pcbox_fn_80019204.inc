    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    lwz r5, 0x60(r3)
    lis r6, lbl_80266C00@ha
    addi r7, r6, lbl_80266C00@l
    lbz r3, 0x95(r3)
    lwz r0, 0xc(r5)
    lwz r6, 0x0(r7)
    extsb r3, r3
    subfic r0, r0, 0x4
    lwz r5, 0x4(r7)
    add r8, r3, r0
    lwz r3, 0x8(r7)
    lwz r0, 0xc(r7)
    cmpwi r8, 0x0
    stw r6, 0x8(r1)
    stw r5, 0xc(r1)
    stw r3, 0x10(r1)
    stw r0, 0x14(r1)
    blt @L_80019260
    cmpwi r8, 0x4
    blt @L_80019268
    @L_80019260:
    li r3, 0x0
    b @L_80019298
    @L_80019268:
    slwi r0, r8, 2
    addi r5, r1, 0x8
    lha r6, 0x6(r4)
    mr r3, r4
    lwzx r0, r5, r0
    cmpw r6, r0
    bne @L_8001928C
    li r4, 0x1
    b @L_80019290
    @L_8001928C:
    li r4, 0x0
    @L_80019290:
    bl fn_80109220
    li r3, 0x0
    @L_80019298:
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

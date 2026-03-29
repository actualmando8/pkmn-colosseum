    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r3
    lis r3, lbl_802672C8@ha
    addi r6, r3, lbl_802672C8@l
    lwz r5, 0x0(r6)
    lwz r4, 0x4(r6)
    lwz r3, 0x8(r6)
    lwz r0, 0xc(r6)
    stw r5, 0x8(r1)
    stw r4, 0xc(r1)
    stw r3, 0x10(r1)
    stw r0, 0x14(r1)
    bl fn_801D1B4C
    cmpwi r3, 0x0
    blt @L_8004C438
    cmpwi r3, 0x4
    blt @L_8004C440
    @L_8004C438:
    li r3, 0x0
    b @L_8004C48C
    @L_8004C440:
    slwi r0, r3, 2
    addi r3, r1, 0x8
    lwzx r31, r3, r0
    li r0, -0x100
    lbz r5, 0x8b(r30)
    li r3, 0x0
    mr r6, r31
    li r4, 0x0
    or r30, r5, r0
    mr r5, r30
    bl fn_800FB680
    mr r3, r31
    bl fn_800FA444
    srwi r3, r3, 16
    mr r5, r30
    li r4, 0x0
    li r6, 0x36c1
    bl fn_800FB680
    li r3, 0x0
    @L_8004C48C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

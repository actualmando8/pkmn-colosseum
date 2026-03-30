    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr. r29, r3
    beq @L_8013E638
    lbz r0, 0x18(r29)
    lis r3, lbl_80466BC0@ha
    addi r31, r3, lbl_80466BC0@l
    cmplwi r0, 0x1e
    ble @L_8013E5E8
    li r0, 0x1e
    stb r0, 0x18(r29)
    @L_8013E5E8:
    lhz r3, 0x4(r31)
    li r5, 0xa0
    lhz r4, 0x6(r31)
    li r6, 0x0
    li r7, 0x0
    bl fn_800EF5FC
    mr. r30, r3
    beq @L_8013E638
    stw r30, 0x4(r29)
    li r4, 0x0
    bl fn_800EF548
    lhz r5, 0x4(r31)
    li r4, 0x0
    lhz r0, 0x6(r31)
    mullw r5, r5, r0
    bl memset
    mr r3, r30
    bl fn_800EF504
    li r3, 0x1
    b @L_8013E63C
    @L_8013E638:
    li r3, 0x0
    @L_8013E63C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

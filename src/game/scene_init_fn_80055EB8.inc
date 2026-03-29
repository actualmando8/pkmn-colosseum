    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r4
    li r31, 0x0
    bl fn_80057A08
    cmpwi r3, 0x0
    beq @L_80055F60
    li r3, 0x93
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_80055F60
    lbz r4, 0x95(r3)
    addi r3, r1, 0xc
    extsb r4, r4
    bl fn_80055194
    cmpwi r3, 0x0
    bne @L_80055F60
    addi r3, r1, 0xa
    addi r4, r1, 0x8
    li r31, 0x1
    bl fn_80057094
    lha r0, 0x6(r30)
    lis r3, lbl_802EF0A8@ha
    addi r5, r3, lbl_802EF0A8@l
    lha r4, 0xa(r1)
    mulli r0, r0, 0x1c
    add r3, r5, r0
    lha r0, 0x2(r3)
    add r0, r4, r0
    extsh r0, r0
    sth r0, 0x50(r30)
    lha r0, 0x6(r30)
    lha r4, 0x8(r1)
    mulli r0, r0, 0x1c
    add r3, r5, r0
    lha r0, 0x4(r3)
    add r0, r4, r0
    extsh r0, r0
    sth r0, 0x52(r30)
    @L_80055F60:
    mr r3, r30
    clrlwi r4, r31, 24
    bl fn_80109220
    li r3, 0x0
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

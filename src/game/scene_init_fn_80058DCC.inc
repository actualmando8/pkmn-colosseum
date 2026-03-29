    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    lbz r0, 0x1(r30)
    lwz r3, 0x60(r30)
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_80058EA4
    bge @L_80058E08
    cmpwi r0, 0x0
    beq @L_80058E14
    b @L_80058EEC
    @L_80058E08:
    cmpwi r0, 0x4
    bge @L_80058EEC
    b @L_80058ED4
    @L_80058E14:
    lbz r0, 0x2(r30)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80058E2C
    li r0, 0x1
    stb r0, 0x2(r30)
    @L_80058E2C:
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    beq @L_80058E84
    li r31, 0x0
    @L_80058E3C:
    clrlwi r5, r31, 16
    li r3, 0x0
    li r4, 0x3
    bl fn_8012A5B0
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80058E68
    addi r31, r31, 0x1
    cmpwi r31, 0x6
    blt @L_80058E3C
    @L_80058E68:
    cmpwi r31, 0x6
    blt @L_80058E74
    li r31, -0x1
    @L_80058E74:
    cmpwi r31, 0x0
    blt @L_80058E84
    extsb r0, r31
    stb r0, 0x95(r30)
    @L_80058E84:
    mr r3, r30
    bl fn_80104318
    mr r4, r3
    li r5, 0x0
    lha r3, 0x2(r4)
    lha r4, 0x4(r4)
    bl fn_80057830
    b @L_80058EEC
    @L_80058EA4:
    bl fn_801046B8
    lwz r0, 0x4(r30)
    cmpw r0, r3
    bne @L_80058EEC
    mr r3, r30
    bl fn_80058AF0
    cmpwi r3, 0x0
    bne @L_80058EEC
    li r0, 0x1
    stb r0, 0x98(r30)
    stb r0, 0x99(r30)
    b @L_80058EEC
    @L_80058ED4:
    lbz r0, 0x2(r30)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80058EEC
    li r0, 0x1
    stb r0, 0x2(r30)
    @L_80058EEC:
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

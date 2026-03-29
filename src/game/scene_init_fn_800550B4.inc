    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    lbz r0, 0x1(r30)
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_8005513C
    bge @L_800550EC
    cmpwi r0, 0x0
    beq @L_800550F8
    b @L_80055178
    @L_800550EC:
    cmpwi r0, 0x4
    bge @L_80055178
    b @L_80055160
    @L_800550F8:
    lbz r0, 0x2(r30)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80055110
    li r31, 0x1
    b @L_80055114
    @L_80055110:
    li r31, 0x0
    @L_80055114:
    mr r3, r30
    bl fn_80104318
    mr r4, r3
    mr r5, r31
    lha r3, 0x2(r4)
    lha r4, 0x4(r4)
    bl fn_80057830
    li r0, 0x1
    stb r0, 0x2(r30)
    b @L_80055178
    @L_8005513C:
    bl fn_80057948
    bl fn_80056854
    bl fn_801046B8
    lwz r0, 0x4(r30)
    cmpw r0, r3
    bne @L_80055178
    mr r3, r30
    bl fn_80054EC8
    b @L_80055178
    @L_80055160:
    lbz r0, 0x2(r30)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80055178
    li r0, 0x1
    stb r0, 0x2(r30)
    @L_80055178:
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r30, r3
    bl fn_801040A0
    lbz r0, 0x1(r30)
    mr r31, r3
    li r29, 0x0
    extsb r0, r0
    cmpwi r0, 0x3
    beq @L_80105BD8
    bge @L_80105A90
    cmpwi r0, 0x1
    beq @L_80105C10
    bge @L_80105B88
    cmpwi r0, 0x0
    bge @L_80105A9C
    b @L_80105C10
@L_80105A90:
    cmpwi r0, 0x5
    beq @L_80105C00
    b @L_80105C10
@L_80105A9C:
    lwz r3, 0x0(r31)
    cmplwi r3, 0x0
    beq @L_80105AAC
    bl fn_800FBD88
@L_80105AAC:
    mr r3, r30
    li r4, 0x0
    bl fn_801040D0
    stw r3, 0x0(r31)
    mr r3, r30
    li r4, 0x1
    bl fn_801040D0
    rlwinm r0, r3, 0, 24, 31
    mr r3, r30
    stb r0, 0x4(r31)
    li r4, 0x2
    bl fn_801040D0
    rlwinm r0, r3, 0, 24, 31
    stb r0, 0x5(r31)
    lbz r0, 0x2(r30)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80105AF8
    li r29, 0x1
@L_80105AF8:
    lwz r3, 0x4(r30)
    li r4, 0x2a
    bl fn_80107ED8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80105B14
    li r29, 0x1
@L_80105B14:
    lbz r0, 0xa(r30)
    cmplwi r0, 0x0
    beq @L_80105B24
    li r29, 0x1
@L_80105B24:
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80105B58
    lwz r3, 0x4(r30)
    cmpwi r3, 0x40
    beq @L_80105B48
    li r4, 0x22
    bl fn_801080CC
    b @L_80105B50
@L_80105B48:
    li r4, 0x26
    bl fn_801080CC
@L_80105B50:
    li r0, 0x1
    stb r0, 0x2(r30)
@L_80105B58:
    lwz r0, 0x4(r30)
    cmpwi r0, 0x50
    bne @L_80105C10
    lwz r3, 0x0(r31)
    bl fn_800FA444
    rlwinm r4, r3, 16, 16, 31
    rlwinm r0, r3, 0, 16, 31
    extsh r3, r4
    sth r3, 0xc(r31)
    extsh r0, r0
    sth r0, 0xe(r31)
    b @L_80105C10
@L_80105B88:
    lbz r0, 0x2(r30)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80105BB0
    lwz r3, 0x0(r31)
    lbz r4, 0x4(r31)
    lbz r5, 0x5(r31)
    bl fn_800FBF74
    li r0, 0x1
    stb r0, 0x2(r30)
@L_80105BB0:
    lwz r3, 0x0(r31)
    bl fn_800FA3D0
    cmpwi r3, 0x0
    beq @L_80105BCC
    li r0, 0x0
    stb r0, 0x98(r30)
    b @L_80105C10
@L_80105BCC:
    li r0, 0x1
    stb r0, 0x98(r30)
    b @L_80105C10
@L_80105BD8:
    lbz r0, 0x2(r30)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80105C10
    lwz r3, 0x4(r30)
    li r4, 0x2a
    bl fn_801080CC
    li r0, 0x1
    stb r0, 0x2(r30)
    b @L_80105C10
@L_80105C00:
    lwz r3, 0x0(r31)
    bl fn_800FBD88
    li r0, 0x0
    stw r0, 0x0(r31)
@L_80105C10:
    li r3, 0x0
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

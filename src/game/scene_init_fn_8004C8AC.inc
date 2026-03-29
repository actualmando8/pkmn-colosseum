    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stw r31, 0x3c(r1)
    stw r30, 0x38(r1)
    mr r31, r4
    lis r4, lbl_80267278@ha
    li r3, 0xa
    addi r30, r4, lbl_80267278@l
    lwz r12, 0x0(r30)
    lwz r11, 0x4(r30)
    lwz r10, 0x8(r30)
    lwz r9, 0xc(r30)
    lwz r8, 0x10(r30)
    lwz r7, 0x14(r30)
    lwz r6, 0x18(r30)
    lwz r5, 0x1c(r30)
    lwz r4, 0x20(r30)
    lwz r0, 0x24(r30)
    stw r12, 0x10(r1)
    stw r11, 0x14(r1)
    stw r10, 0x18(r1)
    stw r9, 0x1c(r1)
    stw r8, 0x20(r1)
    stw r7, 0x24(r1)
    stw r6, 0x28(r1)
    stw r5, 0x2c(r1)
    stw r4, 0x30(r1)
    stw r0, 0x34(r1)
    bl fn_80103E68
    srwi r0, r3, 16
    li r30, 0x0
    sth r0, 0x8(r1)
    sth r0, 0xc(r1)
    lha r3, 0x6(r31)
    lwz r0, 0x10(r1)
    cmpw r3, r0
    beq @L_8004C9D8
    li r30, 0x1
    lwz r0, 0x14(r1)
    cmpw r3, r0
    beq @L_8004C9D8
    li r30, 0x2
    lwz r0, 0x18(r1)
    cmpw r3, r0
    beq @L_8004C9D8
    li r30, 0x3
    lwz r0, 0x1c(r1)
    cmpw r3, r0
    beq @L_8004C9D8
    li r30, 0x4
    lwz r0, 0x20(r1)
    cmpw r3, r0
    beq @L_8004C9D8
    li r30, 0x5
    lwz r0, 0x24(r1)
    cmpw r3, r0
    beq @L_8004C9D8
    li r30, 0x6
    lwz r0, 0x28(r1)
    cmpw r3, r0
    beq @L_8004C9D8
    li r30, 0x7
    lwz r0, 0x2c(r1)
    cmpw r3, r0
    beq @L_8004C9D8
    li r30, 0x8
    lwz r0, 0x30(r1)
    cmpw r3, r0
    beq @L_8004C9D8
    li r30, 0x9
    lwz r0, 0x34(r1)
    cmpw r3, r0
    beq @L_8004C9D8
    li r30, 0xa
    @L_8004C9D8:
    cmpwi r30, 0xa
    blt @L_8004C9E8
    li r3, 0x0
    b @L_8004CA80
    @L_8004C9E8:
    lbz r0, 0xc(r1)
    extsb r0, r0
    mulli r0, r0, 0xa
    add r30, r30, r0
    mr r3, r30
    bl fn_8004BE40
    bl fn_801D1ACC
    cmplwi r3, 0x0
    beq @L_8004CA2C
    bl fn_800FA280
    mr r0, r3
    li r3, 0x37
    mr r4, r0
    bl fn_80132A38
    li r0, 0xe7
    stw r0, 0x4c(r31)
    b @L_8004CA34
    @L_8004CA2C:
    li r0, 0x0
    stw r0, 0x4c(r31)
    @L_8004CA34:
    mr r3, r30
    bl fn_8004BE40
    cmpwi r3, 0x0
    blt @L_8004CA7C
    bl fn_801D1B78
    cmpwi r3, 0x0
    beq @L_8004CA64
    li r0, 0xff
    stb r0, 0x64(r31)
    stb r0, 0x65(r31)
    stb r0, 0x66(r31)
    b @L_8004CA7C
    @L_8004CA64:
    li r0, 0xd5
    li r3, 0xaa
    stb r0, 0x64(r31)
    li r0, 0x33
    stb r3, 0x65(r31)
    stb r0, 0x66(r31)
    @L_8004CA7C:
    li r3, 0x0
    @L_8004CA80:
    lwz r0, 0x44(r1)
    lwz r31, 0x3c(r1)
    lwz r30, 0x38(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr

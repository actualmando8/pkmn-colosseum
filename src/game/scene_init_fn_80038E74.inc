    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    mr r31, r4
    lis r3, lbl_80267060@ha
    li r5, 0x0
    addi r4, r3, lbl_80267060@l
    lha r3, 0x6(r31)
    lwz r0, 0x0(r4)
    cmpw r3, r0
    beq @L_80038F34
    addi r4, r4, 0x18
    li r5, 0x1
    lwz r0, 0x0(r4)
    cmpw r3, r0
    beq @L_80038F34
    addi r4, r4, 0x18
    li r5, 0x2
    lwz r0, 0x0(r4)
    cmpw r3, r0
    beq @L_80038F34
    addi r4, r4, 0x18
    li r5, 0x3
    lwz r0, 0x0(r4)
    cmpw r3, r0
    beq @L_80038F34
    addi r4, r4, 0x18
    li r5, 0x4
    lwz r0, 0x0(r4)
    cmpw r3, r0
    beq @L_80038F34
    addi r4, r4, 0x18
    li r5, 0x5
    lwz r0, 0x0(r4)
    cmpw r3, r0
    beq @L_80038F34
    addi r4, r4, 0x18
    li r5, 0x6
    lwz r0, 0x0(r4)
    cmpw r3, r0
    beq @L_80038F34
    addi r4, r4, 0x18
    li r5, 0x7
    lwz r0, 0x0(r4)
    cmpw r3, r0
    beq @L_80038F34
    li r5, 0x8
    @L_80038F34:
    cmpwi r5, 0x8
    blt @L_80038F44
    li r3, 0x0
    b @L_80038FF0
    @L_80038F44:
    mulli r0, r5, 0xc
    lis r4, lbl_803A65B0@ha
    lis r3, lbl_80267060@ha
    addi r4, r4, lbl_803A65B0@l
    lfsx f0, r4, r0
    add r4, r4, r0
    mulli r0, r5, 0x18
    addi r3, r3, lbl_80267060@l
    fctiwz f0, f0
    add r3, r3, r0
    stfd f0, 0x8(r1)
    lbz r0, 0x14(r3)
    lwz r3, 0xc(r1)
    cmplwi r0, 0x0
    sth r3, 0x50(r31)
    lfs f0, 0x4(r4)
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    sth r0, 0x52(r31)
    lfs f0, 0x8(r4)
    fctiwz f0, f0
    stfd f0, 0x18(r1)
    lwz r0, 0x1c(r1)
    stb r0, 0x67(r31)
    beq @L_80038FEC
    li r3, 0x24
    bl fn_801022B8
    lha r0, 0x6(r31)
    cmpw r0, r3
    bne @L_80038FD4
    li r0, 0xff
    stb r0, 0x64(r31)
    stb r0, 0x65(r31)
    stb r0, 0x66(r31)
    b @L_80038FEC
    @L_80038FD4:
    li r0, 0x46
    li r3, 0x8f
    stb r0, 0x64(r31)
    li r0, 0xb4
    stb r3, 0x65(r31)
    stb r0, 0x66(r31)
    @L_80038FEC:
    li r3, 0x0
    @L_80038FF0:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    lwz r12, -21428(r13)
    cmplwi r12, 0x0
    beq @L_801E3A74
    mtctr r12
    bctrl
@L_801E3A74:
    lis r3, -32697
    li r30, -0x1
    addi r31, r3, -21408
    lwz r0, 0xa0(r31)
    cmpwi r0, 0x0
    beq @L_801E3F40
    lbz r0, 0xa4(r31)
    cmplwi r0, 0x2
    bne @L_801E3F40
    lwz r0, 0xa8(r31)
    cmpwi r0, 0x0
    bne @L_801E3AB0
    lwz r0, 0xac(r31)
    cmpwi r0, 0x0
    beq @L_801E3AC8
@L_801E3AB0:
    lis r3, -32697
    li r0, 0x5
    addi r3, r3, -21408
    stb r0, 0xa4(r31)
    stb r0, 0xa5(r3)
    b @L_801E3F40
@L_801E3AC8:
    lwz r3, 0xcc(r31)
    li r7, 0x1
    lwz r0, 0xc8(r31)
    li r6, 0x0
    addc r5, r3, r7
    adde r4, r0, r6
    stw r5, 0xcc(r31)
    xor r3, r5, r6
    xor r0, r4, r6
    stw r4, 0xc8(r31)
    or. r0, r3, r0
    bne @L_801E3C48
    lwz r3, 0x88(r31)
    clrlwi. r0, r3, 31
    beq @L_801E3B18
    bl fn_800AA2F0
    cmplwi r3, 0x0
    bne @L_801E3B38
    li r7, 0x1
    b @L_801E3B3C
@L_801E3B18:
    rlwinm. r0, r3, 0, 30, 30
    beq @L_801E3B3C
    bl fn_800AA2F0
    cmplwi r3, 0x1
    bne @L_801E3B38
    li r7, 0x1
    b @L_801E3B3C
    b @L_801E3B3C
@L_801E3B38:
    li r7, 0x0
@L_801E3B3C:
    cmpwi r7, 0x0
    beq @L_801E3C38
    lis r3, -32697
    addi r29, r3, -21408
    lbz r0, 0xa7(r29)
    cmplwi r0, 0x0
    beq @L_801E3C28
    lwz r3, 0xe4(r29)
    lwz r0, 0xe0(r29)
    subf r0, r3, r0
    cmpwi r0, 0x1
    bgt @L_801E3B9C
    li r3, 0x0
    bl fn_801E4EF0
    lis r5, -32697
    lwz r4, 0xe0(r29)
    addi r6, r5, -21408
    mr r30, r3
    lwz r5, 0xd8(r6)
    addi r0, r4, 0x1
    stw r0, 0xe0(r29)
    subi r0, r5, 0x1
    stw r0, 0xd8(r6)
    b @L_801E3E60
@L_801E3B9C:
    lwz r3, -31520(r13)
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    beq @L_801E3BF0
    bl fn_8014FF0C
    clrlwi. r0, r3, 24
    beq @L_801E3BF0
    lwz r3, -31516(r13)
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    beq @L_801E3BE8
    bl fn_8014FF0C
    clrlwi. r0, r3, 24
    beq @L_801E3BDC
    li r0, 0x1
    b @L_801E3BF4
@L_801E3BDC:
    lwz r3, -31520(r13)
    bl fn_80150564
    b @L_801E3BF0
@L_801E3BE8:
    li r0, 0x1
    b @L_801E3BF4
@L_801E3BF0:
    li r0, 0x0
@L_801E3BF4:
    cmpwi r0, 0x0
    bne @L_801E3C14
    lis r3, -32697
    li r0, 0x5
    addi r3, r3, -21408
    stb r0, 0xa4(r31)
    stb r0, 0xa5(r3)
    b @L_801E3F40
@L_801E3C14:
    lis r3, -32697
    li r0, 0x2
    addi r3, r3, -21408
    stb r0, 0xa5(r3)
    b @L_801E3E60
@L_801E3C28:
    li r3, 0x0
    bl fn_801E4EF0
    mr r30, r3
    b @L_801E3E60
@L_801E3C38:
    li r0, -0x1
    stw r0, 0xcc(r31)
    stw r0, 0xc8(r31)
    b @L_801E3E60
@L_801E3C48:
    lbz r0, 0xa7(r31)
    cmplwi r0, 0x0
    beq @L_801E3CE8
    xor r3, r5, r7
    xor r0, r4, r6
    or. r0, r3, r0
    bne @L_801E3CE8
    lbz r0, 0xa5(r31)
    cmplwi r0, 0x2
    beq @L_801E3CE8
    lwz r3, -31520(r13)
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    beq @L_801E3CC4
    bl fn_8014FF0C
    clrlwi. r0, r3, 24
    beq @L_801E3CC4
    lwz r3, -31516(r13)
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    beq @L_801E3CBC
    bl fn_8014FF0C
    clrlwi. r0, r3, 24
    beq @L_801E3CB0
    li r0, 0x1
    b @L_801E3CC8
@L_801E3CB0:
    lwz r3, -31520(r13)
    bl fn_80150564
    b @L_801E3CC4
@L_801E3CBC:
    li r0, 0x1
    b @L_801E3CC8
@L_801E3CC4:
    li r0, 0x0
@L_801E3CC8:
    cmpwi r0, 0x0
    bne @L_801E3CE0
    li r0, 0x5
    stb r0, 0xa5(r31)
    stb r0, 0xa4(r31)
    b @L_801E3F40
@L_801E3CE0:
    li r0, 0x2
    stb r0, 0xa5(r31)
@L_801E3CE8:
    lis r3, -32697
    addi r3, r3, -21408
    lwz r4, 0x88(r3)
    clrlwi. r0, r4, 31
    beq @L_801E3D10
    bl fn_800AA2F0
    cmplwi r3, 0x0
    bne @L_801E3DF0
    li r0, 0x1
    b @L_801E3DF4
@L_801E3D10:
    rlwinm. r0, r4, 0, 30, 30
    beq @L_801E3D2C
    bl fn_800AA2F0
    cmplwi r3, 0x1
    bne @L_801E3DF0
    li r0, 0x1
    b @L_801E3DF4
@L_801E3D2C:
    lfs f1, -20984(r2)
    lfs f0, 0x4c(r3)
    fmuls f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r29, 0xc(r1)
    bl VIGetTvFormat
    cmplwi r3, 0x1
    bne @L_801E3D90
    lwz r8, 0xcc(r31)
    srawi r0, r29, 31
    lwz r4, 0xc8(r31)
    li r5, 0x0
    mulhwu r3, r8, r29
    li r6, 0x1388
    mullw r7, r4, r29
    mullw r0, r8, r0
    add r3, r3, r7
    mullw r4, r8, r29
    add r3, r3, r0
    bl fn_800C4928
    lis r3, -32697
    addi r3, r3, -21408
    stw r4, 0xd4(r3)
    b @L_801E3DCC
@L_801E3D90:
    lwz r8, 0xcc(r31)
    srawi r0, r29, 31
    lwz r4, 0xc8(r31)
    li r5, 0x0
    mulhwu r3, r8, r29
    li r6, 0x176a
    mullw r7, r4, r29
    mullw r0, r8, r0
    add r3, r3, r7
    mullw r4, r8, r29
    add r3, r3, r0
    bl fn_800C4928
    lis r3, -32697
    addi r3, r3, -21408
    stw r4, 0xd4(r3)
@L_801E3DCC:
    lis r3, -32697
    addi r3, r3, -21408
    lwz r0, 0xd0(r3)
    lwz r4, 0xd4(r3)
    cmpw r0, r4
    beq @L_801E3DF0
    stw r4, 0xd0(r3)
    li r0, 0x1
    b @L_801E3DF4
@L_801E3DF0:
    li r0, 0x0
@L_801E3DF4:
    cmpwi r0, 0x0
    beq @L_801E3E60
    lbz r0, 0xa7(r31)
    cmplwi r0, 0x0
    beq @L_801E3E54
    lis r3, -32697
    addi r29, r3, -21408
    lwz r3, 0xe4(r29)
    lwz r0, 0xe0(r29)
    subf r0, r3, r0
    cmpwi r0, 0x1
    bgt @L_801E3E60
    li r3, 0x0
    bl fn_801E4EF0
    lis r5, -32697
    lwz r4, 0xe0(r29)
    addi r6, r5, -21408
    mr r30, r3
    lwz r5, 0xd8(r6)
    addi r0, r4, 0x1
    stw r0, 0xe0(r29)
    subi r0, r5, 0x1
    stw r0, 0xd8(r6)
    b @L_801E3E60
@L_801E3E54:
    li r3, 0x0
    bl fn_801E4EF0
    mr r30, r3
@L_801E3E60:
    cmplwi r30, 0x0
    beq @L_801E3E9C
    addis r0, r30, 0x1
    cmplwi r0, 0xffff
    beq @L_801E3E9C
    lis r3, -32697
    addi r29, r3, -21408
    lwz r4, 0xe8(r29)
    cmplwi r4, 0x0
    beq @L_801E3E98
    lis r3, -32697
    li r5, 0x0
    addi r3, r3, -23404
    bl fn_8009F230
@L_801E3E98:
    stw r30, 0xe8(r29)
@L_801E3E9C:
    lis r3, -32697
    addi r5, r3, -21408
    lbz r0, 0xa6(r5)
    clrlwi. r0, r0, 31
    bne @L_801E3F40
    lbz r0, 0xa7(r5)
    cmplwi r0, 0x0
    beq @L_801E3EF0
    lwz r4, 0xe4(r5)
    lwz r3, 0xc0(r5)
    lwz r0, 0x50(r5)
    add r3, r4, r3
    cmplw r3, r0
    bne @L_801E3F40
    lwz r0, 0xec(r5)
    cmplwi r0, 0x0
    bne @L_801E3F40
    li r0, 0x3
    stb r0, 0xa5(r5)
    stb r0, 0xa4(r31)
    b @L_801E3F40
@L_801E3EF0:
    lwz r3, 0xe8(r5)
    cmplwi r3, 0x0
    beq @L_801E3F0C
    lwz r3, 0xc(r3)
    lwz r0, 0xc0(r5)
    add r5, r3, r0
    b @L_801E3F14
@L_801E3F0C:
    lwz r3, 0xc0(r5)
    subi r5, r3, 0x1
@L_801E3F14:
    lis r3, -32697
    addi r4, r3, -21408
    lwz r3, 0x50(r4)
    subi r0, r3, 0x1
    cmplw r5, r0
    bne @L_801E3F40
    cmplwi r30, 0x0
    bne @L_801E3F40
    li r0, 0x3
    stb r0, 0xa5(r4)
    stb r0, 0xa4(r31)
@L_801E3F40:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

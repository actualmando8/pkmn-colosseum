    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r28, r3
    lwz r31, 0x28(r3)
    cmplwi r31, 0x0
    beq @L_800DFD78
    lwz r30, 0x8(r28)
    bl fn_801BBD84
    lis r4, lbl_803154E4@ha
    li r5, 0x80
    addi r4, r4, lbl_803154E4@l
    mr r29, r3
    stw r5, 0x40(r4)
    lfs f0, 0x34(r28)
    stfs f0, 0x44(r4)
    lwz r0, 0x2c(r28)
    cmpwi r0, 0x1
    beq @L_800DFB20
    bge @L_800DFB38
    cmpwi r0, 0x0
    bge @L_800DFB2C
    b @L_800DFB38
    b @L_800DFB38
    @L_800DFB20:
    ori r0, r5, 0x5
    stw r0, 0x40(r4)
    b @L_800DFB44
    @L_800DFB2C:
    ori r0, r5, 0x1
    stw r0, 0x40(r4)
    b @L_800DFB44
    @L_800DFB38:
    lwz r0, 0x40(r4)
    ori r0, r0, 0x6
    stw r0, 0x40(r4)
    @L_800DFB44:
    lwz r0, 0x30(r28)
    cmpwi r0, 0x0
    bne @L_800DFB60
    lwz r0, 0x40(r4)
    oris r0, r0, 0x4
    stw r0, 0x40(r4)
    b @L_800DFB6C
    @L_800DFB60:
    lwz r0, 0x40(r4)
    oris r0, r0, 0x3
    stw r0, 0x40(r4)
    @L_800DFB6C:
    mr r3, r31
    li r4, 0x0
    bl fn_800EF548
    stw r3, 0x0(r29)
    mr r3, r31
    bl fn_800EF4FC
    sth r3, 0x4(r29)
    mr r3, r31
    bl fn_800EF4F4
    sth r3, 0x6(r29)
    mr r3, r31
    li r4, 0x1
    bl fn_800EF3E0
    stw r3, 0x8(r29)
    lwz r0, 0x8(r29)
    cmpwi r0, 0xe
    beq @L_800DFCE4
    bge @L_800DFBC4
    cmpwi r0, 0x7
    bge @L_800DFBC4
    cmpwi r0, 0x0
    bge @L_800DFCE4
    @L_800DFBC4:
    lis r3, lbl_802705D0@ha
    addi r3, r3, lbl_802705D0@l
    crxor 6, 6, 6
    bl fn_800DD970
    mr r3, r29
    bl fn_801BBD3C
    mr r3, r29
    bl fn_801BBD60
    lhz r0, 0x2(r28)
    lwz r31, 0x8(r28)
    rlwinm r29, r0, 0, 29, 29
    clrlwi. r0, r29, 31
    beq @L_800DFC0C
    li r0, 0x7f
    stb r0, 0xf(r28)
    stb r0, 0xe(r28)
    stb r0, 0xd(r28)
    stb r0, 0xc(r28)
    @L_800DFC0C:
    rlwinm. r0, r29, 0, 30, 30
    beq @L_800DFC34
    li r0, 0x0
    li r4, 0x1
    stw r0, 0x10(r28)
    li r3, 0x2
    li r0, 0x3
    stw r4, 0x14(r28)
    stw r3, 0x18(r28)
    stw r0, 0x1c(r28)
    @L_800DFC34:
    rlwinm. r0, r29, 0, 29, 29
    beq @L_800DFCB8
    lwz r5, 0x8(r28)
    lwz r30, 0x20(r28)
    lwz r4, 0x8(r5)
    lwz r3, 0x24(r28)
    cmplwi r4, 0x0
    beq @L_800DFCB0
    cmplwi r30, 0x0
    beq @L_800DFCB0
    cmplw r4, r30
    bne @L_800DFC8C
    lwz r0, 0x8(r30)
    stw r0, 0x8(r5)
    b @L_800DFC94
    b @L_800DFC8C
    @L_800DFC74:
    lwz r0, 0x8(r4)
    cmplw r0, r30
    bne @L_800DFC88
    lwz r0, 0x8(r30)
    stw r0, 0x8(r4)
    @L_800DFC88:
    lwz r4, 0x8(r4)
    @L_800DFC8C:
    cmplwi r4, 0x0
    bne @L_800DFC74
    @L_800DFC94:
    cmplwi r3, 0x0
    beq @L_800DFCA0
    bl fn_801BBD3C
    @L_800DFCA0:
    cmplwi r30, 0x0
    beq @L_800DFCB0
    mr r3, r30
    bl fn_801BBED4
    @L_800DFCB0:
    li r0, 0x0
    stw r0, 0x28(r28)
    @L_800DFCB8:
    lhz r0, 0x2(r28)
    andc r0, r0, r29
    sth r0, 0x2(r28)
    lhz r0, 0x2(r28)
    cmplwi r0, 0x0
    bne @L_800DFCD8
    li r0, 0x0
    stw r0, 0x20(r31)
    @L_800DFCD8:
    mr r3, r31
    bl fn_801A6FF0
    b @L_800DFE84
    @L_800DFCE4:
    mr r3, r31
    bl fn_800EF4E4
    clrlwi r4, r3, 24
    lis r3, lbl_803154E4@ha
    neg r0, r4
    lfs f0, lbl_8047CAC8(r2)
    or r0, r0, r4
    addi r3, r3, lbl_803154E4@l
    srwi r0, r0, 31
    stw r0, 0xc(r29)
    stfs f0, 0x10(r29)
    stfs f0, 0x14(r29)
    stw r29, 0x4c(r3)
    bl fn_801BE4CC
    mr r0, r3
    mr r3, r30
    mr r27, r0
    bl fn_801A6DC4
    mr. r4, r3
    beq @L_800DFD58
    b @L_800DFD3C
    @L_800DFD38:
    mr r4, r0
    @L_800DFD3C:
    lwz r0, 0x8(r4)
    cmplwi r0, 0x0
    bne @L_800DFD38
    mr r3, r30
    mr r5, r27
    bl fn_801A6D5C
    b @L_800DFD64
    @L_800DFD58:
    mr r3, r30
    mr r4, r27
    bl fn_801A6DA0
    @L_800DFD64:
    stw r27, 0x20(r28)
    mr r3, r31
    stw r29, 0x24(r28)
    bl fn_800EF504
    b @L_800DFE84
    @L_800DFD78:
    lis r3, lbl_80270610@ha
    addi r3, r3, lbl_80270610@l
    crxor 6, 6, 6
    bl fn_800DD970
    lhz r0, 0x2(r28)
    lwz r31, 0x8(r28)
    rlwinm r29, r0, 0, 29, 29
    clrlwi. r0, r29, 31
    beq @L_800DFDB0
    li r0, 0x7f
    stb r0, 0xf(r28)
    stb r0, 0xe(r28)
    stb r0, 0xd(r28)
    stb r0, 0xc(r28)
    @L_800DFDB0:
    rlwinm. r0, r29, 0, 30, 30
    beq @L_800DFDD8
    li r0, 0x0
    li r4, 0x1
    stw r0, 0x10(r28)
    li r3, 0x2
    li r0, 0x3
    stw r4, 0x14(r28)
    stw r3, 0x18(r28)
    stw r0, 0x1c(r28)
    @L_800DFDD8:
    rlwinm. r0, r29, 0, 29, 29
    beq @L_800DFE5C
    lwz r5, 0x8(r28)
    lwz r30, 0x20(r28)
    lwz r4, 0x8(r5)
    lwz r3, 0x24(r28)
    cmplwi r4, 0x0
    beq @L_800DFE54
    cmplwi r30, 0x0
    beq @L_800DFE54
    cmplw r4, r30
    bne @L_800DFE30
    lwz r0, 0x8(r30)
    stw r0, 0x8(r5)
    b @L_800DFE38
    b @L_800DFE30
    @L_800DFE18:
    lwz r0, 0x8(r4)
    cmplw r0, r30
    bne @L_800DFE2C
    lwz r0, 0x8(r30)
    stw r0, 0x8(r4)
    @L_800DFE2C:
    lwz r4, 0x8(r4)
    @L_800DFE30:
    cmplwi r4, 0x0
    bne @L_800DFE18
    @L_800DFE38:
    cmplwi r3, 0x0
    beq @L_800DFE44
    bl fn_801BBD3C
    @L_800DFE44:
    cmplwi r30, 0x0
    beq @L_800DFE54
    mr r3, r30
    bl fn_801BBED4
    @L_800DFE54:
    li r0, 0x0
    stw r0, 0x28(r28)
    @L_800DFE5C:
    lhz r0, 0x2(r28)
    andc r0, r0, r29
    sth r0, 0x2(r28)
    lhz r0, 0x2(r28)
    cmplwi r0, 0x0
    bne @L_800DFE7C
    li r0, 0x0
    stw r0, 0x20(r31)
    @L_800DFE7C:
    mr r3, r31
    bl fn_801A6FF0
    @L_800DFE84:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

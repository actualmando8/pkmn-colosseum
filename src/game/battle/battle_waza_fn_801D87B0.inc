    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r23, 0x2c(r1)
    mr r27, r3
    lwz r4, 0xb0(r3)
    lwz r3, 0x78(r3)
    lwz r30, 0x3c(r4)
    cmpwi r3, 0x0
    lhz r0, 0x32(r30)
    lwz r4, 0x2c(r30)
    mulli r0, r0, 0xd4
    lwz r26, 0x0(r30)
    lwz r25, 0x4(r30)
    add r29, r4, r0
    beq @L_801D8B04
    mr r31, r3
    bl fn_8013151C
    mr. r28, r3
    bne @L_801D881C
    bl fn_800057A8
    cmpwi r3, 0x2
    bne @L_801D8814
    li r3, 0x80
    bl fn_801D744C
@L_801D8814:
    li r3, 0x0
    b @L_801D8B24
@L_801D881C:
    lwz r3, 0xb0(r27)
    addi r4, r1, 0x14
    lwz r5, 0x10(r30)
    bl fn_801D9950
    lwz r3, 0x88(r27)
    addi r0, r3, 0x1
    cmplwi r0, 0xd
    bgt @L_801D8ADC
    lis r3, -32713
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, 9768
    lwzx r0, r3, r0
    mtctr r0
    bctr
    lbz r0, 0x4c(r28)
    cmpwi r0, 0x1
    beq @L_801D8874
    bge @L_801D8868
    b @L_801D8AF8
@L_801D8868:
    cmpwi r0, 0x3
    bge @L_801D8AF8
    b @L_801D8918
@L_801D8874:
    bl fn_80115BD8
    mr r27, r3
    bl fn_80113F48
    li r24, 0x0
    mr r25, r3
    li r23, 0x0
    bl fn_80113F48
    lwz r4, 0x8(r27)
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_801D890C
    lis r4, -32728
    addi r4, r4, -27060
    bl fn_80191ECC
    mr. r29, r3
    beq @L_801D890C
    lwz r0, 0x0(r29)
    cmplwi r0, 0x0
    beq @L_801D890C
    lwz r3, 0x8(r27)
    bl fn_80115274
    mr r27, r3
    mr r30, r28
    li r26, 0x0
    b @L_801D88FC
@L_801D88D8:
    mr r3, r25
    or r4, r27, r24
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_801D88F4
    stwu r3, 0x4(r30)
    addi r23, r23, 0x1
@L_801D88F4:
    addi r26, r26, 0x4
    addi r24, r24, 0x1
@L_801D88FC:
    lwz r3, 0x0(r29)
    lwzx r0, r3, r26
    cmplwi r0, 0x0
    bne @L_801D88D8
@L_801D890C:
    rlwinm r0, r23, 0, 16, 31
    stw r0, 0x48(r28)
    b @L_801D8AF8
@L_801D8918:
    lwz r4, 0x24(r30)
    li r0, 0x1
    mr r3, r30
    stw r4, 0x4(r28)
    stw r0, 0x48(r28)
    bl fn_801DCDCC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801D8964
    lwz r3, 0x24(r30)
    bl fn_800E5978
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801D8964
    li r0, 0x1
    addi r4, r28, 0x44
    stb r0, 0x4d(r28)
    stb r0, 0x4f(r28)
    lwz r3, 0x24(r30)
    bl fn_800E59C8
    b @L_801D8AF8
@L_801D8964:
    li r0, 0x0
    stb r0, 0x4d(r28)
    stb r0, 0x4f(r28)
    b @L_801D8AF8
    sth r26, 0x4c(r28)
    sth r25, 0x4e(r28)
    lwz r0, 0x80(r27)
    rlwinm r0, r0, 2, 0, 29
    add r3, r29, r0
    lwz r0, 0x4c(r3)
    stw r0, 0x50(r28)
    b @L_801D8AF8
    lfs f0, 0x14(r1)
    stfs f0, 0x10(r28)
    sth r26, 0xc(r28)
    sth r25, 0xe(r28)
    b @L_801D8AF8
    addi r3, r28, 0x28
    addi r4, r1, 0x14
    bl fn_800E01D0
    sth r26, 0xa(r28)
    sth r25, 0xc(r28)
    lwz r0, 0x80(r27)
    rlwinm r0, r0, 2, 0, 29
    add r3, r29, r0
    lwz r0, 0x4c(r3)
    sth r0, 0xe(r28)
    b @L_801D8AF8
    sth r26, 0x24(r28)
    sth r25, 0x26(r28)
    lwz r0, 0x80(r27)
    rlwinm r0, r0, 2, 0, 29
    add r3, r29, r0
    lwz r0, 0x4c(r3)
    sth r0, 0x28(r28)
    lwz r0, 0x84(r27)
    rlwinm r0, r0, 2, 0, 29
    add r3, r29, r0
    lwz r0, 0x4c(r3)
    sth r0, 0x2a(r28)
    b @L_801D8AF8
    sth r26, 0x46(r28)
    sth r25, 0x48(r28)
    lwz r0, 0x80(r27)
    rlwinm r0, r0, 2, 0, 29
    add r3, r29, r0
    lwz r0, 0x4c(r3)
    sth r0, 0x4a(r28)
    lfs f0, 0x14(r1)
    stfs f0, 0xc(r28)
    b @L_801D8AF8
    lwz r3, 0x24(r30)
    addi r4, r28, 0x54
    bl fn_800E3D6C
    lwz r0, 0xa8(r28)
    cmpwi r0, 0x0
    beq @L_801D8AF8
    lwz r3, 0x24(r30)
    addi r4, r1, 0x8
    bl fn_800E3D98
    addi r3, r1, 0x8
    addi r5, r28, 0x48
    mr r4, r3
    bl fn_800E019C
    lwz r3, 0x24(r30)
    addi r4, r1, 0x8
    bl fn_800E43A4
    b @L_801D8AF8
    lwz r3, 0x24(r30)
    addi r4, r28, 0x30
    bl fn_800E3D6C
    b @L_801D8AF8
    lwz r0, 0x24(r30)
    stw r0, 0x0(r28)
    b @L_801D8AF8
    lwz r0, 0x10(r28)
    cmpwi r0, 0x0
    bne @L_801D8AF8
    lwz r0, 0x24(r30)
    stw r0, 0x0(r28)
    b @L_801D8AF8
    lwz r0, 0x24(r30)
    stw r0, 0x0(r28)
    b @L_801D8AF8
    lwz r0, 0x24(r30)
    stw r0, 0x8(r28)
    lwz r3, 0x2c(r30)
    lwz r0, 0x54(r3)
    stw r0, 0xc(r28)
    lfs f1, 0x18(r28)
    lfs f0, 0x14(r1)
    fmuls f0, f1, f0
    stfs f0, 0x14(r28)
    b @L_801D8AF8
@L_801D8ADC:
    bl fn_800057A8
    cmpwi r3, 0x2
    bne @L_801D8AF0
    li r3, 0x80
    bl fn_801D744C
@L_801D8AF0:
    li r3, 0x0
    b @L_801D8B24
@L_801D8AF8:
    mr r3, r31
    bl fn_8013111C
    b @L_801D8B20
@L_801D8B04:
    bl fn_800057A8
    cmpwi r3, 0x2
    bne @L_801D8B18
    li r3, 0x80
    bl fn_801D744C
@L_801D8B18:
    li r3, 0x0
    b @L_801D8B24
@L_801D8B20:
    li r3, 0x1
@L_801D8B24:
    lmw r23, 0x2c(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr

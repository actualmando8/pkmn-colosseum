    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stmw r27, 0x4c(r1)
    mr r28, r3
    lwz r0, 0x28(r3)
    lwz r3, 0xb0(r3)
    cmplwi r0, 0x7
    lwz r29, 0x88(r28)
    lwz r30, 0x3c(r3)
    bgt @L_801D8BB4
    lis r4, -32713
    rlwinm r0, r0, 2, 0, 29
    addi r4, r4, 9856
    lwzx r0, r4, r0
    mtctr r0
    bctr
    li r31, 0x1
    b @L_801D8BB8
    li r31, 0x2
    b @L_801D8BB8
    li r31, 0x3
    b @L_801D8BB8
    li r31, 0x4
    b @L_801D8BB8
    li r31, 0x5
    b @L_801D8BB8
    li r31, 0x6
    b @L_801D8BB8
    li r31, 0x7
    b @L_801D8BB8
@L_801D8BB4:
    li r31, 0x0
@L_801D8BB8:
    lwz r0, 0x1c(r28)
    lwz r27, 0x24(r30)
    rlwinm r0, r0, 0, 31, 31
    cmpwi r0, 0x1
    bne @L_801D8C0C
    lwz r4, 0x8(r28)
    cmpwi r4, 0x0
    ble @L_801D8C0C
    bl fn_801DCDA8
    lwz r4, 0x70(r3)
    lwz r0, 0x70(r28)
    cmpw r4, r0
    bgt @L_801D8C0C
    lwz r0, 0x4(r3)
    cmpwi r0, 0x2
    bne @L_801D8C0C
    lwz r3, 0xa4(r3)
    cmplwi r3, 0x0
    beq @L_801D8C0C
    mr r27, r3
    bl fn_800EC0E8
@L_801D8C0C:
    mr r3, r27
    addi r4, r1, 0x38
    bl fn_800E3D98
    mr r3, r27
    addi r4, r1, 0x2c
    bl fn_800E3D6C
    lwz r3, 0xb0(r28)
    addi r4, r1, 0x20
    lwz r5, 0x10(r30)
    bl fn_801D9950
    cmplwi r29, 0x0
    bne @L_801D8C58
    bl fn_800057A8
    cmpwi r3, 0x2
    bne @L_801D8C50
    li r3, 0x20
    bl fn_801D744C
@L_801D8C50:
    li r3, 0x0
    b @L_801D91D8
@L_801D8C58:
    lwz r0, 0x84(r28)
    lwz r3, 0x88(r28)
    lwz r4, 0x80(r28)
    rlwinm r5, r0, 0, 31, 31
    bl fn_801190DC
    stw r3, 0x8c(r28)
    li r0, 0x0
    stw r0, 0x90(r28)
    lwz r3, 0x8c(r28)
    cmplwi r3, 0x0
    bne @L_801D8CA0
    bl fn_800057A8
    cmpwi r3, 0x2
    bne @L_801D8C98
    li r3, 0x20
    bl fn_801D744C
@L_801D8C98:
    li r3, 0x0
    b @L_801D91D8
@L_801D8CA0:
    lwz r4, 0x84(r28)
    rlwinm. r0, r4, 0, 29, 29
    beq @L_801D8CB8
    rlwinm r5, r4, 28, 31, 31
    li r4, 0x1
    bl fn_80118D3C
@L_801D8CB8:
    lwz r0, 0x84(r28)
    rlwinm. r0, r0, 0, 28, 28
    beq @L_801D8CD0
    lwz r3, 0x8c(r28)
    li r4, 0x1
    bl fn_80118D60
@L_801D8CD0:
    lwz r0, 0x84(r28)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_801D8CE8
    lwz r3, 0x8c(r28)
    li r4, 0x1
    bl fn_80118D18
@L_801D8CE8:
    lwz r0, 0x84(r28)
    rlwinm. r0, r0, 0, 20, 20
    beq @L_801D8D00
    lwz r3, 0x8c(r28)
    li r4, 0x1
    bl fn_80118CD0
@L_801D8D00:
    lwz r0, 0x84(r28)
    rlwinm. r0, r0, 0, 19, 19
    beq @L_801D8D18
    lwz r3, 0x8c(r28)
    li r4, 0x1
    bl fn_80118CAC
@L_801D8D18:
    lwz r5, 0x84(r28)
    lwz r3, 0x8c(r28)
    rlwinm r6, r5, 27, 31, 31
    rlwinm r0, r5, 23, 31, 31
    rlwinm r4, r5, 24, 31, 31
    rlwinm r5, r5, 22, 31, 31
    mr r29, r6
    mr r30, r0
    bl fn_80118CF4
    lwz r3, 0x8c(r28)
    addi r4, r1, 0x8
    bl fn_80118F7C
    li r0, 0x0
    stw r0, 0x90(r28)
    lwz r0, 0x1c(r28)
    rlwinm r0, r0, 0, 29, 29
    cmpwi r0, 0x4
    bne @L_801D8DE0
    addi r3, r1, 0x8
    bl fn_800E0204
    lwz r3, 0x8c(r28)
    addi r4, r1, 0x8
    bl fn_80118F04
    lwz r3, 0x8c(r28)
    addi r4, r1, 0x8
    bl fn_80118E8C
    lwz r0, 0x1c(r28)
    rlwinm r0, r0, 0, 28, 28
    cmpwi r0, 0x8
    bne @L_801D8DB0
    addi r3, r1, 0x20
    bl fn_801C3B80
    lwz r3, 0x8c(r28)
    mr r5, r29
    mr r6, r30
    addi r4, r1, 0x20
    bl fn_80118DE0
    b @L_801D8DD8
@L_801D8DB0:
    lfs f1, -21332(r2)
    addi r3, r1, 0x20
    fmr f2, f1
    fmr f3, f1
    bl fn_800E01F4
    lwz r3, 0x8c(r28)
    mr r5, r29
    mr r6, r30
    addi r4, r1, 0x20
    bl fn_80118DE0
@L_801D8DD8:
    li r3, 0x1
    b @L_801D91D8
@L_801D8DE0:
    mr r3, r28
    bl fn_801D97F0
    mr. r27, r3
    beq @L_801D91D4
    cmpwi r31, 0x0
    beq @L_801D9150
    lwz r3, 0x1c(r28)
    mr r4, r27
    lwz r0, 0x84(r28)
    mr r5, r31
    rlwinm r6, r3, 31, 31, 31
    lwz r3, 0x8c(r28)
    rlwinm r8, r0, 31, 31, 31
    li r7, 0x1
    bl fn_80118FB0
    li r0, 0x1
    mr r3, r27
    stw r0, 0x90(r28)
    addi r6, r1, 0x14
    li r4, 0x0
    li r5, 0x0
    bl fn_800EE3BC
    addi r3, r1, 0x14
    addi r5, r1, 0x20
    mr r4, r3
    bl fn_800E0108
    cmplwi r31, 0x7
    bgt @L_801D9128
    lis r3, -32713
    rlwinm r0, r31, 2, 0, 29
    addi r3, r3, 9824
    lwzx r0, r3, r0
    mtctr r0
    bctr
    lfs f2, 0x14(r1)
    addi r4, r1, 0x8
    lfs f5, -21332(r2)
    lfs f1, 0x18(r1)
    lfs f0, 0x1c(r1)
    fsubs f4, f2, f5
    lfs f6, 0x8(r1)
    fsubs f2, f1, f5
    lfs f3, 0xc(r1)
    fsubs f0, f0, f5
    lfs f1, 0x10(r1)
    fmuls f4, f6, f4
    fmuls f2, f3, f2
    fmuls f0, f1, f0
    stfs f4, 0x8(r1)
    stfs f2, 0xc(r1)
    stfs f0, 0x10(r1)
    lwz r3, 0x8c(r28)
    bl fn_80118F04
    b @L_801D91CC
    lfs f2, 0x20(r1)
    addi r4, r1, 0x8
    lfs f5, -21332(r2)
    lfs f1, 0x24(r1)
    lfs f0, 0x28(r1)
    fsubs f4, f2, f5
    lfs f6, 0x8(r1)
    fsubs f2, f1, f5
    lfs f3, 0xc(r1)
    fsubs f0, f0, f5
    lfs f1, 0x10(r1)
    fmuls f4, f6, f4
    fmuls f2, f3, f2
    fmuls f0, f1, f0
    stfs f4, 0x8(r1)
    stfs f2, 0xc(r1)
    stfs f0, 0x10(r1)
    lwz r3, 0x8c(r28)
    bl fn_80118F04
    lwz r3, 0x8c(r28)
    mr r5, r29
    mr r6, r30
    addi r4, r1, 0x20
    bl fn_80118DE0
    b @L_801D91CC
    lfs f2, 0x20(r1)
    addi r4, r1, 0x8
    lfs f5, -21332(r2)
    lfs f1, 0x24(r1)
    lfs f0, 0x28(r1)
    fsubs f4, f2, f5
    lfs f6, 0x8(r1)
    fsubs f2, f1, f5
    lfs f3, 0xc(r1)
    fsubs f0, f0, f5
    lfs f1, 0x10(r1)
    fmuls f4, f6, f4
    fmuls f2, f3, f2
    fmuls f0, f1, f0
    stfs f4, 0x8(r1)
    stfs f2, 0xc(r1)
    stfs f0, 0x10(r1)
    lwz r3, 0x8c(r28)
    bl fn_80118F04
    lwz r3, 0x8c(r28)
    mr r5, r29
    mr r6, r30
    addi r4, r1, 0x20
    bl fn_80118DE0
    lwz r3, 0x8c(r28)
    addi r4, r1, 0x2c
    bl fn_80118E8C
    b @L_801D91CC
    lfs f2, 0x14(r1)
    addi r3, r1, 0x8
    lfs f5, -21332(r2)
    mr r4, r3
    lfs f1, 0x18(r1)
    addi r5, r1, 0x38
    lfs f0, 0x1c(r1)
    fsubs f4, f2, f5
    lfs f6, 0x8(r1)
    fsubs f2, f1, f5
    lfs f3, 0xc(r1)
    fsubs f0, f0, f5
    lfs f1, 0x10(r1)
    fmuls f4, f6, f4
    fmuls f2, f3, f2
    fmuls f0, f1, f0
    stfs f4, 0x8(r1)
    stfs f2, 0xc(r1)
    stfs f0, 0x10(r1)
    bl fn_800E019C
    lwz r3, 0x8c(r28)
    addi r4, r1, 0x8
    bl fn_80118F04
    b @L_801D91CC
    lfs f2, 0x20(r1)
    addi r3, r1, 0x8
    lfs f5, -21332(r2)
    mr r4, r3
    lfs f1, 0x24(r1)
    addi r5, r1, 0x38
    lfs f0, 0x28(r1)
    fsubs f4, f2, f5
    lfs f6, 0x8(r1)
    fsubs f2, f1, f5
    lfs f3, 0xc(r1)
    fsubs f0, f0, f5
    lfs f1, 0x10(r1)
    fmuls f4, f6, f4
    fmuls f2, f3, f2
    fmuls f0, f1, f0
    stfs f4, 0x8(r1)
    stfs f2, 0xc(r1)
    stfs f0, 0x10(r1)
    bl fn_800E019C
    lwz r3, 0x8c(r28)
    addi r4, r1, 0x8
    bl fn_80118F04
    lwz r3, 0x8c(r28)
    mr r5, r29
    mr r6, r30
    addi r4, r1, 0x20
    bl fn_80118DE0
    b @L_801D91CC
    lfs f2, 0x14(r1)
    addi r4, r1, 0x8
    lfs f5, -21332(r2)
    lfs f1, 0x18(r1)
    lfs f0, 0x1c(r1)
    fsubs f4, f2, f5
    lfs f6, 0x8(r1)
    fsubs f2, f1, f5
    lfs f3, 0xc(r1)
    fsubs f0, f0, f5
    lfs f1, 0x10(r1)
    fmuls f4, f6, f4
    fmuls f2, f3, f2
    fmuls f0, f1, f0
    stfs f4, 0x8(r1)
    stfs f2, 0xc(r1)
    stfs f0, 0x10(r1)
    lwz r3, 0x8c(r28)
    bl fn_80118F04
    lwz r3, 0x8c(r28)
    addi r4, r1, 0x2c
    bl fn_80118E8C
    b @L_801D91CC
    lfs f2, 0x14(r1)
    addi r3, r1, 0x8
    lfs f5, -21332(r2)
    mr r4, r3
    lfs f1, 0x18(r1)
    addi r5, r1, 0x38
    lfs f0, 0x1c(r1)
    fsubs f4, f2, f5
    lfs f6, 0x8(r1)
    fsubs f2, f1, f5
    lfs f3, 0xc(r1)
    fsubs f0, f0, f5
    lfs f1, 0x10(r1)
    fmuls f4, f6, f4
    fmuls f2, f3, f2
    fmuls f0, f1, f0
    stfs f4, 0x8(r1)
    stfs f2, 0xc(r1)
    stfs f0, 0x10(r1)
    bl fn_800E019C
    lwz r3, 0x8c(r28)
    addi r4, r1, 0x8
    bl fn_80118F04
    lwz r3, 0x8c(r28)
    addi r4, r1, 0x2c
    bl fn_80118E8C
    b @L_801D91CC
@L_801D9128:
    bl fn_800057A8
    cmpwi r3, 0x2
    bne @L_801D913C
    li r3, 0x1
    bl fn_801D744C
@L_801D913C:
    lis r3, -32728
    addi r3, r3, -27048
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_801D91CC
@L_801D9150:
    lfs f2, 0x20(r1)
    addi r3, r1, 0x8
    lfs f5, -21332(r2)
    mr r5, r3
    lfs f1, 0x24(r1)
    addi r4, r1, 0x38
    lfs f0, 0x28(r1)
    fsubs f4, f2, f5
    lfs f6, 0x8(r1)
    fsubs f2, f1, f5
    lfs f3, 0xc(r1)
    fsubs f0, f0, f5
    lfs f1, 0x10(r1)
    fmuls f4, f6, f4
    fmuls f2, f3, f2
    fmuls f0, f1, f0
    stfs f4, 0x8(r1)
    stfs f2, 0xc(r1)
    stfs f0, 0x10(r1)
    bl fn_800E019C
    lwz r3, 0x8c(r28)
    addi r4, r1, 0x8
    bl fn_80118F04
    lwz r3, 0x8c(r28)
    addi r4, r1, 0x2c
    bl fn_80118E8C
    lwz r3, 0x8c(r28)
    mr r5, r29
    mr r6, r30
    addi r4, r1, 0x20
    bl fn_80118DE0
@L_801D91CC:
    mr r3, r27
    bl fn_800EE828
@L_801D91D4:
    li r3, 0x1
@L_801D91D8:
    lmw r27, 0x4c(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr

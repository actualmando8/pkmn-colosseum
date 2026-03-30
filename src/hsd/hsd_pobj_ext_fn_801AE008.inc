    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r24, 0x30(r1)
    mr r24, r3
    mr r25, r4
    mr r26, r5
    mr r27, r6
    lwz r3, 0x8(r3)
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    bne @L_801AE060
    lwz r29, 0x4(r24)
    li r28, 0x0
    b @L_801AE054
@L_801AE044:
    lwz r3, 0x4(r29)
    bl fn_801BF138
    lwz r29, 0x0(r29)
    add r28, r28, r3
@L_801AE054:
    cmplwi r29, 0x0
    bne @L_801AE044
    stw r28, 0x8(r24)
@L_801AE060:
    lwz r0, -21784(r13)
    cmplwi r0, 0x0
    bne @L_801AE090
    lwz r0, -21780(r13)
    cmplwi r0, 0x0
    bne @L_801AE080
    li r0, 0x64
    stw r0, -21780(r13)
@L_801AE080:
    lwz r0, -21780(r13)
    rlwinm r3, r0, 2, 0, 29
    bl fn_801A6928
    stw r3, -21784(r13)
@L_801AE090:
    lwz r3, -21780(r13)
    lwz r0, 0x8(r24)
    cmplw r3, r0
    bge @L_801AE0C8
    lis r3, -32729
    lwz r4, 0x8(r24)
    lwz r5, -21780(r13)
    addi r3, r3, 20984
    crxor 6, 6, 6
    bl OSReport
    addi r3, r2, -22864
    li r4, 0x41b
    addi r5, r2, -22844
    bl fn_80196D78
@L_801AE0C8:
    lwz r30, -21784(r13)
    lwz r31, 0x4(r24)
    b @L_801AE424
@L_801AE0D4:
    lwz r28, 0x8(r31)
    cmplwi r28, 0x0
    bne @L_801AE0F0
    addi r3, r2, -22864
    li r4, 0x424
    addi r5, r2, -22840
    bl fn_80196E10
@L_801AE0F0:
    lwz r29, 0x8(r31)
    cmplwi r29, 0x0
    beq @L_801AE144
    cmplwi r29, 0x0
    bne @L_801AE114
    addi r3, r2, -22832
    li r4, 0x25d
    addi r5, r2, -22840
    bl fn_80196E10
@L_801AE114:
    lwz r0, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801AE134
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801AE134
    li r3, 0x1
@L_801AE134:
    cmpwi r3, 0x0
    beq @L_801AE144
    mr r3, r29
    bl fn_8019D9DC
@L_801AE144:
    li r29, 0x1
    b @L_801AE40C
@L_801AE14C:
    lwz r0, 0x4(r31)
    and r3, r0, r29
    cmpwi r3, 0x400
    beq @L_801AE408
    bge @L_801AE1D8
    cmpwi r3, 0x20
    beq @L_801AE2CC
    bge @L_801AE1A8
    cmpwi r3, 0x4
    beq @L_801AE2A4
    bge @L_801AE190
    cmpwi r3, 0x2
    beq @L_801AE28C
    bge @L_801AE408
    cmpwi r3, 0x1
    bge @L_801AE274
    b @L_801AE408
@L_801AE190:
    cmpwi r3, 0x10
    beq @L_801AE2BC
    bge @L_801AE408
    cmpwi r3, 0x8
    beq @L_801AE408
    b @L_801AE408
@L_801AE1A8:
    cmpwi r3, 0x100
    beq @L_801AE2FC
    bge @L_801AE1CC
    cmpwi r3, 0x80
    beq @L_801AE2EC
    bge @L_801AE408
    cmpwi r3, 0x40
    beq @L_801AE2DC
    b @L_801AE408
@L_801AE1CC:
    cmpwi r3, 0x200
    beq @L_801AE30C
    b @L_801AE408
@L_801AE1D8:
    lis r0, 0x20
    cmpw r3, r0
    beq @L_801AE398
    bge @L_801AE234
    lis r0, 0x2
    cmpw r3, r0
    beq @L_801AE340
    bge @L_801AE214
    lis r0, 0x1
    cmpw r3, r0
    beq @L_801AE31C
    bge @L_801AE408
    cmpwi r3, 0x800
    beq @L_801AE408
    b @L_801AE408
@L_801AE214:
    lis r0, 0x10
    cmpw r3, r0
    beq @L_801AE388
    bge @L_801AE408
    lis r0, 0x4
    cmpw r3, r0
    beq @L_801AE364
    b @L_801AE408
@L_801AE234:
    lis r0, 0x100
    cmpw r3, r0
    beq @L_801AE3D4
    bge @L_801AE264
    lis r0, 0x80
    cmpw r3, r0
    beq @L_801AE3B8
    bge @L_801AE408
    lis r0, 0x40
    cmpw r3, r0
    beq @L_801AE3A8
    b @L_801AE408
@L_801AE264:
    lis r0, 0x200
    cmpw r3, r0
    beq @L_801AE3F0
    b @L_801AE408
@L_801AE274:
    lfs f1, -22824(r2)
    lfs f0, 0x1c(r28)
    fmuls f0, f1, f0
    stfs f0, 0x0(r30)
    addi r30, r30, 0x4
    b @L_801AE408
@L_801AE28C:
    lfs f1, -22824(r2)
    lfs f0, 0x20(r28)
    fmuls f0, f1, f0
    stfs f0, 0x0(r30)
    addi r30, r30, 0x4
    b @L_801AE408
@L_801AE2A4:
    lfs f1, -22824(r2)
    lfs f0, 0x24(r28)
    fmuls f0, f1, f0
    stfs f0, 0x0(r30)
    addi r30, r30, 0x4
    b @L_801AE408
@L_801AE2BC:
    lfs f0, 0x38(r28)
    stfs f0, 0x0(r30)
    addi r30, r30, 0x4
    b @L_801AE408
@L_801AE2CC:
    lfs f0, 0x3c(r28)
    stfs f0, 0x0(r30)
    addi r30, r30, 0x4
    b @L_801AE408
@L_801AE2DC:
    lfs f0, 0x40(r28)
    stfs f0, 0x0(r30)
    addi r30, r30, 0x4
    b @L_801AE408
@L_801AE2EC:
    lfs f0, 0x2c(r28)
    stfs f0, 0x0(r30)
    addi r30, r30, 0x4
    b @L_801AE408
@L_801AE2FC:
    lfs f0, 0x30(r28)
    stfs f0, 0x0(r30)
    addi r30, r30, 0x4
    b @L_801AE408
@L_801AE30C:
    lfs f0, 0x34(r28)
    stfs f0, 0x0(r30)
    addi r30, r30, 0x4
    b @L_801AE408
@L_801AE31C:
    addi r3, r28, 0x44
    addi r4, r1, 0x14
    bl fn_801A98CC
    lfs f1, -22824(r2)
    lfs f0, 0x14(r1)
    fmuls f0, f1, f0
    stfs f0, 0x0(r30)
    addi r30, r30, 0x4
    b @L_801AE408
@L_801AE340:
    addi r3, r28, 0x44
    addi r4, r1, 0x14
    bl fn_801A98CC
    lfs f1, -22824(r2)
    lfs f0, 0x18(r1)
    fmuls f0, f1, f0
    stfs f0, 0x0(r30)
    addi r30, r30, 0x4
    b @L_801AE408
@L_801AE364:
    addi r3, r28, 0x44
    addi r4, r1, 0x14
    bl fn_801A98CC
    lfs f1, -22824(r2)
    lfs f0, 0x1c(r1)
    fmuls f0, f1, f0
    stfs f0, 0x0(r30)
    addi r30, r30, 0x4
    b @L_801AE408
@L_801AE388:
    lfs f0, 0x50(r28)
    stfs f0, 0x0(r30)
    addi r30, r30, 0x4
    b @L_801AE408
@L_801AE398:
    lfs f0, 0x60(r28)
    stfs f0, 0x0(r30)
    addi r30, r30, 0x4
    b @L_801AE408
@L_801AE3A8:
    lfs f0, 0x70(r28)
    stfs f0, 0x0(r30)
    addi r30, r30, 0x4
    b @L_801AE408
@L_801AE3B8:
    addi r3, r28, 0x44
    addi r4, r1, 0x24
    bl fn_801A8D1C
    lfs f0, 0x24(r1)
    stfs f0, 0x0(r30)
    addi r30, r30, 0x4
    b @L_801AE408
@L_801AE3D4:
    addi r3, r28, 0x44
    addi r4, r1, 0x24
    bl fn_801A8D1C
    lfs f0, 0x28(r1)
    stfs f0, 0x0(r30)
    addi r30, r30, 0x4
    b @L_801AE408
@L_801AE3F0:
    addi r3, r28, 0x44
    addi r4, r1, 0x24
    bl fn_801A8D1C
    lfs f0, 0x2c(r1)
    stfs f0, 0x0(r30)
    addi r30, r30, 0x4
@L_801AE408:
    rlwinm r29, r29, 1, 0, 30
@L_801AE40C:
    cmpwi r29, 0x0
    beq @L_801AE420
    lwz r0, 0x4(r31)
    cmplw r29, r0
    ble @L_801AE14C
@L_801AE420:
    lwz r31, 0x0(r31)
@L_801AE424:
    cmplwi r31, 0x0
    bne @L_801AE0D4
    lbz r0, 0xc(r24)
    cmplwi r0, 0x0
    beq @L_801AE44C
    lwz r3, 0x0(r24)
    lwz r4, -21784(r13)
    lwz r5, 0x8(r24)
    bl fn_801920E4
    b @L_801AE45C
@L_801AE44C:
    lwz r12, 0x0(r24)
    lwz r3, -21784(r13)
    mtctr r12
    bctrl
@L_801AE45C:
    subi r0, r25, 0x1
    stfs f1, 0x8(r1)
    cmplwi r0, 0x1
    ble @L_801AE474
    cmplwi r25, 0x3
    bne @L_801AE484
@L_801AE474:
    lfs f1, -22820(r2)
    lfs f0, 0x8(r1)
    fmuls f0, f1, f0
    stfs f0, 0x8(r1)
@L_801AE484:
    mr r12, r27
    mr r3, r26
    mr r4, r25
    addi r5, r1, 0x8
    mtctr r12
    bctrl
    lmw r24, 0x30(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr

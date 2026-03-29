    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    stw r28, 0x20(r1)
    lis r3, 0xc6a
    li r4, 0x1
    addi r3, r3, 0x1000
    bl fn_801CB954
    lis r3, 0xc6a
    li r4, 0x1
    addi r3, r3, 0x1001
    bl fn_801CB954
    lis r3, 0xc6a
    li r4, 0x1
    addi r3, r3, 0x1002
    bl fn_801CB954
    li r3, 0x3e5
    bl fn_801902E0
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80026004
    lis r3, 0xc6b
    addi r3, r3, 0x1000
    bl fn_801CBA0C
    li r4, 0x0
    mr r28, r3
    bl fn_801CB954
    li r29, 0x0
    b @L_80026020
    @L_80026004:
    lis r3, 0xc6c
    addi r3, r3, 0x1000
    bl fn_801CBA0C
    li r4, 0x0
    mr r28, r3
    bl fn_801CB954
    li r29, 0x0
    @L_80026020:
    bl fn_80113F48
    mr r4, r28
    bl fn_800F9318
    li r4, 0x0
    bl fn_800E3C94
    bl fn_80113F48
    mr r4, r28
    bl fn_800F9318
    mr r30, r3
    bl fn_80113F48
    lis r4, 0xc6a
    addi r4, r4, 0x1002
    bl fn_800F9318
    mr r31, r3
    bl fn_80113F48
    lis r4, 0xc6a
    addi r4, r4, 0x1603
    bl fn_800F9318
    stw r31, 0x8(r1)
    mr r31, r3
    mr r3, r30
    li r4, 0x2
    bl fn_800E9108
    mr r3, r30
    mr r4, r31
    bl fn_800E8FE8
    mr r3, r30
    addi r5, r1, 0x8
    li r4, 0x1
    bl fn_800E900C
    li r3, 0x280
    li r4, 0x1e0
    bl fn_800E8FA0
    bl fn_80113F48
    lis r4, 0xc83
    addi r4, r4, 0x1400
    bl fn_800F9318
    mr r31, r3
    bl fn_80113F48
    mr r4, r28
    bl fn_800F9318
    mr r4, r31
    bl fn_800E3C08
    bl fn_80113F48
    mr r4, r28
    bl fn_800F9318
    li r4, 0x4
    bl fn_800E3C00
    mr r3, r28
    li r4, 0x1
    bl fn_801CB954
    lis r4, 0xc6a
    mr r3, r28
    addi r4, r4, 0x1000
    li r5, 0x0
    bl fn_801CB61C
    mr r3, r28
    mr r4, r29
    li r5, 0x0
    li r6, 0x1
    bl fn_801CB834
    li r3, 0x449
    li r4, 0x0
    li r5, 0xff
    bl fn_80165A20
    lbz r0, lbl_8047A380(r13)
    stw r28, lbl_8047A384(r13)
    cmplwi r0, 0x0
    bne @L_80026168
    lwz r3, lbl_8047A394(r13)
    lwz r5, lbl_80478DEC(r13)
    slwi r4, r3, 4
    addi r0, r3, 0x1
    add r3, r5, r4
    lwz r29, 0xa4(r3)
    cmpwi r0, 0x9
    lwz r28, 0xa8(r3)
    stw r0, lbl_8047A394(r13)
    ble @L_80026198
    li r0, 0x0
    stw r0, lbl_8047A394(r13)
    b @L_80026198
    @L_80026168:
    lwz r3, lbl_8047A398(r13)
    lwz r5, lbl_80478DEC(r13)
    slwi r4, r3, 4
    addi r0, r3, 0x1
    add r3, r5, r4
    lwz r29, 0x4(r3)
    cmpwi r0, 0x9
    lwz r28, 0x8(r3)
    stw r0, lbl_8047A398(r13)
    ble @L_80026198
    li r0, 0x0
    stw r0, lbl_8047A398(r13)
    @L_80026198:
    lfs f0, lbl_8047B8A8(r2)
    li r3, 0x0
    li r4, 0x0
    li r5, 0x44
    stfs f0, lbl_8047A3A4(r13)
    li r6, 0x0
    li r7, 0x0
    stfs f0, lbl_8047A3A0(r13)
    bl fn_800EF5FC
    lis r4, fn_80025A80@ha
    stw r3, lbl_8047A38C(r13)
    addi r4, r4, fn_80025A80@l
    li r5, 0x0
    bl fn_800DC390
    bl fn_80113F48
    mr r4, r28
    li r5, 0x0
    li r6, 0x0
    bl fn_80176E0C
    li r30, 0x1
    bl fn_800D37CC
    cmpwi r3, 0x32
    bne @L_8002620C
    lfs f1, lbl_8047B914(r2)
    bl fn_800C46B0
    mr r30, r3
    cmplwi r30, 0x1
    bge @L_8002620C
    li r30, 0x1
    @L_8002620C:
    li r28, 0x0
    b @L_80026220
    @L_80026214:
    bl fn_800F0308
    bl fn_800D3088
    add r28, r28, r3
    @L_80026220:
    cmplw r28, r30
    blt @L_80026214
    lis r3, 0xc6a
    mr r4, r29
    addi r3, r3, 0x1000
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    li r29, 0xae
    bl fn_800D37CC
    cmpwi r3, 0x32
    bne @L_80026268
    lfs f1, lbl_8047B918(r2)
    bl fn_800C46B0
    mr r29, r3
    cmplwi r29, 0x1
    bge @L_80026268
    li r29, 0x1
    @L_80026268:
    li r28, 0x0
    b @L_8002627C
    @L_80026270:
    bl fn_800F0308
    bl fn_800D3088
    add r28, r28, r3
    @L_8002627C:
    cmplw r28, r29
    blt @L_80026270
    lis r3, lbl_803A2040@ha
    lfs f2, lbl_8047B8F0(r2)
    addi r3, r3, lbl_803A2040@l
    lfs f1, lbl_8047B8F4(r2)
    lfs f0, lbl_8047B8AC(r2)
    stfs f2, 0x0(r3)
    stfs f1, 0x4(r3)
    stfs f0, 0x8(r3)
    bl fn_80113F48
    lwz r4, lbl_8047A384(r13)
    bl fn_800F9318
    lfs f0, lbl_8047B8A8(r2)
    stw r3, lbl_8047A39C(r13)
    stfs f0, lbl_8047A3A4(r13)
    bl fn_800D37CC
    xoris r3, r3, 0x8000
    lis r0, 0x4330
    stw r3, 0x14(r1)
    lfd f2, lbl_8047B8B8(r2)
    stw r0, 0x10(r1)
    lfs f0, lbl_8047B8E4(r2)
    lfd f1, 0x10(r1)
    fsubs f1, f1, f2
    fmuls f1, f0, f1
    bl fn_800C46B0
    subfic r29, r3, 0x78
    cmplwi r29, 0x0
    beq @L_80026334
    bl fn_800D37CC
    cmpwi r3, 0x32
    bne @L_80026334
    lis r0, 0x4330
    stw r29, 0x14(r1)
    lfd f2, lbl_8047B8D0(r2)
    stw r0, 0x10(r1)
    lfs f0, lbl_8047B900(r2)
    lfd f1, 0x10(r1)
    fsubs f1, f1, f2
    fdivs f1, f1, f0
    bl fn_800C46B0
    mr r29, r3
    cmplwi r29, 0x1
    bge @L_80026334
    li r29, 0x1
    @L_80026334:
    li r28, 0x0
    b @L_80026348
    @L_8002633C:
    bl fn_800F0308
    bl fn_800D3088
    add r28, r28, r3
    @L_80026348:
    cmplw r28, r29
    blt @L_8002633C
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    lwz r28, 0x20(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    clrlwi r7, r5, 16
    lis r3, lbl_803A6818@ha
    rlwinm r0, r7, 0, 30, 30
    li r5, 0x2
    addi r6, r3, lbl_803A6818@l
    cmpwi r0, 0x0
    stw r5, 0x1c(r6)
    beq @L_800437BC
    lhz r4, lbl_8047A4E8(r13)
    cmplwi r4, 0x0
    beq @L_800439A8
    lwz r3, 0x0(r6)
    subi r0, r4, 0x1
    cmpw r3, r0
    bge @L_800439A8
    addi r0, r3, 0x1
    li r3, 0x23
    stw r0, 0x0(r6)
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    lis r3, lbl_803A6818@ha
    lfs f0, lbl_8047BCF4(r2)
    addi r5, r3, lbl_803A6818@l
    lfs f1, 0x38(r5)
    lwz r4, 0x8(r5)
    fsubs f0, f1, f0
    lwz r3, 0xc(r5)
    addi r4, r4, 0x1
    addi r0, r3, 0x1
    stw r4, 0x8(r5)
    stfs f0, 0x38(r5)
    stw r0, 0xc(r5)
    b @L_800439A8
    @L_800437BC:
    clrlwi r0, r7, 31
    cmpwi r0, 0x0
    beq @L_8004382C
    lhz r0, lbl_8047A4E8(r13)
    cmplwi r0, 0x0
    beq @L_800439A8
    lwz r3, 0x0(r6)
    cmpwi r3, 0x0
    ble @L_800439A8
    subi r0, r3, 0x1
    li r3, 0x23
    stw r0, 0x0(r6)
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    lis r3, lbl_803A6818@ha
    lfs f0, lbl_8047BCF4(r2)
    addi r5, r3, lbl_803A6818@l
    lfs f1, 0x38(r5)
    lwz r4, 0x8(r5)
    fadds f0, f1, f0
    lwz r3, 0xc(r5)
    subi r4, r4, 0x1
    subi r0, r3, 0x1
    stw r4, 0x8(r5)
    stfs f0, 0x38(r5)
    stw r0, 0xc(r5)
    b @L_800439A8
    @L_8004382C:
    rlwinm r0, r7, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_800438F4
    cmpwi r4, 0x0
    bne @L_800439A8
    lhz r3, lbl_8047A4E8(r13)
    cmplwi r3, 0x0
    beq @L_800439A8
    lwz r5, 0x0(r6)
    subi r4, r3, 0x1
    addi r0, r5, 0xa
    cmpw r0, r4
    stw r0, 0x0(r6)
    ble @L_800438B8
    mulli r3, r4, 0x1b
    lis r0, 0x4330
    stw r0, 0x8(r1)
    cmpw r5, r4
    lfd f1, lbl_8047BCB0(r2)
    xoris r0, r3, 0x8000
    stw r0, 0xc(r1)
    lfd f0, 0x8(r1)
    stw r4, 0x0(r6)
    fsubs f0, f0, f1
    fneg f0, f0
    stfs f0, 0x38(r6)
    beq @L_800439A8
    lwz r3, 0x8(r6)
    subf r4, r5, r4
    lwz r0, 0xc(r6)
    add r3, r3, r4
    add r0, r0, r4
    stw r3, 0x8(r6)
    stw r0, 0xc(r6)
    b @L_800439A8
    @L_800438B8:
    lfs f1, 0x38(r6)
    li r3, 0x23
    lfs f0, lbl_8047BCF8(r2)
    li r4, 0x0
    lwz r8, 0x8(r6)
    li r5, 0x0
    fsubs f0, f1, f0
    lwz r7, 0xc(r6)
    addi r8, r8, 0xa
    addi r0, r7, 0xa
    stw r8, 0x8(r6)
    stfs f0, 0x38(r6)
    stw r0, 0xc(r6)
    bl fn_80166AB8
    b @L_800439A8
    @L_800438F4:
    rlwinm r0, r7, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_800439A8
    cmpwi r4, 0x0
    bne @L_800439A8
    lhz r4, lbl_8047A4E8(r13)
    cmplwi r4, 0x0
    beq @L_800439A8
    lwz r3, 0x0(r6)
    subi r0, r3, 0xa
    cmpwi r0, 0x0
    stw r0, 0x0(r6)
    bge @L_80043970
    li r0, 0x0
    lfs f0, lbl_8047BC94(r2)
    cmpw r3, r0
    stw r0, 0x0(r6)
    stfs f0, 0x38(r6)
    beq @L_800439A8
    li r0, -0x5
    cmplwi r4, 0x5
    stw r0, 0x8(r6)
    blt @L_8004395C
    li r0, 0x5
    stw r0, 0xc(r6)
    b @L_80043960
    @L_8004395C:
    stw r4, 0xc(r6)
    @L_80043960:
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    stw r4, 0x10(r3)
    b @L_800439A8
    @L_80043970:
    lfs f1, 0x38(r6)
    li r3, 0x23
    lfs f0, lbl_8047BCF8(r2)
    li r4, 0x0
    lwz r8, 0x8(r6)
    li r5, 0x0
    fadds f0, f1, f0
    lwz r7, 0xc(r6)
    subi r8, r8, 0xa
    subi r0, r7, 0xa
    stw r8, 0x8(r6)
    stfs f0, 0x38(r6)
    stw r0, 0xc(r6)
    bl fn_80166AB8
    @L_800439A8:
    li r3, 0x0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stmw r27, 0x5c(r1)
    mr r27, r3
    lis r3, lbl_80271068@ha
    addi r31, r3, lbl_80271068@l
    lwz r4, 0x14(r27)
    addi r0, r4, 0x1
    stw r0, 0x14(r27)
    lwz r3, 0x14(r27)
    lbz r28, 0x0(r4)
    addi r0, r3, 0x1
    stw r0, 0x14(r27)
    cmplwi r28, 0x0
    lwz r0, lbl_8047E710(r2)
    lbz r29, 0x0(r3)
    stw r0, 0x2c(r1)
    bne @L_800F3480
    addi r3, r31, 0x28
    clrlwi r4, r28, 16
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x2c(r1)
    stw r0, 0x34(r1)
    b @L_800F35A4
    @L_800F3480:
    rlwinm. r0, r28, 0, 24, 24
    clrlwi r30, r28, 16
    beq @L_800F34D4
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F34B0
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x1c(r1)
    b @L_800F34C8
    @L_800F34B0:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x1c(r1)
    @L_800F34C8:
    lwz r0, 0x1c(r1)
    stw r0, 0xc(r1)
    b @L_800F359C
    @L_800F34D4:
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F34F8
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x18(r1)
    b @L_800F3510
    @L_800F34F8:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x18(r1)
    @L_800F3510:
    lwz r4, 0x18(r1)
    rlwinm. r0, r30, 0, 26, 26
    stw r4, 0x30(r1)
    beq @L_800F3558
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F3544
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r0, r3, 0x6c
    add r0, r27, r0
    stw r0, 0xc(r1)
    b @L_800F359C
    @L_800F3544:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r0, r3, r0
    stw r0, 0xc(r1)
    b @L_800F359C
    @L_800F3558:
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F3578
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r3, r3, 0x6c
    add r3, r27, r3
    b @L_800F3584
    @L_800F3578:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r3, r3, r0
    @L_800F3584:
    rlwinm. r0, r30, 0, 23, 23
    beq @L_800F3594
    stw r3, 0xc(r1)
    b @L_800F359C
    @L_800F3594:
    lwz r0, 0x0(r3)
    stw r0, 0xc(r1)
    @L_800F359C:
    lwz r0, 0xc(r1)
    stw r0, 0x34(r1)
    @L_800F35A4:
    lwz r3, 0x34(r1)
    cmplwi r29, 0x0
    lwz r0, lbl_8047E710(r2)
    stw r3, 0x3c(r1)
    stw r0, 0x20(r1)
    bne @L_800F35D8
    addi r3, r31, 0x28
    clrlwi r4, r29, 16
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x20(r1)
    stw r0, 0x28(r1)
    b @L_800F36FC
    @L_800F35D8:
    rlwinm. r0, r29, 0, 24, 24
    clrlwi r30, r29, 16
    beq @L_800F362C
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F3608
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x14(r1)
    b @L_800F3620
    @L_800F3608:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x14(r1)
    @L_800F3620:
    lwz r0, 0x14(r1)
    stw r0, 0x8(r1)
    b @L_800F36F4
    @L_800F362C:
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F3650
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x10(r1)
    b @L_800F3668
    @L_800F3650:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x10(r1)
    @L_800F3668:
    lwz r4, 0x10(r1)
    rlwinm. r0, r30, 0, 26, 26
    stw r4, 0x24(r1)
    beq @L_800F36B0
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F369C
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r0, r3, 0x6c
    add r0, r27, r0
    stw r0, 0x8(r1)
    b @L_800F36F4
    @L_800F369C:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r0, r3, r0
    stw r0, 0x8(r1)
    b @L_800F36F4
    @L_800F36B0:
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F36D0
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r3, r3, 0x6c
    add r3, r27, r3
    b @L_800F36DC
    @L_800F36D0:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r3, r3, r0
    @L_800F36DC:
    rlwinm. r0, r30, 0, 23, 23
    beq @L_800F36EC
    stw r3, 0x8(r1)
    b @L_800F36F4
    @L_800F36EC:
    lwz r0, 0x0(r3)
    stw r0, 0x8(r1)
    @L_800F36F4:
    lwz r0, 0x8(r1)
    stw r0, 0x28(r1)
    @L_800F36FC:
    clrlwi r0, r29, 26
    lwz r4, 0x28(r1)
    cmplwi r0, 0x2
    stw r4, 0x40(r1)
    bne @L_800F3778
    clrlwi r0, r28, 26
    cmplwi r0, 0x2
    bne @L_800F373C
    lwz r0, 0x3c(r1)
    xor r0, r4, r0
    srawi r3, r0, 1
    and r0, r0, r4
    subf r0, r0, r3
    srwi r0, r0, 31
    stw r0, 0x38(r1)
    b @L_800F37E4
    @L_800F373C:
    xoris r3, r4, 0x8000
    lis r0, 0x4330
    stw r3, 0x4c(r1)
    lfd f2, lbl_8047CCC0(r2)
    stw r0, 0x48(r1)
    lfs f0, 0x3c(r1)
    lfd f1, 0x48(r1)
    fsubs f1, f1, f2
    fcmpo cr0, f1, f0
    ble @L_800F376C
    li r0, 0x1
    b @L_800F3770
    @L_800F376C:
    li r0, 0x0
    @L_800F3770:
    stw r0, 0x38(r1)
    b @L_800F37E4
    @L_800F3778:
    clrlwi r0, r28, 26
    cmplwi r0, 0x2
    bne @L_800F37C4
    lwz r3, 0x3c(r1)
    lis r0, 0x4330
    stw r0, 0x48(r1)
    xoris r0, r3, 0x8000
    lfd f1, lbl_8047CCC0(r2)
    stw r0, 0x4c(r1)
    lfs f2, 0x40(r1)
    lfd f0, 0x48(r1)
    fsubs f0, f0, f1
    fcmpo cr0, f2, f0
    ble @L_800F37B8
    li r0, 0x1
    b @L_800F37BC
    @L_800F37B8:
    li r0, 0x0
    @L_800F37BC:
    stw r0, 0x38(r1)
    b @L_800F37E4
    @L_800F37C4:
    lfs f1, 0x40(r1)
    lfs f0, 0x3c(r1)
    fcmpo cr0, f1, f0
    ble @L_800F37DC
    li r0, 0x1
    b @L_800F37E0
    @L_800F37DC:
    li r0, 0x0
    @L_800F37E0:
    stw r0, 0x38(r1)
    @L_800F37E4:
    lwz r4, 0x28(r27)
    cmpwi r4, 0x40
    ble @L_800F3800
    addi r3, r31, 0x0
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F3818
    @L_800F3800:
    addi r3, r4, 0x1
    slwi r0, r4, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x38(r1)
    stw r0, 0x6c(r3)
    @L_800F3818:
    lmw r27, 0x5c(r1)
    li r3, 0x1
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr

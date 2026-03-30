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
    bne @L_800F283C
    addi r3, r31, 0x28
    clrlwi r4, r28, 16
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x2c(r1)
    stw r0, 0x34(r1)
    b @L_800F2960
    @L_800F283C:
    rlwinm. r0, r28, 0, 24, 24
    clrlwi r30, r28, 16
    beq @L_800F2890
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F286C
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x1c(r1)
    b @L_800F2884
    @L_800F286C:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x1c(r1)
    @L_800F2884:
    lwz r0, 0x1c(r1)
    stw r0, 0xc(r1)
    b @L_800F2958
    @L_800F2890:
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F28B4
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x18(r1)
    b @L_800F28CC
    @L_800F28B4:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x18(r1)
    @L_800F28CC:
    lwz r4, 0x18(r1)
    rlwinm. r0, r30, 0, 26, 26
    stw r4, 0x30(r1)
    beq @L_800F2914
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F2900
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r0, r3, 0x6c
    add r0, r27, r0
    stw r0, 0xc(r1)
    b @L_800F2958
    @L_800F2900:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r0, r3, r0
    stw r0, 0xc(r1)
    b @L_800F2958
    @L_800F2914:
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F2934
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r3, r3, 0x6c
    add r3, r27, r3
    b @L_800F2940
    @L_800F2934:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r3, r3, r0
    @L_800F2940:
    rlwinm. r0, r30, 0, 23, 23
    beq @L_800F2950
    stw r3, 0xc(r1)
    b @L_800F2958
    @L_800F2950:
    lwz r0, 0x0(r3)
    stw r0, 0xc(r1)
    @L_800F2958:
    lwz r0, 0xc(r1)
    stw r0, 0x34(r1)
    @L_800F2960:
    lwz r3, 0x34(r1)
    cmplwi r29, 0x0
    lwz r0, lbl_8047E710(r2)
    stw r3, 0x3c(r1)
    stw r0, 0x20(r1)
    bne @L_800F2994
    addi r3, r31, 0x28
    clrlwi r4, r29, 16
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x20(r1)
    stw r0, 0x28(r1)
    b @L_800F2AB8
    @L_800F2994:
    rlwinm. r0, r29, 0, 24, 24
    clrlwi r30, r29, 16
    beq @L_800F29E8
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F29C4
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x14(r1)
    b @L_800F29DC
    @L_800F29C4:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x14(r1)
    @L_800F29DC:
    lwz r0, 0x14(r1)
    stw r0, 0x8(r1)
    b @L_800F2AB0
    @L_800F29E8:
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F2A0C
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x10(r1)
    b @L_800F2A24
    @L_800F2A0C:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x10(r1)
    @L_800F2A24:
    lwz r4, 0x10(r1)
    rlwinm. r0, r30, 0, 26, 26
    stw r4, 0x24(r1)
    beq @L_800F2A6C
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F2A58
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r0, r3, 0x6c
    add r0, r27, r0
    stw r0, 0x8(r1)
    b @L_800F2AB0
    @L_800F2A58:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r0, r3, r0
    stw r0, 0x8(r1)
    b @L_800F2AB0
    @L_800F2A6C:
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F2A8C
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r3, r3, 0x6c
    add r3, r27, r3
    b @L_800F2A98
    @L_800F2A8C:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r3, r3, r0
    @L_800F2A98:
    rlwinm. r0, r30, 0, 23, 23
    beq @L_800F2AA8
    stw r3, 0x8(r1)
    b @L_800F2AB0
    @L_800F2AA8:
    lwz r0, 0x0(r3)
    stw r0, 0x8(r1)
    @L_800F2AB0:
    lwz r0, 0x8(r1)
    stw r0, 0x28(r1)
    @L_800F2AB8:
    clrlwi r0, r29, 26
    lwz r4, 0x28(r1)
    cmplwi r0, 0x2
    stw r4, 0x40(r1)
    bne @L_800F2B30
    clrlwi r0, r28, 26
    cmplwi r0, 0x2
    bne @L_800F2AF4
    lwz r0, 0x3c(r1)
    subf r3, r4, r0
    subf r0, r0, r4
    or r0, r3, r0
    srwi r0, r0, 31
    stw r0, 0x38(r1)
    b @L_800F2B9C
    @L_800F2AF4:
    xoris r3, r4, 0x8000
    lis r0, 0x4330
    stw r3, 0x4c(r1)
    lfd f2, lbl_8047CCC0(r2)
    stw r0, 0x48(r1)
    lfs f0, 0x3c(r1)
    lfd f1, 0x48(r1)
    fsubs f1, f1, f2
    fcmpu cr0, f1, f0
    beq @L_800F2B24
    li r0, 0x1
    b @L_800F2B28
    @L_800F2B24:
    li r0, 0x0
    @L_800F2B28:
    stw r0, 0x38(r1)
    b @L_800F2B9C
    @L_800F2B30:
    clrlwi r0, r28, 26
    cmplwi r0, 0x2
    bne @L_800F2B7C
    lwz r3, 0x3c(r1)
    lis r0, 0x4330
    stw r0, 0x48(r1)
    xoris r0, r3, 0x8000
    lfd f1, lbl_8047CCC0(r2)
    stw r0, 0x4c(r1)
    lfs f2, 0x40(r1)
    lfd f0, 0x48(r1)
    fsubs f0, f0, f1
    fcmpu cr0, f2, f0
    beq @L_800F2B70
    li r0, 0x1
    b @L_800F2B74
    @L_800F2B70:
    li r0, 0x0
    @L_800F2B74:
    stw r0, 0x38(r1)
    b @L_800F2B9C
    @L_800F2B7C:
    lfs f1, 0x40(r1)
    lfs f0, 0x3c(r1)
    fcmpu cr0, f1, f0
    beq @L_800F2B94
    li r0, 0x1
    b @L_800F2B98
    @L_800F2B94:
    li r0, 0x0
    @L_800F2B98:
    stw r0, 0x38(r1)
    @L_800F2B9C:
    lwz r4, 0x28(r27)
    cmpwi r4, 0x40
    ble @L_800F2BB8
    addi r3, r31, 0x0
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F2BD0
    @L_800F2BB8:
    addi r3, r4, 0x1
    slwi r0, r4, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x38(r1)
    stw r0, 0x6c(r3)
    @L_800F2BD0:
    lmw r27, 0x5c(r1)
    li r3, 0x1
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr

    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stmw r27, 0x4c(r1)
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
    bne @L_800F1A74
    addi r3, r31, 0x28
    clrlwi r4, r28, 16
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x2c(r1)
    stw r0, 0x34(r1)
    b @L_800F1B98
    @L_800F1A74:
    rlwinm. r0, r28, 0, 24, 24
    clrlwi r30, r28, 16
    beq @L_800F1AC8
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F1AA4
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x1c(r1)
    b @L_800F1ABC
    @L_800F1AA4:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x1c(r1)
    @L_800F1ABC:
    lwz r0, 0x1c(r1)
    stw r0, 0xc(r1)
    b @L_800F1B90
    @L_800F1AC8:
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F1AEC
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x18(r1)
    b @L_800F1B04
    @L_800F1AEC:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x18(r1)
    @L_800F1B04:
    lwz r4, 0x18(r1)
    rlwinm. r0, r30, 0, 26, 26
    stw r4, 0x30(r1)
    beq @L_800F1B4C
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F1B38
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r0, r3, 0x6c
    add r0, r27, r0
    stw r0, 0xc(r1)
    b @L_800F1B90
    @L_800F1B38:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r0, r3, r0
    stw r0, 0xc(r1)
    b @L_800F1B90
    @L_800F1B4C:
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F1B6C
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r3, r3, 0x6c
    add r3, r27, r3
    b @L_800F1B78
    @L_800F1B6C:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r3, r3, r0
    @L_800F1B78:
    rlwinm. r0, r30, 0, 23, 23
    beq @L_800F1B88
    stw r3, 0xc(r1)
    b @L_800F1B90
    @L_800F1B88:
    lwz r0, 0x0(r3)
    stw r0, 0xc(r1)
    @L_800F1B90:
    lwz r0, 0xc(r1)
    stw r0, 0x34(r1)
    @L_800F1B98:
    lwz r3, 0x34(r1)
    cmplwi r29, 0x0
    lwz r0, lbl_8047E710(r2)
    stw r3, 0x3c(r1)
    stw r0, 0x20(r1)
    bne @L_800F1BCC
    addi r3, r31, 0x28
    clrlwi r4, r29, 16
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x20(r1)
    stw r0, 0x28(r1)
    b @L_800F1CF0
    @L_800F1BCC:
    rlwinm. r0, r29, 0, 24, 24
    clrlwi r30, r29, 16
    beq @L_800F1C20
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F1BFC
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x14(r1)
    b @L_800F1C14
    @L_800F1BFC:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x14(r1)
    @L_800F1C14:
    lwz r0, 0x14(r1)
    stw r0, 0x8(r1)
    b @L_800F1CE8
    @L_800F1C20:
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F1C44
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x10(r1)
    b @L_800F1C5C
    @L_800F1C44:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x10(r1)
    @L_800F1C5C:
    lwz r4, 0x10(r1)
    rlwinm. r0, r30, 0, 26, 26
    stw r4, 0x24(r1)
    beq @L_800F1CA4
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F1C90
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r0, r3, 0x6c
    add r0, r27, r0
    stw r0, 0x8(r1)
    b @L_800F1CE8
    @L_800F1C90:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r0, r3, r0
    stw r0, 0x8(r1)
    b @L_800F1CE8
    @L_800F1CA4:
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F1CC4
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r3, r3, 0x6c
    add r3, r27, r3
    b @L_800F1CD0
    @L_800F1CC4:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r3, r3, r0
    @L_800F1CD0:
    rlwinm. r0, r30, 0, 23, 23
    beq @L_800F1CE0
    stw r3, 0x8(r1)
    b @L_800F1CE8
    @L_800F1CE0:
    lwz r0, 0x0(r3)
    stw r0, 0x8(r1)
    @L_800F1CE8:
    lwz r0, 0x8(r1)
    stw r0, 0x28(r1)
    @L_800F1CF0:
    clrlwi r0, r29, 26
    lwz r3, 0x28(r1)
    cmplwi r0, 0x2
    stw r3, 0x40(r1)
    bne @L_800F1D74
    clrlwi r0, r28, 26
    cmplwi r0, 0x2
    bne @L_800F1D40
    cmpwi r3, 0x0
    li r3, 0x0
    bne @L_800F1D28
    lwz r0, 0x3c(r1)
    cmpwi r0, 0x0
    beq @L_800F1D2C
    @L_800F1D28:
    li r3, 0x1
    @L_800F1D2C:
    neg r0, r3
    or r0, r0, r3
    srwi r0, r0, 31
    stw r0, 0x38(r1)
    b @L_800F1DEC
    @L_800F1D40:
    cmpwi r3, 0x0
    li r3, 0x0
    bne @L_800F1D5C
    lfs f1, 0x3c(r1)
    lfs f0, lbl_8047CCBC(r2)
    fcmpu cr0, f1, f0
    beq @L_800F1D60
    @L_800F1D5C:
    li r3, 0x1
    @L_800F1D60:
    neg r0, r3
    or r0, r0, r3
    srwi r0, r0, 31
    stw r0, 0x38(r1)
    b @L_800F1DEC
    @L_800F1D74:
    clrlwi r0, r28, 26
    cmplwi r0, 0x2
    bne @L_800F1DB8
    lfs f1, 0x40(r1)
    li r3, 0x0
    lfs f0, lbl_8047CCBC(r2)
    fcmpu cr0, f1, f0
    bne @L_800F1DA0
    lwz r0, 0x3c(r1)
    cmpwi r0, 0x0
    beq @L_800F1DA4
    @L_800F1DA0:
    li r3, 0x1
    @L_800F1DA4:
    neg r0, r3
    or r0, r0, r3
    srwi r0, r0, 31
    stw r0, 0x38(r1)
    b @L_800F1DEC
    @L_800F1DB8:
    lfs f0, 0x40(r1)
    li r3, 0x0
    lfs f1, lbl_8047CCBC(r2)
    fcmpu cr0, f0, f1
    bne @L_800F1DD8
    lfs f0, 0x3c(r1)
    fcmpu cr0, f0, f1
    beq @L_800F1DDC
    @L_800F1DD8:
    li r3, 0x1
    @L_800F1DDC:
    neg r0, r3
    or r0, r0, r3
    srwi r0, r0, 31
    stw r0, 0x38(r1)
    @L_800F1DEC:
    lwz r4, 0x28(r27)
    cmpwi r4, 0x40
    ble @L_800F1E08
    addi r3, r31, 0x0
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F1E20
    @L_800F1E08:
    addi r3, r4, 0x1
    slwi r0, r4, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x38(r1)
    stw r0, 0x6c(r3)
    @L_800F1E20:
    lmw r27, 0x4c(r1)
    li r3, 0x1
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr

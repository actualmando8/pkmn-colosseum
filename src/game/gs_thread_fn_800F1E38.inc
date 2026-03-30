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
    bne @L_800F1EA0
    addi r3, r31, 0x28
    clrlwi r4, r28, 16
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x2c(r1)
    stw r0, 0x34(r1)
    b @L_800F1FC4
    @L_800F1EA0:
    rlwinm. r0, r28, 0, 24, 24
    clrlwi r30, r28, 16
    beq @L_800F1EF4
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F1ED0
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x1c(r1)
    b @L_800F1EE8
    @L_800F1ED0:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x1c(r1)
    @L_800F1EE8:
    lwz r0, 0x1c(r1)
    stw r0, 0xc(r1)
    b @L_800F1FBC
    @L_800F1EF4:
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F1F18
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x18(r1)
    b @L_800F1F30
    @L_800F1F18:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x18(r1)
    @L_800F1F30:
    lwz r4, 0x18(r1)
    rlwinm. r0, r30, 0, 26, 26
    stw r4, 0x30(r1)
    beq @L_800F1F78
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F1F64
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r0, r3, 0x6c
    add r0, r27, r0
    stw r0, 0xc(r1)
    b @L_800F1FBC
    @L_800F1F64:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r0, r3, r0
    stw r0, 0xc(r1)
    b @L_800F1FBC
    @L_800F1F78:
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F1F98
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r3, r3, 0x6c
    add r3, r27, r3
    b @L_800F1FA4
    @L_800F1F98:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r3, r3, r0
    @L_800F1FA4:
    rlwinm. r0, r30, 0, 23, 23
    beq @L_800F1FB4
    stw r3, 0xc(r1)
    b @L_800F1FBC
    @L_800F1FB4:
    lwz r0, 0x0(r3)
    stw r0, 0xc(r1)
    @L_800F1FBC:
    lwz r0, 0xc(r1)
    stw r0, 0x34(r1)
    @L_800F1FC4:
    lwz r3, 0x34(r1)
    cmplwi r29, 0x0
    lwz r0, lbl_8047E710(r2)
    stw r3, 0x3c(r1)
    stw r0, 0x20(r1)
    bne @L_800F1FF8
    addi r3, r31, 0x28
    clrlwi r4, r29, 16
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x20(r1)
    stw r0, 0x28(r1)
    b @L_800F211C
    @L_800F1FF8:
    rlwinm. r0, r29, 0, 24, 24
    clrlwi r30, r29, 16
    beq @L_800F204C
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F2028
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x14(r1)
    b @L_800F2040
    @L_800F2028:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x14(r1)
    @L_800F2040:
    lwz r0, 0x14(r1)
    stw r0, 0x8(r1)
    b @L_800F2114
    @L_800F204C:
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F2070
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x10(r1)
    b @L_800F2088
    @L_800F2070:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x10(r1)
    @L_800F2088:
    lwz r4, 0x10(r1)
    rlwinm. r0, r30, 0, 26, 26
    stw r4, 0x24(r1)
    beq @L_800F20D0
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F20BC
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r0, r3, 0x6c
    add r0, r27, r0
    stw r0, 0x8(r1)
    b @L_800F2114
    @L_800F20BC:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r0, r3, r0
    stw r0, 0x8(r1)
    b @L_800F2114
    @L_800F20D0:
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F20F0
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r3, r3, 0x6c
    add r3, r27, r3
    b @L_800F20FC
    @L_800F20F0:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r3, r3, r0
    @L_800F20FC:
    rlwinm. r0, r30, 0, 23, 23
    beq @L_800F210C
    stw r3, 0x8(r1)
    b @L_800F2114
    @L_800F210C:
    lwz r0, 0x0(r3)
    stw r0, 0x8(r1)
    @L_800F2114:
    lwz r0, 0x8(r1)
    stw r0, 0x28(r1)
    @L_800F211C:
    clrlwi r0, r29, 26
    lwz r3, 0x28(r1)
    cmplwi r0, 0x2
    stw r3, 0x40(r1)
    bne @L_800F21A0
    clrlwi r0, r28, 26
    cmplwi r0, 0x2
    bne @L_800F216C
    cmpwi r3, 0x0
    li r3, 0x0
    beq @L_800F2158
    lwz r0, 0x3c(r1)
    cmpwi r0, 0x0
    beq @L_800F2158
    li r3, 0x1
    @L_800F2158:
    neg r0, r3
    or r0, r0, r3
    srwi r0, r0, 31
    stw r0, 0x38(r1)
    b @L_800F2218
    @L_800F216C:
    cmpwi r3, 0x0
    li r3, 0x0
    beq @L_800F218C
    lfs f1, 0x3c(r1)
    lfs f0, lbl_8047CCBC(r2)
    fcmpu cr0, f1, f0
    beq @L_800F218C
    li r3, 0x1
    @L_800F218C:
    neg r0, r3
    or r0, r0, r3
    srwi r0, r0, 31
    stw r0, 0x38(r1)
    b @L_800F2218
    @L_800F21A0:
    clrlwi r0, r28, 26
    cmplwi r0, 0x2
    bne @L_800F21E4
    lfs f1, 0x40(r1)
    li r3, 0x0
    lfs f0, lbl_8047CCBC(r2)
    fcmpu cr0, f1, f0
    beq @L_800F21D0
    lwz r0, 0x3c(r1)
    cmpwi r0, 0x0
    beq @L_800F21D0
    li r3, 0x1
    @L_800F21D0:
    neg r0, r3
    or r0, r0, r3
    srwi r0, r0, 31
    stw r0, 0x38(r1)
    b @L_800F2218
    @L_800F21E4:
    lfs f0, 0x40(r1)
    li r3, 0x0
    lfs f1, lbl_8047CCBC(r2)
    fcmpu cr0, f0, f1
    beq @L_800F2208
    lfs f0, 0x3c(r1)
    fcmpu cr0, f0, f1
    beq @L_800F2208
    li r3, 0x1
    @L_800F2208:
    neg r0, r3
    or r0, r0, r3
    srwi r0, r0, 31
    stw r0, 0x38(r1)
    @L_800F2218:
    lwz r4, 0x28(r27)
    cmpwi r4, 0x40
    ble @L_800F2234
    addi r3, r31, 0x0
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F224C
    @L_800F2234:
    addi r3, r4, 0x1
    slwi r0, r4, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x38(r1)
    stw r0, 0x6c(r3)
    @L_800F224C:
    lmw r27, 0x4c(r1)
    li r3, 0x1
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr

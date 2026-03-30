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
    bne @L_800F3CB8
    addi r3, r31, 0x28
    clrlwi r4, r28, 16
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x2c(r1)
    stw r0, 0x34(r1)
    b @L_800F3DDC
    @L_800F3CB8:
    rlwinm. r0, r28, 0, 24, 24
    clrlwi r30, r28, 16
    beq @L_800F3D0C
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F3CE8
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x1c(r1)
    b @L_800F3D00
    @L_800F3CE8:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x1c(r1)
    @L_800F3D00:
    lwz r0, 0x1c(r1)
    stw r0, 0xc(r1)
    b @L_800F3DD4
    @L_800F3D0C:
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F3D30
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x18(r1)
    b @L_800F3D48
    @L_800F3D30:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x18(r1)
    @L_800F3D48:
    lwz r4, 0x18(r1)
    rlwinm. r0, r30, 0, 26, 26
    stw r4, 0x30(r1)
    beq @L_800F3D90
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F3D7C
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r0, r3, 0x6c
    add r0, r27, r0
    stw r0, 0xc(r1)
    b @L_800F3DD4
    @L_800F3D7C:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r0, r3, r0
    stw r0, 0xc(r1)
    b @L_800F3DD4
    @L_800F3D90:
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F3DB0
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r3, r3, 0x6c
    add r3, r27, r3
    b @L_800F3DBC
    @L_800F3DB0:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r3, r3, r0
    @L_800F3DBC:
    rlwinm. r0, r30, 0, 23, 23
    beq @L_800F3DCC
    stw r3, 0xc(r1)
    b @L_800F3DD4
    @L_800F3DCC:
    lwz r0, 0x0(r3)
    stw r0, 0xc(r1)
    @L_800F3DD4:
    lwz r0, 0xc(r1)
    stw r0, 0x34(r1)
    @L_800F3DDC:
    lwz r3, 0x34(r1)
    cmplwi r29, 0x0
    lwz r0, lbl_8047E710(r2)
    stw r3, 0x3c(r1)
    stw r0, 0x20(r1)
    bne @L_800F3E10
    addi r3, r31, 0x28
    clrlwi r4, r29, 16
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x20(r1)
    stw r0, 0x28(r1)
    b @L_800F3F34
    @L_800F3E10:
    rlwinm. r0, r29, 0, 24, 24
    clrlwi r30, r29, 16
    beq @L_800F3E64
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F3E40
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x14(r1)
    b @L_800F3E58
    @L_800F3E40:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x14(r1)
    @L_800F3E58:
    lwz r0, 0x14(r1)
    stw r0, 0x8(r1)
    b @L_800F3F2C
    @L_800F3E64:
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F3E88
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x10(r1)
    b @L_800F3EA0
    @L_800F3E88:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x10(r1)
    @L_800F3EA0:
    lwz r4, 0x10(r1)
    rlwinm. r0, r30, 0, 26, 26
    stw r4, 0x24(r1)
    beq @L_800F3EE8
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F3ED4
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r0, r3, 0x6c
    add r0, r27, r0
    stw r0, 0x8(r1)
    b @L_800F3F2C
    @L_800F3ED4:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r0, r3, r0
    stw r0, 0x8(r1)
    b @L_800F3F2C
    @L_800F3EE8:
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F3F08
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r3, r3, 0x6c
    add r3, r27, r3
    b @L_800F3F14
    @L_800F3F08:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r3, r3, r0
    @L_800F3F14:
    rlwinm. r0, r30, 0, 23, 23
    beq @L_800F3F24
    stw r3, 0x8(r1)
    b @L_800F3F2C
    @L_800F3F24:
    lwz r0, 0x0(r3)
    stw r0, 0x8(r1)
    @L_800F3F2C:
    lwz r0, 0x8(r1)
    stw r0, 0x28(r1)
    @L_800F3F34:
    clrlwi r0, r29, 26
    lwz r3, 0x28(r1)
    cmplwi r0, 0x2
    stw r3, 0x40(r1)
    bne @L_800F3FB0
    clrlwi r0, r28, 26
    cmplwi r0, 0x2
    bne @L_800F3F74
    lwz r4, 0x3c(r1)
    xor r0, r4, r3
    srawi r3, r0, 1
    and r0, r0, r4
    subf r0, r0, r3
    srwi r0, r0, 31
    stw r0, 0x38(r1)
    b @L_800F401C
    @L_800F3F74:
    xoris r3, r3, 0x8000
    lis r0, 0x4330
    stw r3, 0x4c(r1)
    lfd f2, lbl_8047CCC0(r2)
    stw r0, 0x48(r1)
    lfs f0, 0x3c(r1)
    lfd f1, 0x48(r1)
    fsubs f1, f1, f2
    fcmpo cr0, f1, f0
    bge @L_800F3FA4
    li r0, 0x1
    b @L_800F3FA8
    @L_800F3FA4:
    li r0, 0x0
    @L_800F3FA8:
    stw r0, 0x38(r1)
    b @L_800F401C
    @L_800F3FB0:
    clrlwi r0, r28, 26
    cmplwi r0, 0x2
    bne @L_800F3FFC
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
    bge @L_800F3FF0
    li r0, 0x1
    b @L_800F3FF4
    @L_800F3FF0:
    li r0, 0x0
    @L_800F3FF4:
    stw r0, 0x38(r1)
    b @L_800F401C
    @L_800F3FFC:
    lfs f1, 0x40(r1)
    lfs f0, 0x3c(r1)
    fcmpo cr0, f1, f0
    bge @L_800F4014
    li r0, 0x1
    b @L_800F4018
    @L_800F4014:
    li r0, 0x0
    @L_800F4018:
    stw r0, 0x38(r1)
    @L_800F401C:
    lwz r4, 0x28(r27)
    cmpwi r4, 0x40
    ble @L_800F4038
    addi r3, r31, 0x0
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F4050
    @L_800F4038:
    addi r3, r4, 0x1
    slwi r0, r4, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x38(r1)
    stw r0, 0x6c(r3)
    @L_800F4050:
    lmw r27, 0x5c(r1)
    li r3, 0x1
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr

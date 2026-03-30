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
    bne @L_800F4CA0
    addi r3, r31, 0x28
    clrlwi r4, r28, 16
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x2c(r1)
    stw r0, 0x34(r1)
    b @L_800F4DC4
    @L_800F4CA0:
    rlwinm. r0, r28, 0, 24, 24
    clrlwi r30, r28, 16
    beq @L_800F4CF4
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F4CD0
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x1c(r1)
    b @L_800F4CE8
    @L_800F4CD0:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x1c(r1)
    @L_800F4CE8:
    lwz r0, 0x1c(r1)
    stw r0, 0xc(r1)
    b @L_800F4DBC
    @L_800F4CF4:
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F4D18
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x18(r1)
    b @L_800F4D30
    @L_800F4D18:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x18(r1)
    @L_800F4D30:
    lwz r4, 0x18(r1)
    rlwinm. r0, r30, 0, 26, 26
    stw r4, 0x30(r1)
    beq @L_800F4D78
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F4D64
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r0, r3, 0x6c
    add r0, r27, r0
    stw r0, 0xc(r1)
    b @L_800F4DBC
    @L_800F4D64:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r0, r3, r0
    stw r0, 0xc(r1)
    b @L_800F4DBC
    @L_800F4D78:
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F4D98
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r3, r3, 0x6c
    add r3, r27, r3
    b @L_800F4DA4
    @L_800F4D98:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r3, r3, r0
    @L_800F4DA4:
    rlwinm. r0, r30, 0, 23, 23
    beq @L_800F4DB4
    stw r3, 0xc(r1)
    b @L_800F4DBC
    @L_800F4DB4:
    lwz r0, 0x0(r3)
    stw r0, 0xc(r1)
    @L_800F4DBC:
    lwz r0, 0xc(r1)
    stw r0, 0x34(r1)
    @L_800F4DC4:
    lwz r3, 0x34(r1)
    cmplwi r29, 0x0
    lwz r0, lbl_8047E710(r2)
    stw r3, 0x3c(r1)
    stw r0, 0x20(r1)
    bne @L_800F4DF8
    addi r3, r31, 0x28
    clrlwi r4, r29, 16
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x20(r1)
    stw r0, 0x28(r1)
    b @L_800F4F1C
    @L_800F4DF8:
    rlwinm. r0, r29, 0, 24, 24
    clrlwi r30, r29, 16
    beq @L_800F4E4C
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F4E28
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x14(r1)
    b @L_800F4E40
    @L_800F4E28:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x14(r1)
    @L_800F4E40:
    lwz r0, 0x14(r1)
    stw r0, 0x8(r1)
    b @L_800F4F14
    @L_800F4E4C:
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F4E70
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x10(r1)
    b @L_800F4E88
    @L_800F4E70:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x10(r1)
    @L_800F4E88:
    lwz r4, 0x10(r1)
    rlwinm. r0, r30, 0, 26, 26
    stw r4, 0x24(r1)
    beq @L_800F4ED0
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F4EBC
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r0, r3, 0x6c
    add r0, r27, r0
    stw r0, 0x8(r1)
    b @L_800F4F14
    @L_800F4EBC:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r0, r3, r0
    stw r0, 0x8(r1)
    b @L_800F4F14
    @L_800F4ED0:
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F4EF0
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r3, r3, 0x6c
    add r3, r27, r3
    b @L_800F4EFC
    @L_800F4EF0:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r3, r3, r0
    @L_800F4EFC:
    rlwinm. r0, r30, 0, 23, 23
    beq @L_800F4F0C
    stw r3, 0x8(r1)
    b @L_800F4F14
    @L_800F4F0C:
    lwz r0, 0x0(r3)
    stw r0, 0x8(r1)
    @L_800F4F14:
    lwz r0, 0x8(r1)
    stw r0, 0x28(r1)
    @L_800F4F1C:
    lwz r3, 0x3c(r1)
    lwz r4, 0x28(r1)
    cmpwi r3, 0x0
    stw r4, 0x40(r1)
    beq @L_800F4FCC
    clrlwi r0, r29, 26
    cmplwi r0, 0x2
    bne @L_800F4F80
    clrlwi r0, r28, 26
    cmplwi r0, 0x2
    bne @L_800F4F54
    divw r0, r4, r3
    stw r0, 0x38(r1)
    b @L_800F4FE0
    @L_800F4F54:
    xoris r3, r4, 0x8000
    lis r0, 0x4330
    stw r3, 0x4c(r1)
    lfd f2, lbl_8047CCC0(r2)
    stw r0, 0x48(r1)
    lfs f0, 0x3c(r1)
    lfd f1, 0x48(r1)
    fsubs f1, f1, f2
    fdivs f0, f1, f0
    stfs f0, 0x38(r1)
    b @L_800F4FE0
    @L_800F4F80:
    clrlwi r0, r28, 26
    cmplwi r0, 0x2
    bne @L_800F4FB8
    xoris r3, r3, 0x8000
    lis r0, 0x4330
    stw r3, 0x4c(r1)
    lfd f1, lbl_8047CCC0(r2)
    stw r0, 0x48(r1)
    lfs f2, 0x40(r1)
    lfd f0, 0x48(r1)
    fsubs f0, f0, f1
    fdivs f0, f2, f0
    stfs f0, 0x38(r1)
    b @L_800F4FE0
    @L_800F4FB8:
    lfs f1, 0x40(r1)
    lfs f0, 0x3c(r1)
    fdivs f0, f1, f0
    stfs f0, 0x38(r1)
    b @L_800F4FE0
    @L_800F4FCC:
    addi r3, r31, 0xd8
    crxor 6, 6, 6
    bl fn_800DD38C
    li r0, 0x0
    stw r0, 0x38(r1)
    @L_800F4FE0:
    lwz r4, 0x28(r27)
    cmpwi r4, 0x40
    ble @L_800F4FFC
    addi r3, r31, 0x0
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F5014
    @L_800F4FFC:
    addi r3, r4, 0x1
    slwi r0, r4, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x38(r1)
    stw r0, 0x6c(r3)
    @L_800F5014:
    lmw r27, 0x5c(r1)
    li r3, 0x1
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr

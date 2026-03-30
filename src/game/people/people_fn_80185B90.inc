    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stfd f31, 0x40(r1)
    psq_st f31, 0x48(r1), 0, 0
    stmw r25, 0x24(r1)
    mr r27, r3
    fmr f31, f1
    bl fn_8018FBD4
    mr. r25, r3
    beq @L_80185ECC
    lfs f1, lbl_8047D7A0(r2)
    lfs f0, 0xd8(r27)
    fcmpu cr0, f1, f0
    beq @L_80185C20
    bl fn_800D3088
    lis r0, 0x4330
    stw r3, 0x1c(r1)
    lfd f3, lbl_8047D7D0(r2)
    stw r0, 0x18(r1)
    lfs f2, 0xd8(r27)
    lfd f0, 0x18(r1)
    lfs f1, 0xd4(r27)
    fsubs f3, f0, f3
    lfs f0, lbl_8047D79C(r2)
    fmadds f1, f2, f3, f1
    stfs f1, 0xd4(r27)
    lfs f1, 0xd4(r27)
    fcmpo cr0, f1, f0
    ble @L_80185C14
    stfs f0, 0xd4(r27)
    lfs f0, lbl_8047D7A0(r2)
    stfs f0, 0xd8(r27)
    @L_80185C14:
    lfs f1, 0xd4(r27)
    mr r3, r25
    bl fn_800EC5B8
    @L_80185C20:
    mr r3, r27
    li r4, 0x8
    bl fn_8018FB94
    clrlwi. r0, r3, 24
    beq @L_80185ECC
    lfs f0, lbl_8047D830(r2)
    fcmpo cr0, f31, f0
    bge @L_80185C48
    li r3, 0x1
    b @L_80185C68
    @L_80185C48:
    cror eq, gt, eq
    bne @L_80185C64
    lfs f0, lbl_8047D834(r2)
    fcmpo cr0, f31, f0
    bge @L_80185C64
    li r3, 0x2
    b @L_80185C68
    @L_80185C64:
    li r3, 0x3
    @L_80185C68:
    lwz r0, 0x90(r27)
    cmpw r0, r3
    beq @L_80185ECC
    stw r3, 0x90(r27)
    lwz r3, 0x30(r27)
    bl fn_8018F6F4
    cmplwi r3, 0x0
    beq @L_80185ECC
    lwz r0, 0x90(r27)
    addi r5, r1, 0x14
    addi r6, r1, 0x8
    clrlwi r4, r0, 24
    bl fn_8018F4C8
    lwz r30, 0x14(r1)
    cmpwi r30, -0x1
    beq @L_80185ECC
    cmpwi r30, 0x0
    lwz r26, 0x2c(r27)
    lbz r29, 0x8(r1)
    li r31, 0x0
    lwz r27, 0x28(r27)
    blt @L_80185ECC
    mr r28, r31
    b @L_80185D00
    @L_80185CC8:
    mr r3, r28
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80185CFC
    lwz r0, 0x28(r3)
    cmplw r0, r27
    bne @L_80185CFC
    lwz r0, 0x2c(r3)
    cmplw r0, r26
    bne @L_80185CFC
    lwz r26, 0x4(r3)
    b @L_80185D6C
    @L_80185CFC:
    addi r28, r28, 0x1
    @L_80185D00:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_80185CC8
    li r25, 0x0
    b @L_80185D5C
    @L_80185D14:
    mr r3, r25
    bl fn_8018FD88
    mr r28, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80185D58
    lwz r0, 0x2c(r28)
    cmplw r0, r26
    bne @L_80185D58
    lis r3, lbl_80273FD8@ha
    mr r4, r27
    addi r3, r3, lbl_80273FD8@l
    mr r5, r26
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r26, 0x4(r28)
    b @L_80185D6C
    @L_80185D58:
    addi r25, r25, 0x1
    @L_80185D5C:
    bl fn_8018FDB4
    cmpw r25, r3
    blt @L_80185D14
    li r26, 0x0
    @L_80185D6C:
    li r27, 0x0
    b @L_80185DA0
    @L_80185D74:
    mr r3, r27
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80185D9C
    lwz r0, 0x4(r3)
    cmplw r0, r26
    bne @L_80185D9C
    mr r26, r3
    b @L_80185DB0
    @L_80185D9C:
    addi r27, r27, 0x1
    @L_80185DA0:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_80185D74
    li r26, 0x0
    @L_80185DB0:
    cmplwi r26, 0x0
    beq @L_80185ECC
    mr r3, r26
    bl fn_8018FBD4
    mr. r28, r3
    beq @L_80185ECC
    bl fn_800EC954
    clrlwi. r0, r3, 24
    beq @L_80185DDC
    li r31, 0x1
    b @L_80185E20
    @L_80185DDC:
    mr r3, r28
    bl fn_800EC960
    clrlwi. r0, r3, 24
    bne @L_80185DF4
    li r31, 0x1
    b @L_80185E20
    @L_80185DF4:
    mr r3, r28
    addi r4, r1, 0xc
    addi r5, r1, 0x10
    bl fn_800EC578
    lwz r0, 0xc(r1)
    cmpw r0, r30
    bne @L_80185E1C
    lwz r0, 0x10(r1)
    cmpwi r0, -0x1
    beq @L_80185E20
    @L_80185E1C:
    li r31, 0x1
    @L_80185E20:
    clrlwi. r0, r31, 24
    beq @L_80185EA8
    stw r30, 0x48(r26)
    mr r3, r28
    lfs f0, lbl_8047D7A0(r2)
    mr r4, r30
    stfs f0, 0xd8(r26)
    bl fn_800ECCA8
    lfs f1, lbl_8047D7A0(r2)
    mr r3, r28
    bl fn_800ECA78
    lfs f1, lbl_8047D7A4(r2)
    mr r3, r28
    bl fn_800EC9DC
    mr r3, r28
    mr r4, r30
    bl fn_800EC35C
    lfs f1, lbl_8047D7A0(r2)
    mr r3, r28
    bl fn_800EC2A4
    lfs f1, lbl_8047D7A4(r2)
    mr r3, r28
    bl fn_800EC308
    cmplwi r29, 0x0
    beq @L_80185E94
    mr r3, r28
    li r4, 0x1
    bl fn_800ECB74
    b @L_80185EA0
    @L_80185E94:
    mr r3, r28
    li r4, 0x0
    bl fn_800ECB74
    @L_80185EA0:
    mr r3, r28
    bl fn_800EC990
    @L_80185EA8:
    cmplwi r29, 0x0
    beq @L_80185EC0
    mr r3, r28
    li r4, 0x1
    bl fn_800ECB74
    b @L_80185ECC
    @L_80185EC0:
    mr r3, r28
    li r4, 0x0
    bl fn_800ECB74
    @L_80185ECC:
    psq_l f31, 0x48(r1), 0, 0
    lfd f31, 0x40(r1)
    lmw r25, 0x24(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr

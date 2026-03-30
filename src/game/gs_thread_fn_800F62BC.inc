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
    bne @L_800F6324
    addi r3, r31, 0x28
    clrlwi r4, r28, 16
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x2c(r1)
    stw r0, 0x34(r1)
    b @L_800F6448
    @L_800F6324:
    rlwinm. r0, r28, 0, 24, 24
    clrlwi r30, r28, 16
    beq @L_800F6378
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F6354
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x1c(r1)
    b @L_800F636C
    @L_800F6354:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x1c(r1)
    @L_800F636C:
    lwz r0, 0x1c(r1)
    stw r0, 0xc(r1)
    b @L_800F6440
    @L_800F6378:
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F639C
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x18(r1)
    b @L_800F63B4
    @L_800F639C:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x18(r1)
    @L_800F63B4:
    lwz r4, 0x18(r1)
    rlwinm. r0, r30, 0, 26, 26
    stw r4, 0x30(r1)
    beq @L_800F63FC
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F63E8
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r0, r3, 0x6c
    add r0, r27, r0
    stw r0, 0xc(r1)
    b @L_800F6440
    @L_800F63E8:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r0, r3, r0
    stw r0, 0xc(r1)
    b @L_800F6440
    @L_800F63FC:
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F641C
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r3, r3, 0x6c
    add r3, r27, r3
    b @L_800F6428
    @L_800F641C:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r3, r3, r0
    @L_800F6428:
    rlwinm. r0, r30, 0, 23, 23
    beq @L_800F6438
    stw r3, 0xc(r1)
    b @L_800F6440
    @L_800F6438:
    lwz r0, 0x0(r3)
    stw r0, 0xc(r1)
    @L_800F6440:
    lwz r0, 0xc(r1)
    stw r0, 0x34(r1)
    @L_800F6448:
    lwz r3, 0x34(r1)
    cmplwi r29, 0x0
    lwz r0, lbl_8047E710(r2)
    stw r3, 0x3c(r1)
    stw r0, 0x20(r1)
    bne @L_800F647C
    addi r3, r31, 0x28
    clrlwi r4, r29, 16
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x20(r1)
    stw r0, 0x28(r1)
    b @L_800F65A0
    @L_800F647C:
    rlwinm. r0, r29, 0, 24, 24
    clrlwi r30, r29, 16
    beq @L_800F64D0
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F64AC
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x14(r1)
    b @L_800F64C4
    @L_800F64AC:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x14(r1)
    @L_800F64C4:
    lwz r0, 0x14(r1)
    stw r0, 0x8(r1)
    b @L_800F6598
    @L_800F64D0:
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F64F4
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x10(r1)
    b @L_800F650C
    @L_800F64F4:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x10(r1)
    @L_800F650C:
    lwz r4, 0x10(r1)
    rlwinm. r0, r30, 0, 26, 26
    stw r4, 0x24(r1)
    beq @L_800F6554
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F6540
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r0, r3, 0x6c
    add r0, r27, r0
    stw r0, 0x8(r1)
    b @L_800F6598
    @L_800F6540:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r0, r3, r0
    stw r0, 0x8(r1)
    b @L_800F6598
    @L_800F6554:
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F6574
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r3, r3, 0x6c
    add r3, r27, r3
    b @L_800F6580
    @L_800F6574:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r3, r3, r0
    @L_800F6580:
    rlwinm. r0, r30, 0, 23, 23
    beq @L_800F6590
    stw r3, 0x8(r1)
    b @L_800F6598
    @L_800F6590:
    lwz r0, 0x0(r3)
    stw r0, 0x8(r1)
    @L_800F6598:
    lwz r0, 0x8(r1)
    stw r0, 0x28(r1)
    @L_800F65A0:
    clrlwi r0, r29, 26
    lwz r3, 0x28(r1)
    cmplwi r0, 0x2
    stw r3, 0x40(r1)
    bne @L_800F65EC
    clrlwi r0, r28, 26
    cmplwi r0, 0x2
    bne @L_800F65D0
    lwz r0, 0x3c(r1)
    and r0, r3, r0
    stw r0, 0x38(r1)
    b @L_800F6640
    @L_800F65D0:
    lfs f0, 0x3c(r1)
    fctiwz f0, f0
    stfd f0, 0x48(r1)
    lwz r0, 0x4c(r1)
    and r0, r3, r0
    stw r0, 0x38(r1)
    b @L_800F6640
    @L_800F65EC:
    clrlwi r0, r28, 26
    cmplwi r0, 0x2
    bne @L_800F6618
    lfs f0, 0x40(r1)
    lwz r0, 0x3c(r1)
    fctiwz f0, f0
    stfd f0, 0x48(r1)
    lwz r3, 0x4c(r1)
    and r0, r3, r0
    stw r0, 0x38(r1)
    b @L_800F6640
    @L_800F6618:
    lfs f1, 0x40(r1)
    lfs f0, 0x3c(r1)
    fctiwz f1, f1
    fctiwz f0, f0
    stfd f1, 0x48(r1)
    stfd f0, 0x50(r1)
    lwz r3, 0x4c(r1)
    lwz r0, 0x54(r1)
    and r0, r3, r0
    stw r0, 0x38(r1)
    @L_800F6640:
    lwz r4, 0x28(r27)
    cmpwi r4, 0x40
    ble @L_800F665C
    addi r3, r31, 0x0
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F6674
    @L_800F665C:
    addi r3, r4, 0x1
    slwi r0, r4, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x38(r1)
    stw r0, 0x6c(r3)
    @L_800F6674:
    lmw r27, 0x5c(r1)
    li r3, 0x1
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr

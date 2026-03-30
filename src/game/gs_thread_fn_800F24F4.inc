    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r27, 0x3c(r1)
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
    stw r0, 0x1c(r1)
    bne @L_800F255C
    addi r3, r31, 0x28
    clrlwi r4, r28, 16
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x1c(r1)
    stw r0, 0x24(r1)
    b @L_800F2680
    @L_800F255C:
    rlwinm. r0, r28, 0, 24, 24
    clrlwi r30, r28, 16
    beq @L_800F25B0
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F258C
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x14(r1)
    b @L_800F25A4
    @L_800F258C:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x14(r1)
    @L_800F25A4:
    lwz r0, 0x14(r1)
    stw r0, 0x8(r1)
    b @L_800F2678
    @L_800F25B0:
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F25D4
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0x10(r1)
    b @L_800F25EC
    @L_800F25D4:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x10(r1)
    @L_800F25EC:
    lwz r4, 0x10(r1)
    rlwinm. r0, r30, 0, 26, 26
    stw r4, 0x20(r1)
    beq @L_800F2634
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F2620
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r0, r3, 0x6c
    add r0, r27, r0
    stw r0, 0x8(r1)
    b @L_800F2678
    @L_800F2620:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r0, r3, r0
    stw r0, 0x8(r1)
    b @L_800F2678
    @L_800F2634:
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F2654
    lwz r0, 0x1c(r27)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r3, r3, 0x6c
    add r3, r27, r3
    b @L_800F2660
    @L_800F2654:
    lwz r3, 0x18(r27)
    slwi r0, r4, 2
    add r3, r3, r0
    @L_800F2660:
    rlwinm. r0, r30, 0, 23, 23
    beq @L_800F2670
    stw r3, 0x8(r1)
    b @L_800F2678
    @L_800F2670:
    lwz r0, 0x0(r3)
    stw r0, 0x8(r1)
    @L_800F2678:
    lwz r0, 0x8(r1)
    stw r0, 0x24(r1)
    @L_800F2680:
    lwz r3, 0x24(r1)
    clrlwi r0, r29, 26
    clrlwi r4, r28, 26
    cmpw r0, r4
    stw r3, 0x28(r1)
    beq @L_800F26D8
    cmpwi r4, 0x2
    bne @L_800F26C4
    xoris r3, r3, 0x8000
    lis r0, 0x4330
    stw r3, 0x34(r1)
    lfd f1, lbl_8047CCC0(r2)
    stw r0, 0x30(r1)
    lfd f0, 0x30(r1)
    fsubs f0, f0, f1
    stfs f0, 0x28(r1)
    b @L_800F26D8
    @L_800F26C4:
    lfs f0, 0x28(r1)
    fctiwz f0, f0
    stfd f0, 0x30(r1)
    lwz r0, 0x34(r1)
    stw r0, 0x28(r1)
    @L_800F26D8:
    cmplwi r29, 0x0
    bne @L_800F26F4
    addi r3, r31, 0x84
    clrlwi r4, r29, 16
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F2788
    @L_800F26F4:
    rlwinm. r0, r29, 0, 24, 24
    clrlwi r28, r29, 16
    beq @L_800F2710
    addi r3, r31, 0xa0
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F2788
    @L_800F2710:
    lwz r3, 0x28(r27)
    cmpwi r3, 0x0
    bgt @L_800F2734
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r27)
    stw r0, 0xc(r1)
    b @L_800F274C
    @L_800F2734:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x6c(r3)
    stw r0, 0xc(r1)
    @L_800F274C:
    lwz r5, 0xc(r1)
    rlwinm. r0, r28, 0, 25, 25
    stw r5, 0x18(r1)
    beq @L_800F2778
    lwz r3, 0x1c(r27)
    lwz r0, 0x28(r1)
    add r3, r3, r5
    slwi r3, r3, 2
    add r3, r27, r3
    stw r0, 0x6c(r3)
    b @L_800F2788
    @L_800F2778:
    lwz r4, 0x18(r27)
    slwi r3, r5, 2
    lwz r0, 0x28(r1)
    stwx r0, r4, r3
    @L_800F2788:
    lwz r4, 0x28(r27)
    cmpwi r4, 0x40
    ble @L_800F27A4
    addi r3, r31, 0x0
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F27BC
    @L_800F27A4:
    addi r3, r4, 0x1
    slwi r0, r4, 2
    stw r3, 0x28(r27)
    add r3, r27, r0
    lwz r0, 0x28(r1)
    stw r0, 0x6c(r3)
    @L_800F27BC:
    lmw r27, 0x3c(r1)
    li r3, 0x1
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr

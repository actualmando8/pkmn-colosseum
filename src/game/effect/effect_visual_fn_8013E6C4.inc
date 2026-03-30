    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr. r30, r3
    beq @L_8013E888
    lwz r31, 0x4(r30)
    cmplwi r31, 0x0
    bne @L_8013E6F4
    li r3, 0x0
    b @L_8013E88C
    @L_8013E6F4:
    bl fn_8013EA44
    lfs f1, lbl_8047D2AC(r2)
    lfs f3, lbl_8047D2B0(r2)
    fmr f2, f1
    lfs f4, lbl_8047D2B4(r2)
    bl fn_800D9B58
    lbz r0, 0x19(r30)
    rlwinm. r0, r0, 0, 29, 29
    beq @L_8013E72C
    li r3, 0x1
    li r4, 0x6
    li r5, 0x7
    bl fn_800DA4C4
    b @L_8013E73C
    @L_8013E72C:
    li r3, 0x1
    li r4, 0x6
    li r5, 0x1
    bl fn_800DA4C4
    @L_8013E73C:
    li r3, 0x0
    li r4, 0x7
    li r5, 0x0
    bl fn_800DA1E8
    li r3, 0x1
    li r4, 0x1
    li r5, 0x0
    bl fn_800DA2BC
    li r3, 0x0
    bl fn_800DA028
    li r3, 0x3
    bl fn_800D88DC
    li r3, 0x4
    bl fn_800D888C
    lis r3, lbl_80314AE8@ha
    addi r3, r3, lbl_80314AE8@l
    bl fn_800D7820
    li r3, 0x4
    bl fn_800D6A00
    mr r4, r31
    li r3, 0x0
    bl fn_800D85D4
    li r3, 0x4
    bl fn_800D67BC
    lfs f1, lbl_8047D2AC(r2)
    fmr f2, f1
    fmr f3, f1
    bl fn_800D6680
    lbz r4, 0x8(r30)
    li r3, 0x0
    lbz r5, 0x9(r30)
    lbz r6, 0xa(r30)
    lbz r7, 0xb(r30)
    bl fn_800D5CB8
    lfs f1, lbl_8047D2AC(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    lfs f2, lbl_8047D2AC(r2)
    lfs f1, lbl_8047D2B0(r2)
    fmr f3, f2
    bl fn_800D6680
    lbz r4, 0x8(r30)
    li r3, 0x0
    lbz r5, 0x9(r30)
    lbz r6, 0xa(r30)
    lbz r7, 0xb(r30)
    bl fn_800D5CB8
    lfs f1, lbl_8047D2A8(r2)
    li r3, 0x0
    lfs f2, lbl_8047D2AC(r2)
    bl fn_800D59B8
    lfs f1, lbl_8047D2AC(r2)
    lfs f2, lbl_8047D2B4(r2)
    fmr f3, f1
    bl fn_800D6680
    lbz r4, 0x8(r30)
    li r3, 0x0
    lbz r5, 0x9(r30)
    lbz r6, 0xa(r30)
    lbz r7, 0xb(r30)
    bl fn_800D5CB8
    lfs f1, lbl_8047D2AC(r2)
    li r3, 0x0
    lfs f2, lbl_8047D2A8(r2)
    bl fn_800D59B8
    lfs f1, lbl_8047D2B0(r2)
    lfs f2, lbl_8047D2B4(r2)
    lfs f3, lbl_8047D2AC(r2)
    bl fn_800D6680
    lbz r4, 0x8(r30)
    li r3, 0x0
    lbz r5, 0x9(r30)
    lbz r6, 0xa(r30)
    lbz r7, 0xb(r30)
    bl fn_800D5CB8
    lfs f1, lbl_8047D2A8(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    bl fn_800D6728
    li r3, 0x1
    b @L_8013E88C
    @L_8013E888:
    li r3, 0x0
    @L_8013E88C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

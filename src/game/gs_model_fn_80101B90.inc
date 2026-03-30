    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stw r31, 0x3c(r1)
    stw r30, 0x38(r1)
    stw r29, 0x34(r1)
    stw r28, 0x30(r1)
    mr r28, r3
    lwz r0, lbl_8047ACF0(r13)
    cmpwi r0, 0x0
    beq @L_80101D3C
    bl OSGetTick
    mr r31, r3
    li r3, 0x1
    bl fn_800D9ED8
    li r3, 0x1
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    lfs f1, lbl_8047CD80(r13)
    lfs f3, lbl_8047CD84(r13)
    fmr f2, f1
    lfs f4, lbl_8047CD88(r13)
    bl fn_800D9B58
    li r3, 0x1
    li r4, 0x6
    li r5, 0x7
    bl fn_800DA4C4
    li r3, 0x2
    li r4, 0x1
    li r5, 0x0
    bl fn_800DA2BC
    li r3, 0x1
    li r4, 0x1
    li r5, 0x1
    bl fn_800DA1E8
    li r3, 0x0
    bl fn_800DA028
    li r3, 0x7
    bl fn_800D6A00
    li r3, 0x0
    bl fn_800D7820
    rlwinm r5, r28, 16, 24, 31
    rlwinm r4, r28, 24, 24, 31
    rlwinm r0, r28, 0, 24, 31
    li r3, 0x2
    mr r30, r5
    mr r29, r4
    mr r28, r0
    bl fn_800D67BC
    lwz r5, lbl_8047ACE8(r13)
    lis r3, 0x4330
    lwz r4, lbl_8047ACEC(r13)
    lbz r0, lbl_80478B28(r13)
    subf r4, r5, r4
    stw r3, 0x8(r1)
    lfd f5, lbl_8047CD98(r13)
    stw r4, 0xc(r1)
    lfs f0, lbl_8047CD8C(r13)
    lfd f1, 0x8(r1)
    stw r0, 0x14(r1)
    fsubs f4, f1, f5
    lfs f6, lbl_8047CD84(r13)
    stw r3, 0x10(r1)
    lfs f2, lbl_8047CD90(r13)
    lfd f1, 0x10(r1)
    lfs f3, lbl_8047CD80(r13)
    fsubs f1, f1, f5
    fdivs f1, f4, f1
    fdivs f0, f1, f0
    fmuls f1, f6, f0
    bl fn_800D6680
    mr r4, r30
    mr r5, r29
    mr r6, r28
    li r3, 0x0
    li r7, 0xff
    bl fn_800D5CB8
    lwz r4, lbl_8047ACE8(r13)
    lis r3, 0x4330
    lbz r0, lbl_80478B28(r13)
    subf r4, r4, r31
    stw r3, 0x18(r1)
    lfd f5, lbl_8047CD98(r13)
    stw r4, 0x1c(r1)
    lfs f0, lbl_8047CD8C(r13)
    lfd f1, 0x18(r1)
    stw r0, 0x24(r1)
    fsubs f4, f1, f5
    lfs f6, lbl_8047CD84(r13)
    stw r3, 0x20(r1)
    lfs f2, lbl_8047CD94(r13)
    lfd f1, 0x20(r1)
    lfs f3, lbl_8047CD80(r13)
    fsubs f1, f1, f5
    fdivs f1, f4, f1
    fdivs f0, f1, f0
    fmuls f1, f6, f0
    bl fn_800D6680
    mr r4, r30
    mr r5, r29
    mr r6, r28
    li r3, 0x0
    li r7, 0xff
    bl fn_800D5CB8
    bl fn_800D6728
    stw r31, lbl_8047ACEC(r13)
@L_80101D3C:
    lwz r0, 0x44(r1)
    lwz r31, 0x3c(r1)
    lwz r30, 0x38(r1)
    lwz r29, 0x34(r1)
    lwz r28, 0x30(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr


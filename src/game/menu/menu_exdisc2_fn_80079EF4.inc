    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    stw r0, 0x84(r1)
    stfd f31, 0x70(r1)
    psq_st f31, 0x78(r1), 0, 0
    stfd f30, 0x60(r1)
    psq_st f30, 0x68(r1), 0, 0
    stfd f29, 0x50(r1)
    psq_st f29, 0x58(r1), 0, 0
    stfd f28, 0x40(r1)
    psq_st f28, 0x48(r1), 0, 0
    stfd f27, 0x30(r1)
    psq_st f27, 0x38(r1), 0, 0
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    mr r31, r3
    lwz r5, lbl_804788F0(r13)
    li r6, 0x0
    lis r3, lbl_802E61D8@ha
    stb r6, lbl_8047A630(r13)
    subi r29, r5, 0x1
    stb r6, lbl_8047A631(r13)
    rlwinm r5, r29, 2, 0, 29
    addi r0, r3, lbl_802E61D8@l
    stb r6, lbl_8047A632(r13)
    add r3, r0, r5
    addi r0, r29, 0x1
    mtctr r0
    cmpwi r29, 0x0
    blt @L_80079F88
@L_80079F70:
    lwz r0, 0x0(r3)
    cmplw r0, r4
    ble @L_80079F88
    subi r3, r3, 0x4
    subi r29, r29, 0x1
    bdnz @L_80079F70
@L_80079F88:
    cmpwi r29, 0x0
    bge @L_80079F94
    li r29, 0x0
@L_80079F94:
    li r3, 0xe1
    bl fn_80102510
    lfs f27, lbl_8047C114(r2)
    lfd f31, lbl_8047C118(r2)
    lis r30, 0x4330
    lfd f29, lbl_8047C120(r2)
    lfs f28, lbl_8047C128(r2)
    b @L_80079FEC
@L_80079FB4:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r30, 0x8(r1)
    stw r0, 0xc(r1)
    lfd f0, 0x8(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0x14(r1)
    stw r30, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fadds f27, f27, f0
@L_80079FEC:
    fcmpo cr0, f27, f28
    blt @L_80079FB4
    li r3, 0xef
    li r4, 0x0
    bl fn_8010264C
    lfs f27, lbl_8047C114(r2)
    lfd f31, lbl_8047C118(r2)
    lis r30, 0x4330
    lfd f29, lbl_8047C120(r2)
    lfs f28, lbl_8047C108(r2)
    b @L_8007A050
@L_8007A018:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r30, 0x10(r1)
    stw r0, 0x14(r1)
    lfd f0, 0x10(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0xc(r1)
    stw r30, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fadds f27, f27, f0
@L_8007A050:
    fcmpo cr0, f27, f28
    blt @L_8007A018
    cmpwi r29, 0x1
    bge @L_8007A10C
    li r3, 0x43a7
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    li r3, 0x1
    bl fn_801069FC
    cmpwi r31, 0x0
    bne @L_8007A09C
    li r3, 0x2
    li r4, 0x44cf
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
@L_8007A09C:
    li r3, 0xef
    bl fn_80102510
    lfs f27, lbl_8047C114(r2)
    lfd f31, lbl_8047C118(r2)
    lis r30, 0x4330
    lfd f29, lbl_8047C120(r2)
    lfs f28, lbl_8047C108(r2)
    b @L_8007A0F4
@L_8007A0BC:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r30, 0x10(r1)
    stw r0, 0x14(r1)
    lfd f0, 0x10(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0xc(r1)
    stw r30, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fadds f27, f27, f0
@L_8007A0F4:
    fcmpo cr0, f27, f28
    blt @L_8007A0BC
    li r0, 0x1
    li r3, 0x0
    stw r0, lbl_8047A638(r13)
    b @L_8007A5A4
@L_8007A10C:
    cmpwi r29, 0x2
    beq @L_8007A204
    bge @L_8007A124
    cmpwi r29, 0x1
    bge @L_8007A130
    b @L_8007A450
@L_8007A124:
    cmpwi r29, 0x4
    bge @L_8007A450
    b @L_8007A30C
@L_8007A130:
    lbz r0, lbl_8047A635(r13)
    cmplwi r0, 0x0
    beq @L_8007A1E8
    li r3, 0x43ae
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    li r3, 0x1
    bl fn_801069FC
    cmpwi r31, 0x0
    bne @L_8007A178
    li r3, 0x2
    li r4, 0x44cf
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
@L_8007A178:
    li r3, 0xef
    bl fn_80102510
    lfs f27, lbl_8047C114(r2)
    lfd f31, lbl_8047C118(r2)
    lis r30, 0x4330
    lfd f29, lbl_8047C120(r2)
    lfs f28, lbl_8047C108(r2)
    b @L_8007A1D0
@L_8007A198:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r30, 0x10(r1)
    stw r0, 0x14(r1)
    lfd f0, 0x10(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0xc(r1)
    stw r30, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fadds f27, f27, f0
@L_8007A1D0:
    fcmpo cr0, f27, f28
    blt @L_8007A198
    li r0, 0x1
    li r3, 0x0
    stw r0, lbl_8047A638(r13)
    b @L_8007A5A4
@L_8007A1E8:
    li r3, 0x43b4
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    li r0, 0x1
    stb r0, lbl_8047A632(r13)
    b @L_8007A4C8
@L_8007A204:
    lbz r3, lbl_8047A635(r13)
    cmplwi r3, 0x0
    beq @L_8007A2C8
    lbz r0, lbl_8047A634(r13)
    cmplwi r0, 0x0
    beq @L_8007A2C8
    li r3, 0x43ab
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    li r3, 0x1
    bl fn_801069FC
    cmpwi r31, 0x0
    bne @L_8007A258
    li r3, 0x2
    li r4, 0x44cf
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
@L_8007A258:
    li r3, 0xef
    bl fn_80102510
    lfs f27, lbl_8047C114(r2)
    lfd f31, lbl_8047C118(r2)
    lis r30, 0x4330
    lfd f29, lbl_8047C120(r2)
    lfs f28, lbl_8047C108(r2)
    b @L_8007A2B0
@L_8007A278:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r30, 0x10(r1)
    stw r0, 0x14(r1)
    lfd f0, 0x10(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0xc(r1)
    stw r30, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fadds f27, f27, f0
@L_8007A2B0:
    fcmpo cr0, f27, f28
    blt @L_8007A278
    li r0, 0x1
    li r3, 0x0
    stw r0, lbl_8047A638(r13)
    b @L_8007A5A4
@L_8007A2C8:
    cmplwi r3, 0x0
    beq @L_8007A2EC
    li r3, 0x43b3
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    li r0, 0x1
    stb r0, lbl_8047A631(r13)
    b @L_8007A4C8
@L_8007A2EC:
    li r3, 0x43b6
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    li r0, 0x1
    stb r0, lbl_8047A632(r13)
    stb r0, lbl_8047A631(r13)
    b @L_8007A4C8
@L_8007A30C:
    lbz r3, lbl_8047A635(r13)
    cmplwi r3, 0x0
    beq @L_8007A3DC
    lbz r0, lbl_8047A634(r13)
    cmplwi r0, 0x0
    beq @L_8007A3DC
    lbz r0, lbl_8047A633(r13)
    cmplwi r0, 0x0
    beq @L_8007A3DC
    li r3, 0x43a9
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    li r3, 0x1
    bl fn_801069FC
    cmpwi r31, 0x0
    bne @L_8007A36C
    li r3, 0x2
    li r4, 0x44cf
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
@L_8007A36C:
    li r3, 0xef
    bl fn_80102510
    lfs f27, lbl_8047C114(r2)
    lfd f31, lbl_8047C118(r2)
    lis r30, 0x4330
    lfd f29, lbl_8047C120(r2)
    lfs f28, lbl_8047C108(r2)
    b @L_8007A3C4
@L_8007A38C:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r30, 0x10(r1)
    stw r0, 0x14(r1)
    lfd f0, 0x10(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0xc(r1)
    stw r30, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fadds f27, f27, f0
@L_8007A3C4:
    fcmpo cr0, f27, f28
    blt @L_8007A38C
    li r0, 0x1
    li r3, 0x0
    stw r0, lbl_8047A638(r13)
    b @L_8007A5A4
@L_8007A3DC:
    lbz r0, lbl_8047A634(r13)
    cmplwi r0, 0x0
    beq @L_8007A404
    li r3, 0x43b1
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    li r0, 0x1
    stb r0, lbl_8047A630(r13)
    b @L_8007A4C8
@L_8007A404:
    cmplwi r3, 0x0
    beq @L_8007A42C
    li r3, 0x43b5
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    li r0, 0x1
    stb r0, lbl_8047A630(r13)
    stb r0, lbl_8047A631(r13)
    b @L_8007A4C8
@L_8007A42C:
    li r3, 0x43c2
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    li r0, 0x1
    stb r0, lbl_8047A630(r13)
    stb r0, lbl_8047A631(r13)
    stb r0, lbl_8047A632(r13)
    b @L_8007A4C8
@L_8007A450:
    li r3, 0xef
    bl fn_80102510
    li r3, 0x1
    bl fn_801069FC
    lfs f27, lbl_8047C114(r2)
    lfd f31, lbl_8047C118(r2)
    lis r30, 0x4330
    lfd f29, lbl_8047C120(r2)
    lfs f28, lbl_8047C108(r2)
    b @L_8007A4B0
@L_8007A478:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r30, 0x10(r1)
    stw r0, 0x14(r1)
    lfd f0, 0x10(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0xc(r1)
    stw r30, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fadds f27, f27, f0
@L_8007A4B0:
    fcmpo cr0, f27, f28
    blt @L_8007A478
    li r0, 0x1
    li r3, 0x0
    stw r0, lbl_8047A638(r13)
    b @L_8007A5A4
@L_8007A4C8:
    li r3, 0x43d1
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    bl fn_8001E184
    mr r30, r3
    li r3, 0x1
    bl fn_801069FC
    extsb r0, r30
    cmpwi r0, 0x0
    beq @L_8007A5A0
    bge @L_8007A504
    cmpwi r0, -0x1
    bge @L_8007A50C
    b @L_8007A5A0
@L_8007A504:
    cmpwi r0, 0x2
    bge @L_8007A5A0
@L_8007A50C:
    cmpwi r31, 0x0
    bne @L_8007A530
    li r3, 0x2
    li r4, 0x44cf
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
@L_8007A530:
    li r3, 0xef
    bl fn_80102510
    lfs f27, lbl_8047C114(r2)
    lfd f28, lbl_8047C118(r2)
    lis r31, 0x4330
    lfd f30, lbl_8047C120(r2)
    lfs f31, lbl_8047C108(r2)
    b @L_8007A588
@L_8007A550:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r31, 0x10(r1)
    stw r0, 0x14(r1)
    lfd f0, 0x10(r1)
    fsubs f29, f0, f28
    bl fn_800D3088
    stw r3, 0xc(r1)
    stw r31, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f30
    fdivs f0, f0, f29
    fadds f27, f27, f0
@L_8007A588:
    fcmpo cr0, f27, f31
    blt @L_8007A550
    li r0, 0x1
    li r3, 0x0
    stw r0, lbl_8047A638(r13)
    b @L_8007A5A4
@L_8007A5A0:
    li r3, 0x1
@L_8007A5A4:
    psq_l f31, 0x78(r1), 0, 0
    lfd f31, 0x70(r1)
    psq_l f30, 0x68(r1), 0, 0
    lfd f30, 0x60(r1)
    psq_l f29, 0x58(r1), 0, 0
    lfd f29, 0x50(r1)
    psq_l f28, 0x48(r1), 0, 0
    lfd f28, 0x40(r1)
    psq_l f27, 0x38(r1), 0, 0
    lfd f27, 0x30(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r0, 0x84(r1)
    lwz r29, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr

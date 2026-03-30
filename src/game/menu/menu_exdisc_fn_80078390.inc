    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stfd f31, 0x60(r1)
    psq_st f31, 0x68(r1), 0, 0
    stfd f30, 0x50(r1)
    psq_st f30, 0x58(r1), 0, 0
    stfd f29, 0x40(r1)
    psq_st f29, 0x48(r1), 0, 0
    stfd f28, 0x30(r1)
    psq_st f28, 0x38(r1), 0, 0
    stfd f27, 0x20(r1)
    psq_st f27, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    lis r3, lbl_803F6F18@ha
    addi r3, r3, lbl_803F6F18@l
    bl fn_8012AA2C
    lis r4, lbl_803F6F18@ha
    mr r31, r3
    addi r3, r4, lbl_803F6F18@l
    bl fn_8012AC54
    lis r4, lbl_803F6E40@ha
    addi r4, r4, lbl_803F6E40@l
    lwz r0, 0x8(r4)
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    beq @L_80078494
    mr r4, r3
    li r3, 0x4d
    bl fn_80132A38
    rlwinm r0, r31, 0, 24, 31
    cmpwi r0, 0x1
    beq @L_80078444
    bge @L_80078444
    cmpwi r0, 0x0
    bge @L_80078428
    b @L_80078444
@L_80078428:
    li r3, 0x1
    bl fn_80103CC0
    li r3, 0x43cb
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    b @L_8007845C
@L_80078444:
    li r3, 0x1
    bl fn_80103CC0
    li r3, 0x43cd
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
@L_8007845C:
    li r3, 0x1
    bl fn_801069FC
    li r3, 0x1
    bl fn_80103CC0
    li r3, 0x2
    li r4, 0x44cf
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r0, 0x0
    stw r0, lbl_8047A620(r13)
    b @L_8007887C
@L_80078494:
    mr r30, r3
    mr r4, r3
    li r3, 0x4d
    bl fn_80132A38
    rlwinm r0, r31, 0, 24, 31
    cmpwi r0, 0x1
    beq @L_800784DC
    bge @L_800784DC
    cmpwi r0, 0x0
    bge @L_800784C0
    b @L_800784DC
@L_800784C0:
    li r3, 0x1
    bl fn_80103CC0
    li r3, 0x43c7
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    b @L_800784F4
@L_800784DC:
    li r3, 0x1
    bl fn_80103CC0
    li r3, 0x43c9
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
@L_800784F4:
    li r3, 0x1
    bl fn_801069FC
    lfs f1, lbl_8047C100(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    lis r3, 0x104f
    li r4, 0x2
    addi r3, r3, 0x1000
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    li r3, 0x4c7
    bl fn_80166A28
    lfs f1, lbl_8047C100(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    lis r3, 0x104f
    li r4, 0x1
    addi r3, r3, 0x1000
    bl fn_801CB708
    lis r3, 0x104f
    li r4, 0x3
    addi r3, r3, 0x1000
    li r5, 0x0
    li r6, 0x1
    bl fn_801CB834
    mr r4, r30
    li r3, 0x4d
    bl fn_80132A38
    li r3, 0x1
    bl fn_80103CC0
    li r3, 0x4434
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    li r3, 0x1
    bl fn_801069FC
    lis r3, lbl_803F6E40@ha
    addi r3, r3, lbl_803F6E40@l
    lhz r0, 0xc(r3)
    cmplwi r0, 0x6
    bne @L_800786A0
    li r3, 0x1
    bl fn_80103CC0
    li r3, 0x43a6
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    li r3, 0x4c7
    bl fn_80166A28
    lis r3, 0x104f
    li r4, 0x4
    addi r3, r3, 0x1000
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    lis r3, 0x104f
    li r4, 0x1
    addi r3, r3, 0x1000
    bl fn_801CB708
    lfs f27, lbl_8047C0E0(r2)
    lfd f31, lbl_8047C0F0(r2)
    lis r31, 0x4330
    lfd f29, lbl_8047C0F8(r2)
    lfs f28, lbl_8047C0E4(r2)
    b @L_80078644
@L_8007860C:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r31, 0x8(r1)
    stw r0, 0xc(r1)
    lfd f0, 0x8(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0x14(r1)
    stw r31, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fadds f27, f27, f0
@L_80078644:
    fcmpo cr0, f27, f28
    blt @L_8007860C
    mr r4, r30
    li r3, 0x4d
    bl fn_80132A38
    li r3, 0x1
    bl fn_80103CC0
    li r3, 0x44b0
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    li r3, 0x1
    bl fn_80103CC0
    li r3, 0x2
    li r4, 0x44cf
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r0, 0x0
    stw r0, lbl_8047A620(r13)
    b @L_8007887C
@L_800786A0:
    li r3, 0x4c7
    bl fn_80166A28
    lis r3, 0x104f
    li r4, 0x5
    addi r3, r3, 0x1000
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    lis r3, 0x104f
    li r4, 0x1
    addi r3, r3, 0x1000
    bl fn_801CB708
    lfs f27, lbl_8047C0E0(r2)
    lfd f31, lbl_8047C0F0(r2)
    lis r31, 0x4330
    lfd f29, lbl_8047C0F8(r2)
    lfs f28, lbl_8047C0E4(r2)
    b @L_80078720
@L_800786E8:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r31, 0x10(r1)
    stw r0, 0x14(r1)
    lfd f0, 0x10(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0xc(r1)
    stw r31, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fadds f27, f27, f0
@L_80078720:
    fcmpo cr0, f27, f28
    blt @L_800786E8
    mr r4, r30
    li r3, 0x4d
    bl fn_80132A38
    li r3, 0x1
    bl fn_80103CC0
    li r3, 0x2
    li r4, 0x43b0
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_80103CC0
    li r3, 0x0
    li r4, -0x1
    li r5, -0x1
    li r6, 0x0
    bl fn_8001E074
    mr r31, r3
    li r3, 0x1
    bl fn_801069FC
    extsb r0, r31
    cmpwi r0, 0x0
    beq @L_80078874
    bge @L_80078794
    cmpwi r0, -0x1
    bge @L_8007879C
    b @L_80078874
@L_80078794:
    cmpwi r0, 0x2
    bge @L_80078874
@L_8007879C:
    li r3, 0x4c7
    bl fn_80166A28
    lis r3, 0x104f
    li r4, 0x4
    addi r3, r3, 0x1000
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    lis r3, 0x104f
    li r4, 0x1
    addi r3, r3, 0x1000
    bl fn_801CB708
    lfs f27, lbl_8047C0E0(r2)
    lfd f28, lbl_8047C0F0(r2)
    lis r31, 0x4330
    lfd f30, lbl_8047C0F8(r2)
    lfs f31, lbl_8047C0E4(r2)
    b @L_8007881C
@L_800787E4:
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
@L_8007881C:
    fcmpo cr0, f27, f31
    blt @L_800787E4
    li r3, 0x1
    bl fn_80103CC0
    li r3, 0x44b0
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    li r3, 0x1
    bl fn_801069FC
    li r3, 0x1
    bl fn_80103CC0
    li r3, 0x2
    li r4, 0x44cf
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r0, 0x0
    stw r0, lbl_8047A620(r13)
    b @L_8007887C
@L_80078874:
    li r0, 0xb
    stw r0, lbl_8047A620(r13)
@L_8007887C:
    psq_l f31, 0x68(r1), 0, 0
    lfd f31, 0x60(r1)
    psq_l f30, 0x58(r1), 0, 0
    lfd f30, 0x50(r1)
    psq_l f29, 0x48(r1), 0, 0
    lfd f29, 0x40(r1)
    psq_l f28, 0x38(r1), 0, 0
    lfd f28, 0x30(r1)
    psq_l f27, 0x28(r1), 0, 0
    lfd f27, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r0, 0x74(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr

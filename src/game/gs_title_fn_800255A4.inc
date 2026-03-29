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
    lwz r0, lbl_8047A384(r13)
    cmplwi r0, 0x0
    beq @L_8002566C
    lfs f1, lbl_8047B8E4(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x449
    li r4, 0x0
    bl fn_801657F8
    lfs f27, lbl_8047B8AC(r2)
    lfd f28, lbl_8047B8B8(r2)
    lis r31, 0x4330
    lfd f30, lbl_8047B8D0(r2)
    lfs f31, lbl_8047B8B0(r2)
    b @L_80025650
    @L_80025618:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r31, 0x8(r1)
    stw r0, 0xc(r1)
    lfd f0, 0x8(r1)
    fsubs f29, f0, f28
    bl fn_800D3088
    stw r3, 0x14(r1)
    stw r31, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f30
    fdivs f0, f0, f29
    fadds f27, f27, f0
    @L_80025650:
    fcmpo cr0, f27, f31
    blt @L_80025618
    bl fn_801653C4
    li r4, 0x7d0
    li r5, 0x0
    bl fn_801656F8
    b @L_80025688
    @L_8002566C:
    lfs f1, lbl_8047B8E8(r2)
    li r3, 0x3
    bl fn_801C41C8
    bl fn_801653C4
    li r4, 0x1f4
    li r5, 0x0
    bl fn_801656F8
    @L_80025688:
    li r3, 0x1
    bl fn_801C40F0
    li r3, 0xbd
    bl fn_80102620
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_800256AC
    li r3, 0xbd
    bl fn_80102510
    @L_800256AC:
    li r3, 0xc3
    bl fn_80102620
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_800256C8
    li r3, 0xc3
    bl fn_80102510
    @L_800256C8:
    lwz r0, lbl_8047A384(r13)
    cmplwi r0, 0x0
    beq @L_800256F4
    lwz r3, lbl_8047A388(r13)
    cmplwi r3, 0x0
    beq @L_800256E4
    bl fn_800EF5A4
    @L_800256E4:
    lwz r3, lbl_8047A38C(r13)
    cmplwi r3, 0x0
    beq @L_800256F4
    bl fn_800EF5A4
    @L_800256F4:
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
    lwz r0, 0x74(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr

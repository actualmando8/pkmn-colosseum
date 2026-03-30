    nofralloc
    cmplwi r3, 0x0
    stwu r1, -0x10(r1)
    beq @L_801A66D8
    subi r0, r4, 0x9
    cmplwi r0, 0xd
    bgt @L_801A66D8
    lis r4, jumptable_8036CAF4@ha
    slwi r0, r0, 2
    addi r4, r4, jumptable_8036CAF4@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    lfs f1, 0x0(r5)
    lfd f0, lbl_8047DC08(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801A64E8
    lhz r0, 0x8(r3)
    rlwinm r0, r0, 0, 27, 25
    sth r0, 0x8(r3)
    b @L_801A66D8
    @L_801A64E8:
    lhz r0, 0x8(r3)
    ori r0, r0, 0x20
    sth r0, 0x8(r3)
    b @L_801A66D8
    lhz r0, 0x8(r3)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A6510
    lfs f0, 0x0(r5)
    stfs f0, 0x20(r3)
    b @L_801A66D8
    @L_801A6510:
    lfs f0, 0x0(r5)
    stfs f0, 0x20(r3)
    b @L_801A66D8
    lhz r0, 0x8(r3)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A6534
    lfs f0, 0x0(r5)
    stfs f0, 0x24(r3)
    b @L_801A66D8
    @L_801A6534:
    lfs f0, 0x0(r5)
    stfs f0, 0x2c(r3)
    b @L_801A66D8
    lhz r0, 0x8(r3)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A6558
    lfs f0, 0x0(r5)
    stfs f0, 0x28(r3)
    b @L_801A66D8
    @L_801A6558:
    lfs f0, 0x0(r5)
    stfs f0, 0x28(r3)
    b @L_801A66D8
    lhz r0, 0x8(r3)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A66D8
    lfs f0, 0x0(r5)
    stfs f0, 0x2c(r3)
    b @L_801A66D8
    lhz r0, 0x8(r3)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A66D8
    lfs f0, 0x0(r5)
    stfs f0, 0x30(r3)
    b @L_801A66D8
    lhz r0, 0x8(r3)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A66D8
    lfs f0, 0x0(r5)
    stfs f0, 0x34(r3)
    b @L_801A66D8
    lfs f1, 0x0(r5)
    lfs f0, lbl_8047DBE8(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A65C8
    lfs f1, lbl_8047DBE8(r2)
    b @L_801A65DC
    @L_801A65C8:
    lfs f0, lbl_8047DBE4(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801A65DC
    lfs f1, lbl_8047DBE4(r2)
    @L_801A65DC:
    lfs f0, lbl_8047DC10(r2)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x10(r3)
    b @L_801A66D8
    lfs f1, 0x0(r5)
    lfs f0, lbl_8047DBE8(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A6614
    lfs f1, lbl_8047DBE8(r2)
    b @L_801A6628
    @L_801A6614:
    lfs f0, lbl_8047DBE4(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801A6628
    lfs f1, lbl_8047DBE4(r2)
    @L_801A6628:
    lfs f0, lbl_8047DC10(r2)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x11(r3)
    b @L_801A66D8
    lfs f1, 0x0(r5)
    lfs f0, lbl_8047DBE8(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A6660
    lfs f1, lbl_8047DBE8(r2)
    b @L_801A6674
    @L_801A6660:
    lfs f0, lbl_8047DBE4(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801A6674
    lfs f1, lbl_8047DBE4(r2)
    @L_801A6674:
    lfs f0, lbl_8047DC10(r2)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x12(r3)
    b @L_801A66D8
    lfs f1, 0x0(r5)
    lfs f0, lbl_8047DBE8(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A66AC
    lfs f1, lbl_8047DBE8(r2)
    b @L_801A66C0
    @L_801A66AC:
    lfs f0, lbl_8047DBE4(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801A66C0
    lfs f1, lbl_8047DBE4(r2)
    @L_801A66C0:
    lfs f0, lbl_8047DC10(r2)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x13(r3)
    @L_801A66D8:
    addi r1, r1, 0x10
    blr

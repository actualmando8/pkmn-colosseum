    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stfd f31, 0x80(r1)
    psq_st f31, 0x88(r1), 0, 0
    stmw r25, 0x64(r1)
    mr r29, r3
    bl fn_80115BD8
    mr r28, r3
    bl fn_80113F48
    lis r4, -32698
    mr r31, r3
    addi r26, r4, 29688
    lwz r4, -21536(r13)
    lbz r0, 0x8(r26)
    li r30, 0x0
    rlwinm r0, r0, 27, 0, 1
    srawi r0, r0, 31
    cmpw r4, r0
    beq @L_801D6F30
    bl fn_80113F48
    lwz r4, 0x8(r28)
    bl fn_800F9318
    lis r4, -32728
    addi r4, r4, -27524
    bl fn_80191ECC
    mr. r27, r3
    beq @L_801D7214
    lwz r0, 0x0(r27)
    cmplwi r0, 0x0
    beq @L_801D6F20
    lwz r3, 0x8(r28)
    bl fn_80115274
    mr r28, r3
    li r25, 0x0
    b @L_801D6F10
@L_801D6EE8:
    mr r3, r31
    or r4, r28, r30
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_801D6F08
    lwz r0, -21536(r13)
    rlwinm r4, r0, 0, 24, 31
    bl fn_800E4014
@L_801D6F08:
    addi r25, r25, 0x4
    addi r30, r30, 0x1
@L_801D6F10:
    lwz r3, 0x0(r27)
    lwzx r0, r3, r25
    cmplwi r0, 0x0
    bne @L_801D6EE8
@L_801D6F20:
    lbz r0, 0x8(r26)
    rlwinm r0, r0, 27, 0, 1
    srawi r0, r0, 31
    stw r0, -21536(r13)
@L_801D6F30:
    lwz r0, -21536(r13)
    cmpwi r0, 0x0
    beq @L_801D7214
    bl fn_800D2584
    lhz r4, -21440(r2)
    mr. r25, r3
    lbz r0, -21438(r2)
    sth r4, 0x8(r1)
    stb r0, 0xa(r1)
    beq @L_801D7214
    lwz r3, 0x24(r29)
    cmplwi r3, 0x0
    beq @L_801D7214
    bl fn_800E3C5C
    mr. r26, r3
    beq @L_801D7214
    bl fn_800D2248
    mr r3, r25
    addi r4, r1, 0x54
    bl fn_800D1F84
    mr r3, r25
    addi r4, r1, 0x3c
    addi r5, r1, 0x30
    bl fn_800D1EB8
    addi r3, r1, 0x48
    addi r4, r1, 0x30
    addi r5, r1, 0x54
    bl fn_800E0168
    mr r3, r26
    addi r4, r1, 0xc
    bl fn_80190E34
    addi r3, r1, 0x48
    bl fn_800E008C
    fmr f31, f1
    addi r3, r1, 0xc
    bl fn_800E008C
    fadds f31, f1, f31
    addi r3, r1, 0x48
    mr r4, r3
    bl fn_800E0060
    fmr f1, f31
    addi r3, r1, 0x24
    addi r4, r1, 0x48
    bl fn_800E013C
    addi r3, r1, 0x24
    addi r5, r1, 0x30
    mr r4, r3
    bl fn_800E019C
    addi r3, r1, 0x3c
    mr r4, r3
    bl fn_800E0060
    addi r3, r1, 0x18
    addi r4, r1, 0x48
    addi r5, r1, 0x3c
    bl fn_800DFFCC
    addi r3, r1, 0x18
    mr r4, r3
    bl fn_800E0060
    addi r3, r1, 0x3c
    addi r4, r1, 0x18
    addi r5, r1, 0x48
    bl fn_800DFFCC
    lfs f0, -21424(r2)
    addi r3, r1, 0x18
    mr r4, r3
    fmuls f1, f0, f31
    bl fn_800E013C
    fmr f1, f31
    addi r3, r1, 0x3c
    mr r4, r3
    bl fn_800E013C
    li r3, 0x0
    li r4, 0x1
    li r5, 0x0
    bl fn_800DA4C4
    li r3, 0x1
    li r4, 0x0
    li r5, 0x1
    bl fn_800DA2BC
    li r3, 0x1
    li r4, 0x2
    li r5, 0x1
    bl fn_800DA1E8
    li r3, 0x0
    bl fn_800D9ED8
    li r3, 0x0
    bl fn_800DA028
    li r3, 0x1
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    lis r3, -32719
    addi r3, r3, 18776
    bl fn_800D7820
    li r3, 0x6
    bl fn_800D6A00
    li r3, 0x4
    bl fn_800D67BC
    lfs f3, 0x24(r1)
    lfs f2, 0x18(r1)
    lfs f1, 0x28(r1)
    lfs f0, 0x1c(r1)
    fadds f3, f3, f2
    lfs f5, 0x3c(r1)
    lfs f2, 0x2c(r1)
    fadds f4, f1, f0
    lfs f0, 0x20(r1)
    fadds f1, f5, f3
    lfs f5, 0x40(r1)
    fadds f0, f2, f0
    lfs f3, 0x44(r1)
    fadds f2, f5, f4
    fadds f3, f3, f0
    bl fn_800D6680
    lbz r26, 0x9(r1)
    li r3, 0x0
    lbz r25, 0xa(r1)
    lbz r4, 0x8(r1)
    mr r5, r26
    mr r6, r25
    bl fn_800D5C18
    lfs f3, 0x24(r1)
    lfs f2, 0x18(r1)
    lfs f1, 0x28(r1)
    lfs f0, 0x1c(r1)
    fsubs f3, f3, f2
    lfs f5, 0x3c(r1)
    lfs f2, 0x2c(r1)
    fsubs f4, f1, f0
    lfs f0, 0x20(r1)
    fadds f1, f5, f3
    lfs f5, 0x40(r1)
    fsubs f0, f2, f0
    lfs f3, 0x44(r1)
    fadds f2, f5, f4
    fadds f3, f3, f0
    bl fn_800D6680
    lbz r4, 0x8(r1)
    mr r5, r26
    mr r6, r25
    li r3, 0x0
    bl fn_800D5C18
    lfs f3, 0x24(r1)
    lfs f2, 0x18(r1)
    lfs f1, 0x28(r1)
    lfs f0, 0x1c(r1)
    fsubs f4, f3, f2
    lfs f3, 0x3c(r1)
    lfs f2, 0x2c(r1)
    fsubs f5, f1, f0
    lfs f0, 0x20(r1)
    fsubs f1, f4, f3
    lfs f4, 0x40(r1)
    fsubs f3, f2, f0
    lfs f0, 0x44(r1)
    fsubs f2, f5, f4
    fsubs f3, f3, f0
    bl fn_800D6680
    lbz r4, 0x8(r1)
    mr r5, r26
    mr r6, r25
    li r3, 0x0
    bl fn_800D5C18
    lfs f3, 0x24(r1)
    lfs f2, 0x18(r1)
    lfs f1, 0x28(r1)
    lfs f0, 0x1c(r1)
    fadds f4, f3, f2
    lfs f3, 0x3c(r1)
    lfs f2, 0x2c(r1)
    fadds f5, f1, f0
    lfs f0, 0x20(r1)
    fsubs f1, f4, f3
    lfs f4, 0x40(r1)
    fadds f3, f2, f0
    lfs f0, 0x44(r1)
    fsubs f2, f5, f4
    fsubs f3, f3, f0
    bl fn_800D6680
    lbz r4, 0x8(r1)
    mr r5, r26
    mr r6, r25
    li r3, 0x0
    bl fn_800D5C18
    bl fn_800D6728
@L_801D7214:
    psq_l f31, 0x88(r1), 0, 0
    lfd f31, 0x80(r1)
    lmw r25, 0x64(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr

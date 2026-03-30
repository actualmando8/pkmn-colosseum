    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r27, 0x2c(r1)
    mr r29, r4
    mr r27, r5
    mr r28, r3
    li r30, 0x0
    mr r31, r29
    li r4, 0x0
    li r5, 0xac
    bl memset
    lwz r0, 0x3c(r31)
    cmpwi r0, 0x2
    beq @L_801373B8
    bge @L_801373B8
    cmpwi r0, 0x1
    bge @L_801373A8
    b @L_801373B8
@L_801373A8:
    li r0, 0x0
    li r30, -0x4
    stw r0, 0xa8(r28)
    b @L_801373C0
@L_801373B8:
    lwz r0, 0x40(r31)
    stw r0, 0xa8(r28)
@L_801373C0:
    bl fn_800D37CC
    lis r4, 0x4330
    xoris r3, r3, 0x8000
    stw r3, 0xc(r1)
    xoris r0, r27, 0x8000
    lfd f3, -25976(r2)
    stw r4, 0x8(r1)
    lfs f0, -25992(r2)
    lfd f1, 0x8(r1)
    stw r0, 0x14(r1)
    fsubs f2, f1, f3
    stw r4, 0x10(r1)
    lfd f1, 0x10(r1)
    fsubs f1, f1, f3
    fmuls f1, f1, f2
    fdivs f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x18(r1)
    lwz r0, 0x1c(r1)
    sth r0, 0xa6(r28)
    lfs f0, 0x0(r31)
    stfs f0, 0x48(r28)
    lfs f0, 0x4(r31)
    stfs f0, 0x4c(r28)
    lfs f0, 0x8(r31)
    stfs f0, 0x50(r28)
    lwz r0, 0xc(r31)
    srawi r0, r0, 24
    stb r0, 0x63(r28)
    lwz r0, 0xc(r31)
    rlwinm r0, r0, 16, 24, 31
    stb r0, 0x62(r28)
    lwz r0, 0xc(r31)
    rlwinm r0, r0, 24, 24, 31
    stb r0, 0x61(r28)
    lwz r0, 0xc(r31)
    stb r0, 0x60(r28)
    lfs f0, 0x10(r31)
    stfs f0, 0x64(r28)
    lfs f0, 0x14(r31)
    stfs f0, 0x68(r28)
    lfs f0, 0x18(r31)
    stfs f0, 0x6c(r28)
    lwz r0, 0x1c(r31)
    sth r0, 0x70(r28)
    lwz r0, 0x20(r31)
    sth r0, 0x72(r28)
    lhz r4, 0x70(r28)
    rlwinm r3, r4, 1, 31, 31
    rlwinm r0, r4, 0, 31, 31
    xor r0, r0, r3
    subf. r0, r3, r0
    bne @L_8013749C
    addi r0, r4, 0x1
    sth r0, 0x70(r28)
@L_8013749C:
    lhz r4, 0x72(r28)
    rlwinm r3, r4, 1, 31, 31
    rlwinm r0, r4, 0, 31, 31
    xor r0, r0, r3
    subf. r0, r3, r0
    bne @L_801374BC
    addi r0, r4, 0x1
    sth r0, 0x72(r28)
@L_801374BC:
    lfs f0, 0x24(r31)
    add r29, r30, r29
    addi r3, r29, 0x63
    li r0, 0x4e20
    stfs f0, 0x90(r28)
    rlwinm r29, r3, 0, 0, 26
    lfs f0, 0x28(r31)
    stfs f0, 0x94(r28)
    lfs f0, 0x2c(r31)
    stfs f0, 0x98(r28)
    lfs f0, 0x30(r31)
    stfs f0, 0x9c(r28)
    lfs f0, 0x34(r31)
    stfs f0, 0xa0(r28)
    stw r0, 0x74(r28)
    bl fn_801DB060
    stw r3, 0x7c(r28)
    bl fn_801DB060
    stw r3, 0x78(r28)
    mr r3, r29
    li r5, 0x4e20
    lwz r4, 0x38(r31)
    lwz r6, 0x7c(r28)
    bl fn_8010147C
    lwz r4, 0x7c(r28)
    li r3, 0x4e20
    bl fn_800F9318
    lwz r6, 0x78(r28)
    li r4, 0x4e20
    li r5, 0x0
    bl fn_801013A0
    lwz r4, 0x78(r28)
    li r3, 0x4e20
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_80137554
    li r4, 0x0
    bl fn_800E4014
@L_80137554:
    lwz r3, 0x38(r31)
    addi r0, r3, 0x1f
    rlwinm r0, r0, 0, 0, 26
    add r29, r29, r0
    mr r3, r29
    lmw r27, 0x2c(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr

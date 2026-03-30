    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r27, 0x2c(r1)
    mr r30, r4
    mr r27, r5
    mr r29, r3
    li r4, 0x0
    mr r31, r30
    li r5, 0x70
    bl memset
    bl fn_800D37CC
    lis r5, 0x4330
    xoris r3, r3, 0x8000
    stw r3, 0xc(r1)
    xoris r0, r27, 0x8000
    lfd f3, -25976(r2)
    addi r3, r29, 0x34
    stw r5, 0x8(r1)
    addi r4, r31, 0xc
    lfs f0, -25992(r2)
    lfd f1, 0x8(r1)
    stw r0, 0x14(r1)
    fsubs f2, f1, f3
    stw r5, 0x10(r1)
    lfd f1, 0x10(r1)
    fsubs f1, f1, f3
    fmuls f1, f1, f2
    fdivs f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x18(r1)
    lwz r0, 0x1c(r1)
    sth r0, 0x12(r29)
    lwz r0, 0x0(r31)
    rlwinm r0, r0, 8, 24, 31
    stb r0, 0x23(r29)
    lwz r0, 0x0(r31)
    rlwinm r0, r0, 16, 24, 31
    stb r0, 0x22(r29)
    lwz r0, 0x0(r31)
    rlwinm r0, r0, 24, 24, 31
    stb r0, 0x21(r29)
    lwz r0, 0x0(r31)
    stb r0, 0x20(r29)
    lwz r0, 0x4(r31)
    rlwinm r0, r0, 8, 24, 31
    stb r0, 0x27(r29)
    lwz r0, 0x4(r31)
    rlwinm r0, r0, 16, 24, 31
    stb r0, 0x26(r29)
    lwz r0, 0x4(r31)
    rlwinm r0, r0, 24, 24, 31
    stb r0, 0x25(r29)
    lwz r0, 0x4(r31)
    stb r0, 0x24(r29)
    bl fn_800E01D0
    addi r3, r29, 0x40
    addi r4, r31, 0x18
    bl fn_800E01D0
    lwz r3, 0x8(r31)
    addi r0, r30, 0x73
    rlwinm r30, r0, 0, 0, 26
    li r4, 0x20
    sth r3, 0x8(r29)
    lfs f0, 0x24(r31)
    stfs f0, 0x4c(r29)
    lfs f0, 0x28(r31)
    stfs f0, 0x50(r29)
    lfs f0, 0x2c(r31)
    stfs f0, 0x54(r29)
    lfs f0, 0x30(r31)
    stfs f0, 0x58(r29)
    lfs f0, 0x34(r31)
    stfs f0, 0x5c(r29)
    lfs f0, 0x38(r31)
    stfs f0, 0x60(r29)
    lfs f0, 0x3c(r31)
    stfs f0, 0x64(r29)
    lfs f0, 0x40(r31)
    stfs f0, 0x68(r29)
    lfs f0, 0x44(r31)
    stfs f0, 0x6c(r29)
    lwz r0, 0x48(r31)
    sth r0, 0xe(r29)
    lwz r3, 0x4c(r31)
    addi r0, r3, 0x1f
    rlwinm r3, r0, 0, 0, 26
    bl fn_800E2C04
    rlwinm. r0, r3, 0, 16, 31
    mr r28, r3
    beq @L_80137720
    bl fn_800E27B0
    lwz r5, 0x4c(r31)
    mr r27, r3
    mr r4, r30
    addi r0, r5, 0x1f
    rlwinm r5, r0, 0, 0, 26
    bl memcpy
    mr r3, r27
    bl fn_800EFD3C
    stw r3, 0x1c(r29)
    mr r4, r28
    lwz r3, 0x1c(r29)
    bl fn_800EFD14
    b @L_80137728
@L_80137720:
    li r0, 0x0
    stw r0, 0x1c(r29)
@L_80137728:
    lwz r4, 0x4c(r31)
    lwz r3, 0x1c(r29)
    addi r0, r4, 0x1f
    rlwinm r0, r0, 0, 0, 26
    cmplwi r3, 0x0
    add r30, r30, r0
    beq @L_80137750
    li r4, 0x2
    li r5, 0x2
    bl fn_800EF590
@L_80137750:
    lis r3, -32719
    lis r4, -32719
    addi r0, r3, 17976
    mr r3, r30
    stw r0, 0x14(r29)
    addi r0, r4, 19176
    stw r0, 0x18(r29)
    lmw r27, 0x2c(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr

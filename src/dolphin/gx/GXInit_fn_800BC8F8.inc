    nofralloc
    mflr r0
    li r10, 0x0
    stw r0, 0x4(r1)
    li r11, 0x0
    stwu r1, -0x48(r1)
    stw r31, 0x44(r1)
    rlwinm. r31, r3, 29, 31, 31
    stw r30, 0x40(r1)
    rlwinm r30, r3, 0, 29, 31
    stw r29, 0x3c(r1)
    addi r29, r4, 0x0
    stw r28, 0x38(r1)
    beq @L_800BC974
    fcmpu cr0, f4, f3
    beq @L_800BC93C
    fcmpu cr0, f2, f1
    bne @L_800BC94C
@L_800BC93C:
    lfs f0, -29464(r2)
    stfs f0, 0x2c(r1)
    stfs f0, 0x28(r1)
    b @L_800BCA48
@L_800BC94C:
    fsubs f5, f2, f1
    lfs f6, -29460(r2)
    fsubs f2, f4, f3
    fsubs f0, f1, f3
    fdivs f3, f6, f5
    fmuls f1, f3, f2
    fmuls f0, f3, f0
    stfs f1, 0x2c(r1)
    stfs f0, 0x28(r1)
    b @L_800BCA48
@L_800BC974:
    fcmpu cr0, f4, f3
    beq @L_800BC984
    fcmpu cr0, f2, f1
    bne @L_800BC994
@L_800BC984:
    lfs f3, -29464(r2)
    lfs f4, -29456(r2)
    fmr f5, f3
    b @L_800BC9B0
@L_800BC994:
    fsubs f0, f4, f3
    fsubs f5, f2, f1
    fmuls f2, f4, f3
    fdivs f4, f4, f0
    fmuls f0, f0, f5
    fdivs f5, f1, f5
    fdivs f3, f2, f0
@L_800BC9B0:
    lfs f1, -29456(r2)
    li r3, 0x0
    lfd f0, -29448(r2)
    b @L_800BC9C8
@L_800BC9C0:
    fmuls f4, f4, f1
    addi r3, r3, 0x1
@L_800BC9C8:
    fcmpo cr0, f4, f0
    bgt @L_800BC9C0
    lfd f0, -29432(r2)
    lfs f2, -29440(r2)
    lfs f1, -29464(r2)
    b @L_800BC9E8
@L_800BC9E0:
    fmuls f4, f4, f2
    subi r3, r3, 0x1
@L_800BC9E8:
    fcmpo cr0, f4, f1
    ble @L_800BC9F8
    fcmpo cr0, f4, f0
    blt @L_800BC9E0
@L_800BC9F8:
    addi r28, r3, 0x1
    lfs f0, -29424(r2)
    li r0, 0x1
    lfd f2, -29416(r2)
    slw r0, r0, r28
    fmuls f1, f0, f4
    xoris r3, r0, 0x8000
    stfs f5, 0x28(r1)
    lis r0, 0x4330
    stw r3, 0x34(r1)
    stw r0, 0x30(r1)
    lfd f0, 0x30(r1)
    fsubs f0, f0, f2
    fdivs f0, f3, f0
    stfs f0, 0x2c(r1)
    bl fn_800C46B0
    rlwinm r3, r3, 0, 8, 31
    rlwinm r0, r28, 0, 8, 31
    oris r10, r3, 0xef00
    oris r11, r0, 0xf000
@L_800BCA48:
    lwz r7, 0x2c(r1)
    li r0, 0x61
    lbz r6, 0x1(r29)
    lis r4, 0xcc01
    rlwinm r8, r7, 20, 13, 20
    rlwimi r8, r7, 20, 21, 31
    lbz r9, 0x2(r29)
    lbz r5, 0x0(r29)
    rlwinm r7, r7, 20, 12, 12
    rlwimi r7, r8, 0, 13, 31
    lwz r12, 0x28(r1)
    rlwinm r7, r7, 0, 8, 31
    stb r0, -0x8000(r4)
    oris r7, r7, 0xee00
    rlwinm r6, r6, 8, 0, 23
    lwz r3, -29632(r13)
    stw r7, -0x8000(r4)
    rlwinm r7, r12, 20, 13, 20
    stb r0, -0x8000(r4)
    rlwimi r7, r12, 20, 21, 31
    rlwinm r8, r12, 20, 12, 12
    stw r10, -0x8000(r4)
    rlwimi r8, r7, 0, 13, 31
    rlwinm r7, r31, 20, 0, 11
    stb r0, -0x8000(r4)
    rlwimi r7, r8, 0, 12, 31
    rlwinm r8, r7, 0, 11, 7
    stw r11, -0x8000(r4)
    rlwinm r7, r30, 21, 0, 10
    or r7, r8, r7
    rlwinm r7, r7, 0, 8, 31
    stb r0, -0x8000(r4)
    oris r7, r7, 0xf100
    stw r7, -0x8000(r4)
    rlwimi r6, r9, 0, 24, 31
    stb r0, -0x8000(r4)
    rlwinm r0, r5, 16, 0, 15
    rlwimi r0, r6, 0, 16, 31
    rlwinm r0, r0, 0, 8, 31
    oris r0, r0, 0xf200
    stw r0, -0x8000(r4)
    li r0, 0x0
    sth r0, 0x2(r3)
    lwz r0, 0x4c(r1)
    lwz r31, 0x44(r1)
    lwz r30, 0x40(r1)
    lwz r29, 0x3c(r1)
    lwz r28, 0x38(r1)
    addi r1, r1, 0x48
    mtlr r0
    blr

    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r5
    stw r30, 0x18(r1)
    mr r30, r3
    stw r29, 0x14(r1)
    lwz r3, 0x0(r3)
    rlwinm. r0, r3, 0, 29, 29
    beq @L_800EC8AC
    lwz r0, 0x84(r30)
    cmplw r4, r0
    bgt @L_800EC8AC
    cmplw r31, r0
    ble @L_800EC640
    b @L_800EC8AC
    @L_800EC640:
    ori r0, r3, 0x80
    li r5, 0x0
    stw r0, 0x0(r30)
    li r6, 0x0
    stw r4, 0xbc(r30)
    stw r31, 0xc0(r30)
    lwz r3, 0x4(r30)
    lwz r0, 0xbc(r30)
    lwz r4, 0x4(r3)
    slwi r0, r0, 2
    lwz r3, 0x10(r30)
    lwzx r4, r4, r0
    bl fn_801A2B5C
    lwz r3, 0x4(r30)
    li r5, 0x0
    lwz r0, 0xc0(r30)
    li r6, 0x0
    lwz r4, 0x4(r3)
    slwi r0, r0, 2
    lwz r3, 0x14(r30)
    lwzx r4, r4, r0
    bl fn_801A2B5C
    lfs f0, lbl_8047CC5C(r2)
    li r4, 0x1cb
    stfs f0, 0xd4(r30)
    stfs f0, 0xc4(r30)
    stfs f0, 0xc8(r30)
    lwz r3, 0x10(r30)
    lfs f1, 0xc4(r30)
    bl fn_801A32A0
    lwz r3, 0x14(r30)
    li r4, 0x1cb
    lfs f1, 0xc8(r30)
    bl fn_801A32A0
    lwz r3, 0x10(r30)
    bl fn_801A1B40
    lwz r3, 0x14(r30)
    bl fn_801A1B40
    lfs f0, lbl_8047CC5C(r2)
    lis r3, fn_800EE08C@ha
    addi r6, r3, fn_800EE08C@l
    addi r8, r30, 0xcc
    stfs f0, 0xcc(r30)
    lis r3, 0x1
    subi r5, r3, 0x64d1
    li r4, 0x6
    lwz r3, 0x10(r30)
    li r7, 0x2
    crxor 6, 6, 6
    bl fn_801C028C
    lfs f0, lbl_8047CC5C(r2)
    lis r3, fn_800EE08C@ha
    addi r6, r3, fn_800EE08C@l
    addi r8, r30, 0xd0
    stfs f0, 0xd0(r30)
    lis r3, 0x1
    subi r5, r3, 0x64d1
    li r4, 0x6
    lwz r3, 0x14(r30)
    li r7, 0x2
    crxor 6, 6, 6
    bl fn_801C028C
    lfs f2, lbl_8047CC60(r2)
    lfs f1, 0xcc(r30)
    lfs f0, 0xd0(r30)
    fadds f1, f2, f1
    fadds f0, f2, f0
    fdivs f0, f1, f0
    stfs f0, 0xd8(r30)
    lwz r3, 0x0(r30)
    rlwinm. r0, r3, 0, 18, 18
    beq @L_800EC8A0
    rlwinm. r0, r3, 0, 28, 28
    lwz r29, 0x8(r30)
    beq @L_800EC8A0
    lwz r0, 0x88(r30)
    cmplw r31, r0
    bge @L_800EC8A0
    lwz r0, 0xa8(r30)
    cmplw r31, r0
    beq @L_800EC7E4
    rlwinm. r0, r3, 0, 14, 14
    beq @L_800EC790
    lwz r29, 0x10(r29)
    @L_800EC790:
    stw r31, 0xa8(r30)
    mr r3, r29
    li r4, 0x0
    li r6, 0x0
    lwz r5, 0x4(r30)
    lwz r0, 0xa8(r30)
    lwz r5, 0x8(r5)
    slwi r0, r0, 2
    lwzx r5, r5, r0
    bl fn_801A2B5C
    lfs f0, lbl_8047CC5C(r2)
    lis r3, fn_800EE08C@ha
    addi r6, r3, fn_800EE08C@l
    addi r8, r30, 0xb8
    stfs f0, 0xb8(r30)
    mr r3, r29
    li r4, 0x6
    li r5, 0x64db
    li r7, 0x2
    crxor 6, 6, 6
    bl fn_801C028C
    @L_800EC7E4:
    lwz r4, 0x0(r30)
    lwz r3, 0x8(r30)
    rlwinm. r0, r4, 0, 28, 28
    beq @L_800EC824
    rlwinm. r0, r4, 0, 14, 14
    beq @L_800EC800
    lwz r3, 0x10(r3)
    @L_800EC800:
    lfs f0, lbl_8047CC5C(r2)
    li r4, 0x634
    stfs f0, 0xb0(r30)
    stfs f0, 0xb4(r30)
    lfs f1, 0xb4(r30)
    bl fn_801A32A0
    lwz r0, 0x0(r30)
    rlwinm r0, r0, 0, 17, 15
    stw r0, 0x0(r30)
    @L_800EC824:
    lwz r0, 0x0(r30)
    lwz r4, 0xa4(r30)
    rlwinm. r0, r0, 0, 14, 14
    lwz r3, 0x8(r30)
    beq @L_800EC83C
    lwz r3, 0x10(r3)
    @L_800EC83C:
    stw r4, 0xa4(r30)
    lwz r0, 0xa4(r30)
    cmpwi r0, 0x1
    beq @L_800EC880
    bge @L_800EC8A0
    cmpwi r0, 0x0
    bge @L_800EC85C
    b @L_800EC8A0
    @L_800EC85C:
    lis r4, fn_800EE054@ha
    li r5, 0x64db
    addi r6, r4, fn_800EE054@l
    li r7, 0x3
    li r4, 0x6
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801C028C
    b @L_800EC8A0
    @L_800EC880:
    lis r4, fn_800EE054@ha
    li r5, 0x64db
    addi r6, r4, fn_800EE054@l
    li r7, 0x3
    li r4, 0x6
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801C028C
    @L_800EC8A0:
    lwz r0, 0x0(r30)
    rlwinm r0, r0, 0, 20, 18
    stw r0, 0x0(r30)
    @L_800EC8AC:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

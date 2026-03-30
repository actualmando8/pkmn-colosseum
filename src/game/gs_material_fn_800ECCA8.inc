    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r4
    stw r29, 0x14(r1)
    mr r29, r3
    lwz r3, 0x0(r3)
    lwz r31, 0x8(r29)
    rlwinm. r0, r3, 0, 29, 29
    beq @L_800ED0B4
    lwz r0, 0x84(r29)
    cmplw r30, r0
    bge @L_800ED0B4
    lwz r0, 0x90(r29)
    cmplw r30, r0
    bne @L_800ECCF8
    rlwinm. r0, r3, 0, 24, 24
    beq @L_800ECD8C
    @L_800ECCF8:
    rlwinm. r0, r3, 0, 14, 14
    beq @L_800ECD04
    lwz r31, 0x10(r31)
    @L_800ECD04:
    lwz r0, 0x0(r29)
    rlwinm r0, r0, 0, 25, 23
    stw r0, 0x0(r29)
    stw r30, 0x90(r29)
    lwz r0, 0x0(r29)
    rlwinm. r0, r0, 0, 27, 27
    beq @L_800ECD38
    lwz r3, 0x4(r29)
    lwz r0, 0x90(r29)
    lwz r3, 0xc(r3)
    slwi r0, r0, 2
    lwzx r6, r3, r0
    b @L_800ECD3C
    @L_800ECD38:
    li r6, 0x0
    @L_800ECD3C:
    lwz r4, 0x4(r29)
    mr r3, r31
    lwz r0, 0x90(r29)
    li r5, 0x0
    lwz r4, 0x4(r4)
    slwi r0, r0, 2
    lwzx r4, r4, r0
    bl fn_801A2B5C
    lfs f0, lbl_8047CC5C(r2)
    lis r3, fn_800EE08C@ha
    lis r4, 0x1
    addi r8, r29, 0xa0
    stfs f0, 0xa0(r29)
    addi r6, r3, fn_800EE08C@l
    subi r5, r4, 0x64d1
    mr r3, r31
    li r4, 0x6
    li r7, 0x2
    crxor 6, 6, 6
    bl fn_801C028C
    @L_800ECD8C:
    lwz r4, 0x0(r29)
    lwz r3, 0x8(r29)
    rlwinm. r0, r4, 0, 29, 29
    beq @L_800ECE58
    rlwinm. r0, r4, 0, 24, 24
    bne @L_800ECDCC
    rlwinm. r0, r4, 0, 14, 14
    beq @L_800ECDB0
    lwz r3, 0x10(r3)
    @L_800ECDB0:
    lfs f0, lbl_8047CC5C(r2)
    li r4, 0x1cb
    stfs f0, 0x98(r29)
    stfs f0, 0x9c(r29)
    lfs f1, 0x9c(r29)
    bl fn_801A32A0
    b @L_800ECE00
    @L_800ECDCC:
    lfs f1, lbl_8047CC5C(r2)
    li r4, 0x1cb
    lfs f0, 0xd8(r29)
    fmuls f0, f1, f0
    stfs f0, 0xc4(r29)
    stfs f1, 0xc8(r29)
    lwz r3, 0x10(r29)
    lfs f1, 0xc4(r29)
    bl fn_801A32A0
    lwz r3, 0x14(r29)
    li r4, 0x1cb
    lfs f1, 0xc8(r29)
    bl fn_801A32A0
    @L_800ECE00:
    lwz r3, 0x0(r29)
    li r0, -0x5001
    and r0, r3, r0
    stw r0, 0x0(r29)
    lwz r4, 0x0(r29)
    rlwinm. r0, r4, 0, 18, 18
    beq @L_800ECE58
    rlwinm. r0, r4, 0, 28, 28
    lwz r3, 0x8(r29)
    beq @L_800ECE58
    rlwinm. r0, r4, 0, 14, 14
    beq @L_800ECE34
    lwz r3, 0x10(r3)
    @L_800ECE34:
    lfs f0, lbl_8047CC5C(r2)
    li r4, 0x634
    stfs f0, 0xb0(r29)
    stfs f0, 0xb4(r29)
    lfs f1, 0xb4(r29)
    bl fn_801A32A0
    lwz r0, 0x0(r29)
    rlwinm r0, r0, 0, 17, 15
    stw r0, 0x0(r29)
    @L_800ECE58:
    lwz r0, 0x0(r29)
    lwz r31, 0x8c(r29)
    rlwinm. r0, r0, 0, 14, 14
    lwz r3, 0x8(r29)
    beq @L_800ECE70
    lwz r3, 0x10(r3)
    @L_800ECE70:
    stw r31, 0x8c(r29)
    lwz r0, 0x8c(r29)
    cmpwi r0, 0x1
    beq @L_800ECEB8
    bge @L_800ECEDC
    cmpwi r0, 0x0
    bge @L_800ECE90
    b @L_800ECEDC
    @L_800ECE90:
    lis r4, fn_800EE054@ha
    lis r5, 0x1
    addi r6, r4, fn_800EE054@l
    li r7, 0x3
    subi r5, r5, 0x64d1
    li r4, 0x6
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801C028C
    b @L_800ECEDC
    @L_800ECEB8:
    lis r4, fn_800EE054@ha
    lis r5, 0x1
    addi r6, r4, fn_800EE054@l
    li r7, 0x3
    subi r5, r5, 0x64d1
    li r4, 0x6
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801C028C
    @L_800ECEDC:
    lwz r3, 0x0(r29)
    rlwinm. r0, r3, 0, 18, 18
    beq @L_800ECF5C
    rlwinm. r0, r3, 0, 14, 14
    lwz r3, 0x8(r29)
    beq @L_800ECEF8
    lwz r3, 0x10(r3)
    @L_800ECEF8:
    stw r31, 0xa4(r29)
    lwz r0, 0xa4(r29)
    cmpwi r0, 0x1
    beq @L_800ECF3C
    bge @L_800ECF5C
    cmpwi r0, 0x0
    bge @L_800ECF18
    b @L_800ECF5C
    @L_800ECF18:
    lis r4, fn_800EE054@ha
    li r5, 0x64db
    addi r6, r4, fn_800EE054@l
    li r7, 0x3
    li r4, 0x6
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801C028C
    b @L_800ECF5C
    @L_800ECF3C:
    lis r4, fn_800EE054@ha
    li r5, 0x64db
    addi r6, r4, fn_800EE054@l
    li r7, 0x3
    li r4, 0x6
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801C028C
    @L_800ECF5C:
    lwz r3, 0x0(r29)
    rlwinm. r0, r3, 0, 18, 18
    beq @L_800ED0A8
    rlwinm. r0, r3, 0, 28, 28
    lwz r31, 0x8(r29)
    beq @L_800ED0A8
    lwz r0, 0x88(r29)
    cmplw r30, r0
    bge @L_800ED0A8
    lwz r0, 0xa8(r29)
    cmplw r30, r0
    beq @L_800ECFEC
    rlwinm. r0, r3, 0, 14, 14
    beq @L_800ECF98
    lwz r31, 0x10(r31)
    @L_800ECF98:
    stw r30, 0xa8(r29)
    mr r3, r31
    li r4, 0x0
    li r6, 0x0
    lwz r5, 0x4(r29)
    lwz r0, 0xa8(r29)
    lwz r5, 0x8(r5)
    slwi r0, r0, 2
    lwzx r5, r5, r0
    bl fn_801A2B5C
    lfs f0, lbl_8047CC5C(r2)
    lis r3, fn_800EE08C@ha
    addi r6, r3, fn_800EE08C@l
    addi r8, r29, 0xb8
    stfs f0, 0xb8(r29)
    mr r3, r31
    li r4, 0x6
    li r5, 0x64db
    li r7, 0x2
    crxor 6, 6, 6
    bl fn_801C028C
    @L_800ECFEC:
    lwz r4, 0x0(r29)
    lwz r3, 0x8(r29)
    rlwinm. r0, r4, 0, 28, 28
    beq @L_800ED02C
    rlwinm. r0, r4, 0, 14, 14
    beq @L_800ED008
    lwz r3, 0x10(r3)
    @L_800ED008:
    lfs f0, lbl_8047CC5C(r2)
    li r4, 0x634
    stfs f0, 0xb0(r29)
    stfs f0, 0xb4(r29)
    lfs f1, 0xb4(r29)
    bl fn_801A32A0
    lwz r0, 0x0(r29)
    rlwinm r0, r0, 0, 17, 15
    stw r0, 0x0(r29)
    @L_800ED02C:
    lwz r0, 0x0(r29)
    lwz r4, 0xa4(r29)
    rlwinm. r0, r0, 0, 14, 14
    lwz r3, 0x8(r29)
    beq @L_800ED044
    lwz r3, 0x10(r3)
    @L_800ED044:
    stw r4, 0xa4(r29)
    lwz r0, 0xa4(r29)
    cmpwi r0, 0x1
    beq @L_800ED088
    bge @L_800ED0A8
    cmpwi r0, 0x0
    bge @L_800ED064
    b @L_800ED0A8
    @L_800ED064:
    lis r4, fn_800EE054@ha
    li r5, 0x64db
    addi r6, r4, fn_800EE054@l
    li r7, 0x3
    li r4, 0x6
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801C028C
    b @L_800ED0A8
    @L_800ED088:
    lis r4, fn_800EE054@ha
    li r5, 0x64db
    addi r6, r4, fn_800EE054@l
    li r7, 0x3
    li r4, 0x6
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801C028C
    @L_800ED0A8:
    lwz r0, 0x0(r29)
    rlwinm r0, r0, 0, 20, 18
    stw r0, 0x0(r29)
    @L_800ED0B4:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

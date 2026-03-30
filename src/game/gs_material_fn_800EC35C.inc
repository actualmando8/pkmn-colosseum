    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    stw r30, 0x8(r1)
    lwz r3, 0x0(r3)
    lwz r30, 0x8(r31)
    rlwinm. r0, r3, 0, 28, 28
    beq @L_800EC4B8
    lwz r0, 0x88(r31)
    cmplw r4, r0
    bge @L_800EC4B8
    lwz r0, 0xa8(r31)
    cmplw r4, r0
    beq @L_800EC3FC
    rlwinm. r0, r3, 0, 14, 14
    beq @L_800EC3A8
    lwz r30, 0x10(r30)
    @L_800EC3A8:
    stw r4, 0xa8(r31)
    mr r3, r30
    li r4, 0x0
    li r6, 0x0
    lwz r5, 0x4(r31)
    lwz r0, 0xa8(r31)
    lwz r5, 0x8(r5)
    slwi r0, r0, 2
    lwzx r5, r5, r0
    bl fn_801A2B5C
    lfs f0, lbl_8047CC5C(r2)
    lis r3, fn_800EE08C@ha
    addi r6, r3, fn_800EE08C@l
    addi r8, r31, 0xb8
    stfs f0, 0xb8(r31)
    mr r3, r30
    li r4, 0x6
    li r5, 0x64db
    li r7, 0x2
    crxor 6, 6, 6
    bl fn_801C028C
    @L_800EC3FC:
    lwz r4, 0x0(r31)
    lwz r3, 0x8(r31)
    rlwinm. r0, r4, 0, 28, 28
    beq @L_800EC43C
    rlwinm. r0, r4, 0, 14, 14
    beq @L_800EC418
    lwz r3, 0x10(r3)
    @L_800EC418:
    lfs f0, lbl_8047CC5C(r2)
    li r4, 0x634
    stfs f0, 0xb0(r31)
    stfs f0, 0xb4(r31)
    lfs f1, 0xb4(r31)
    bl fn_801A32A0
    lwz r0, 0x0(r31)
    rlwinm r0, r0, 0, 17, 15
    stw r0, 0x0(r31)
    @L_800EC43C:
    lwz r0, 0x0(r31)
    lwz r4, 0xa4(r31)
    rlwinm. r0, r0, 0, 14, 14
    lwz r3, 0x8(r31)
    beq @L_800EC454
    lwz r3, 0x10(r3)
    @L_800EC454:
    stw r4, 0xa4(r31)
    lwz r0, 0xa4(r31)
    cmpwi r0, 0x1
    beq @L_800EC498
    bge @L_800EC4B8
    cmpwi r0, 0x0
    bge @L_800EC474
    b @L_800EC4B8
    @L_800EC474:
    lis r4, fn_800EE054@ha
    li r5, 0x64db
    addi r6, r4, fn_800EE054@l
    li r7, 0x3
    li r4, 0x6
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801C028C
    b @L_800EC4B8
    @L_800EC498:
    lis r4, fn_800EE054@ha
    li r5, 0x64db
    addi r6, r4, fn_800EE054@l
    li r7, 0x3
    li r4, 0x6
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801C028C
    @L_800EC4B8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r3
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r4
    lwz r0, 0x114(r3)
    cmpwi r0, 0x0
    bne @L_800E4480
    addi r3, r31, 0x18
    bl fn_800E01D0
    lwz r30, 0x8(r31)
    cmplwi r30, 0x0
    bne @L_800E43F4
    la r3, lbl_8047CB60(r2)
    li r4, 0x3a9
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E43F4:
    cmplwi r29, 0x0
    bne @L_800E4410
    lis r4, lbl_80270E50@ha
    la r3, lbl_8047CB60(r2)
    addi r5, r4, lbl_80270E50@l
    li r4, 0x3aa
    bl fn_80196E10
    @L_800E4410:
    lwz r3, 0x0(r29)
    lwz r0, 0x4(r29)
    stw r3, 0x38(r30)
    stw r0, 0x3c(r30)
    lwz r0, 0x8(r29)
    stw r0, 0x40(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E4488
    cmplwi r30, 0x0
    beq @L_800E4488
    bne @L_800E4450
    la r3, lbl_8047CB60(r2)
    li r4, 0x25d
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E4450:
    lwz r4, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E446C
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E446C
    li r3, 0x1
    @L_800E446C:
    cmpwi r3, 0x0
    bne @L_800E4488
    mr r3, r30
    bl fn_8019D620
    b @L_800E4488
    @L_800E4480:
    addi r3, r31, 0x120
    bl fn_800E01D0
    @L_800E4488:
    lwz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_800E44A8
    addi r3, r31, 0x3c
    addi r4, r31, 0x18
    li r5, 0xc
    bl memcpy
    b @L_800E44F8
    @L_800E44A8:
    mr r3, r31
    li r4, 0x1
    bl fn_800EE150
    mr. r30, r3
    bne @L_800E44D0
    addi r3, r31, 0x3c
    addi r4, r31, 0x18
    li r5, 0xc
    bl memcpy
    b @L_800E44F8
    @L_800E44D0:
    addi r4, r31, 0x3c
    li r5, 0x0
    li r6, 0x0
    bl fn_800EE3BC
    mr r3, r30
    bl fn_800EE828
    lfs f1, 0x40(r31)
    lfs f0, 0x48(r31)
    fsubs f0, f1, f0
    stfs f0, 0x40(r31)
    @L_800E44F8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stfd f31, 0x10(r1)
    psq_st f31, 0x18(r1), 0, 0
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r31, r3
    lwz r0, 0x114(r3)
    cmpwi r0, 0x0
    beq @L_800E4AA0
    li r3, 0x0
    clrlwi. r0, r4, 24
    stw r3, 0x118(r31)
    stw r3, 0x11c(r31)
    lwz r0, 0x0(r31)
    rlwinm r0, r0, 0, 14, 11
    stw r0, 0x0(r31)
    stw r3, 0x114(r31)
    beq @L_800E4AA0
    lwz r0, 0x114(r31)
    cmpwi r0, 0x0
    bne @L_800E476C
    addi r3, r31, 0x18
    addi r4, r31, 0x120
    bl fn_800E01D0
    lwz r30, 0x8(r31)
    cmplwi r30, 0x0
    bne @L_800E46E0
    la r3, lbl_8047CB60(r2)
    li r4, 0x3a9
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E46E0:
    addic. r0, r31, 0x120
    bne @L_800E46FC
    lis r4, lbl_80270E50@ha
    la r3, lbl_8047CB60(r2)
    addi r5, r4, lbl_80270E50@l
    li r4, 0x3aa
    bl fn_80196E10
    @L_800E46FC:
    lwz r3, 0x120(r31)
    lwz r0, 0x124(r31)
    stw r3, 0x38(r30)
    stw r0, 0x3c(r30)
    lwz r0, 0x128(r31)
    stw r0, 0x40(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E4778
    cmplwi r30, 0x0
    beq @L_800E4778
    bne @L_800E473C
    la r3, lbl_8047CB60(r2)
    li r4, 0x25d
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E473C:
    lwz r4, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E4758
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E4758
    li r3, 0x1
    @L_800E4758:
    cmpwi r3, 0x0
    bne @L_800E4778
    mr r3, r30
    bl fn_8019D620
    b @L_800E4778
    @L_800E476C:
    addi r3, r31, 0x120
    mr r4, r3
    bl fn_800E01D0
    @L_800E4778:
    lwz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_800E4798
    addi r3, r31, 0x3c
    addi r4, r31, 0x18
    li r5, 0xc
    bl memcpy
    b @L_800E47E8
    @L_800E4798:
    mr r3, r31
    li r4, 0x1
    bl fn_800EE150
    mr. r30, r3
    bne @L_800E47C0
    addi r3, r31, 0x3c
    addi r4, r31, 0x18
    li r5, 0xc
    bl memcpy
    b @L_800E47E8
    @L_800E47C0:
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
    @L_800E47E8:
    lwz r0, 0x114(r31)
    cmpwi r0, 0x0
    bne @L_800E49CC
    addi r3, r31, 0x24
    addi r4, r31, 0x12c
    bl fn_800E01D0
    lwz r30, 0x8(r31)
    lfs f31, 0x12c(r31)
    cmplwi r30, 0x0
    bne @L_800E4820
    la r3, lbl_8047CB60(r2)
    li r4, 0x2a4
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E4820:
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E4840
    lis r4, lbl_80270E28@ha
    la r3, lbl_8047CB60(r2)
    addi r5, r4, lbl_80270E28@l
    li r4, 0x2a5
    bl fn_80196E10
    @L_800E4840:
    stfs f31, 0x1c(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E4898
    cmplwi r30, 0x0
    beq @L_800E4898
    bne @L_800E486C
    la r3, lbl_8047CB60(r2)
    li r4, 0x25d
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E486C:
    lwz r4, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E4888
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E4888
    li r3, 0x1
    @L_800E4888:
    cmpwi r3, 0x0
    bne @L_800E4898
    mr r3, r30
    bl fn_8019D620
    @L_800E4898:
    lwz r30, 0x8(r31)
    lfs f31, 0x130(r31)
    cmplwi r30, 0x0
    bne @L_800E48B8
    la r3, lbl_8047CB60(r2)
    li r4, 0x2b8
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E48B8:
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E48D8
    lis r4, lbl_80270E28@ha
    la r3, lbl_8047CB60(r2)
    addi r5, r4, lbl_80270E28@l
    li r4, 0x2b9
    bl fn_80196E10
    @L_800E48D8:
    stfs f31, 0x20(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E4930
    cmplwi r30, 0x0
    beq @L_800E4930
    bne @L_800E4904
    la r3, lbl_8047CB60(r2)
    li r4, 0x25d
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E4904:
    lwz r4, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E4920
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E4920
    li r3, 0x1
    @L_800E4920:
    cmpwi r3, 0x0
    bne @L_800E4930
    mr r3, r30
    bl fn_8019D620
    @L_800E4930:
    lwz r30, 0x8(r31)
    lfs f31, 0x134(r31)
    cmplwi r30, 0x0
    bne @L_800E4950
    la r3, lbl_8047CB60(r2)
    li r4, 0x2cc
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E4950:
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E4970
    lis r4, lbl_80270E28@ha
    la r3, lbl_8047CB60(r2)
    addi r5, r4, lbl_80270E28@l
    li r4, 0x2cd
    bl fn_80196E10
    @L_800E4970:
    stfs f31, 0x24(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E49D8
    cmplwi r30, 0x0
    beq @L_800E49D8
    bne @L_800E499C
    la r3, lbl_8047CB60(r2)
    li r4, 0x25d
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E499C:
    lwz r4, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E49B8
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E49B8
    li r3, 0x1
    @L_800E49B8:
    cmpwi r3, 0x0
    bne @L_800E49D8
    mr r3, r30
    bl fn_8019D620
    b @L_800E49D8
    @L_800E49CC:
    addi r3, r31, 0x12c
    mr r4, r3
    bl fn_800E01D0
    @L_800E49D8:
    lwz r0, 0x114(r31)
    cmpwi r0, 0x0
    bne @L_800E4A94
    addi r3, r31, 0x30
    addi r4, r31, 0x138
    bl fn_800E01D0
    lwz r30, 0x8(r31)
    cmplwi r30, 0x0
    bne @L_800E4A0C
    la r3, lbl_8047CB60(r2)
    li r4, 0x316
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E4A0C:
    addic. r0, r31, 0x138
    bne @L_800E4A24
    la r3, lbl_8047CB60(r2)
    li r4, 0x317
    la r5, lbl_8047CB70(r2)
    bl fn_80196E10
    @L_800E4A24:
    lwz r3, 0x138(r31)
    lwz r0, 0x13c(r31)
    stw r3, 0x2c(r30)
    stw r0, 0x30(r30)
    lwz r0, 0x140(r31)
    stw r0, 0x34(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E4AA0
    cmplwi r30, 0x0
    beq @L_800E4AA0
    bne @L_800E4A64
    la r3, lbl_8047CB60(r2)
    li r4, 0x25d
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E4A64:
    lwz r4, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E4A80
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E4A80
    li r3, 0x1
    @L_800E4A80:
    cmpwi r3, 0x0
    bne @L_800E4AA0
    mr r3, r30
    bl fn_8019D620
    b @L_800E4AA0
    @L_800E4A94:
    addi r3, r31, 0x138
    mr r4, r3
    bl fn_800E01D0
    @L_800E4AA0:
    psq_l f31, 0x18(r1), 0, 0
    lwz r0, 0x24(r1)
    lfd f31, 0x10(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

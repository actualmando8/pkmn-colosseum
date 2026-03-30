    nofralloc
    stwu r1, -0x110(r1)
    mflr r0
    stw r0, 0x114(r1)
    stfd f31, 0x100(r1)
    psq_st f31, 0x108(r1), 0, 0
    stfd f30, 0xf0(r1)
    psq_st f30, 0xf8(r1), 0, 0
    stfd f29, 0xe0(r1)
    psq_st f29, 0xe8(r1), 0, 0
    stfd f28, 0xd0(r1)
    psq_st f28, 0xd8(r1), 0, 0
    stmw r27, 0xbc(r1)
    fmr f31, f1
    mr r27, r3
    fmr f30, f2
    mr r28, r4
    fmr f29, f3
    mr r31, r5
    fmr f28, f4
    li r30, 0x0
    b @L_801866AC
    @L_80186674:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801866A8
    lwz r0, 0x28(r3)
    cmplw r0, r27
    bne @L_801866A8
    lwz r0, 0x2c(r3)
    cmplw r0, r28
    bne @L_801866A8
    lwz r30, 0x4(r3)
    b @L_80186718
    @L_801866A8:
    addi r30, r30, 0x1
    @L_801866AC:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_80186674
    li r29, 0x0
    b @L_80186708
    @L_801866C0:
    mr r3, r29
    bl fn_8018FD88
    mr r30, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80186704
    lwz r0, 0x2c(r30)
    cmplw r0, r28
    bne @L_80186704
    lis r3, lbl_80273FD8@ha
    mr r4, r27
    addi r3, r3, lbl_80273FD8@l
    mr r5, r28
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r30, 0x4(r30)
    b @L_80186718
    @L_80186704:
    addi r29, r29, 0x1
    @L_80186708:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_801866C0
    li r30, 0x0
    @L_80186718:
    li r29, 0x0
    b @L_8018674C
    @L_80186720:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80186748
    lwz r0, 0x4(r3)
    cmplw r0, r30
    bne @L_80186748
    mr r30, r3
    b @L_8018675C
    @L_80186748:
    addi r29, r29, 0x1
    @L_8018674C:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_80186720
    li r30, 0x0
    @L_8018675C:
    cmplwi r30, 0x0
    bne @L_8018676C
    li r3, 0x0
    b @L_80186B28
    @L_8018676C:
    mr r3, r30
    lis r4, 0x4000
    bl fn_8018FB94
    clrlwi. r0, r3, 24
    beq @L_801867D4
    addi r3, r1, 0x68
    addi r4, r30, 0x74
    bl fn_800E01D0
    lfs f1, lbl_8047D840(r2)
    addi r3, r1, 0x5c
    addi r4, r1, 0x68
    bl fn_800E013C
    addi r3, r1, 0x68
    li r4, 0x0
    li r5, 0x64
    bl fn_80186B5C
    mr r3, r30
    bl fn_8018FC00
    mr r4, r3
    addi r3, r1, 0x80
    bl fn_800E01D0
    addi r3, r1, 0x74
    addi r4, r1, 0x80
    addi r5, r1, 0x68
    bl fn_800E019C
    b @L_8018680C
    @L_801867D4:
    mr r3, r30
    bl fn_8018FCBC
    mr r4, r3
    addi r3, r1, 0x74
    bl fn_800E01D0
    mr r3, r30
    bl fn_8018FC00
    mr r4, r3
    addi r3, r1, 0x80
    bl fn_800E01D0
    addi r3, r1, 0x68
    addi r4, r1, 0x74
    addi r5, r1, 0x80
    bl fn_800E0168
    @L_8018680C:
    fmr f1, f31
    lfs f2, 0x78(r1)
    fmr f3, f30
    addi r3, r1, 0x98
    bl fn_800E01F4
    fmr f1, f29
    lfs f2, 0x78(r1)
    fmr f3, f28
    addi r3, r1, 0x8c
    bl fn_800E01F4
    li r3, 0x0
    li r4, 0x2
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_80186970
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80186970
    bl fn_801779EC
    bl fn_800D258C
    bl fn_800D2248
    li r3, 0x0
    bl fn_800DA028
    lis r3, lbl_80314638@ha
    addi r3, r3, lbl_80314638@l
    bl fn_800D7820
    li r3, 0x1
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    li r3, 0x1
    li r4, 0x6
    li r5, 0x7
    bl fn_800DA4C4
    li r3, 0x0
    bl fn_800D9ED8
    li r3, 0x4
    bl fn_800D6A00
    li r3, 0x4
    bl fn_800D67BC
    lfs f2, 0x78(r1)
    fmr f1, f31
    lfs f0, lbl_8047D844(r2)
    fmr f3, f30
    fsubs f2, f2, f0
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    lfs f2, lbl_8047D844(r2)
    fmr f1, f31
    lfs f0, 0x78(r1)
    fmr f3, f30
    fadds f2, f2, f0
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    lfs f2, 0x78(r1)
    fmr f1, f29
    lfs f0, lbl_8047D844(r2)
    fmr f3, f28
    fsubs f2, f2, f0
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    lfs f2, lbl_8047D844(r2)
    fmr f1, f29
    lfs f0, 0x78(r1)
    fmr f3, f28
    fadds f2, f2, f0
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x80
    li r6, 0xff
    li r7, 0xc0
    bl fn_800D5CB8
    bl fn_800D6728
    @L_80186970:
    lwz r3, 0x30(r30)
    bl fn_8018F6F4
    cmplwi r3, 0x0
    beq @L_8018698C
    bl fn_8018F5E4
    fmr f31, f1
    b @L_80186990
    @L_8018698C:
    lfs f31, lbl_8047D83C(r2)
    @L_80186990:
    fmr f30, f31
    addi r3, r1, 0x68
    bl fn_800E008C
    lfs f0, lbl_8047D7A0(r2)
    fcmpo cr0, f1, f0
    ble @L_801869C0
    fdivs f29, f31, f1
    lfs f0, lbl_8047D79C(r2)
    fcmpo cr0, f29, f0
    ble @L_801869C4
    fmr f29, f0
    b @L_801869C4
    @L_801869C0:
    lfs f29, lbl_8047D79C(r2)
    @L_801869C4:
    addi r3, r1, 0x14
    addi r4, r1, 0x8c
    addi r5, r1, 0x98
    bl fn_800E0168
    addi r3, r1, 0x14
    bl fn_800E008C
    lfs f0, lbl_8047D848(r2)
    fcmpo cr0, f1, f0
    bge @L_801869F0
    li r3, 0x0
    b @L_80186B28
    @L_801869F0:
    addi r3, r1, 0x14
    mr r4, r3
    bl fn_800E0060
    addi r3, r1, 0x8
    addi r4, r1, 0x80
    addi r5, r1, 0x98
    bl fn_800E0168
    addi r3, r1, 0x8
    addi r4, r1, 0x14
    bl fn_800E0000
    addi r3, r1, 0x8
    addi r4, r1, 0x14
    bl fn_800E013C
    addi r3, r1, 0x2c
    addi r4, r1, 0x8
    addi r5, r1, 0x98
    bl fn_800E019C
    addi r3, r1, 0x20
    addi r4, r1, 0x80
    addi r5, r1, 0x2c
    bl fn_800E0168
    lfs f28, lbl_8047D7A0(r2)
    lfs f31, lbl_8047D79C(r2)
    b @L_80186B1C
    @L_80186A50:
    fadds f1, f28, f29
    fcmpo cr0, f1, f31
    ble @L_80186A60
    fmr f1, f31
    @L_80186A60:
    addi r3, r1, 0xa4
    addi r4, r1, 0x68
    bl fn_800E013C
    addi r3, r1, 0xa4
    addi r4, r1, 0x80
    mr r5, r3
    bl fn_800E019C
    fmr f1, f30
    addi r3, r1, 0x2c
    addi r4, r1, 0xa4
    addi r5, r1, 0x98
    addi r6, r1, 0x8c
    addi r7, r1, 0x20
    bl fn_801870E8
    clrlwi. r0, r3, 24
    beq @L_80186B18
    clrlwi. r0, r31, 24
    beq @L_80186B10
    lfs f0, lbl_8047D848(r2)
    addi r3, r1, 0x38
    addi r4, r1, 0x2c
    addi r5, r1, 0xa4
    fadds f1, f0, f30
    bl fn_8010F6A0
    lfs f1, lbl_8047D83C(r2)
    addi r3, r1, 0x80
    addi r4, r1, 0x38
    addi r5, r1, 0x50
    bl fn_8010F320
    cmpwi r3, 0x0
    beq @L_80186AFC
    addi r3, r1, 0x44
    addi r4, r1, 0x50
    addi r5, r1, 0x38
    bl fn_800E0168
    addi r3, r1, 0x38
    addi r5, r1, 0x44
    mr r4, r3
    bl fn_800E019C
    @L_80186AFC:
    mr r3, r30
    addi r4, r1, 0x38
    bl fn_8018FC74
    li r3, 0x1
    bl fn_8012B184
    @L_80186B10:
    li r3, 0x1
    b @L_80186B28
    @L_80186B18:
    fadds f28, f28, f29
    @L_80186B1C:
    fcmpo cr0, f28, f31
    blt @L_80186A50
    li r3, 0x0
    @L_80186B28:
    psq_l f31, 0x108(r1), 0, 0
    lfd f31, 0x100(r1)
    psq_l f30, 0xf8(r1), 0, 0
    lfd f30, 0xf0(r1)
    psq_l f29, 0xe8(r1), 0, 0
    lfd f29, 0xe0(r1)
    psq_l f28, 0xd8(r1), 0, 0
    lfd f28, 0xd0(r1)
    lmw r27, 0xbc(r1)
    lwz r0, 0x114(r1)
    mtlr r0
    addi r1, r1, 0x110
    blr

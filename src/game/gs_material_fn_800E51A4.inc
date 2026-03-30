    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r4
    stw r29, 0x14(r1)
    mr r29, r3
    lwz r0, lbl_8047AB78(r13)
    lwz r31, lbl_8047AB74(r13)
    mtctr r0
    cmplwi r0, 0x0
    ble @L_800E51F0
    @L_800E51D8:
    lwz r0, 0x0(r31)
    clrlwi. r0, r0, 31
    bne @L_800E51E8
    b @L_800E51F4
    @L_800E51E8:
    addi r31, r31, 0x170
    bdnz @L_800E51D8
    @L_800E51F0:
    li r31, 0x0
    @L_800E51F4:
    cmplwi r31, 0x0
    bne @L_800E5204
    li r3, 0x0
    b @L_800E5534
    @L_800E5204:
    mr r3, r31
    li r4, 0x0
    li r5, 0x170
    bl memset
    li r0, 0x2001
    li r4, 0x0
    stw r0, 0x0(r31)
    li r0, 0x1
    mr r3, r30
    stw r4, 0x84(r31)
    stw r4, 0x88(r31)
    stb r4, 0x80(r31)
    stw r4, 0xdc(r31)
    sth r4, 0x150(r31)
    stw r4, 0x14c(r31)
    sth r4, 0x152(r31)
    stw r4, 0x158(r31)
    stw r4, 0x15c(r31)
    stw r4, 0x160(r31)
    sth r4, 0x164(r31)
    stw r4, 0x114(r31)
    stw r4, 0x118(r31)
    stw r4, 0x11c(r31)
    stw r0, 0x148(r31)
    stw r29, 0x4(r31)
    bl fn_801A0FBC
    stw r3, 0x8(r31)
    li r0, 0x0
    addi r3, r31, 0x18
    stw r0, 0xc(r31)
    lwz r4, 0x8(r31)
    lfs f1, 0x38(r4)
    lfs f2, 0x3c(r4)
    lfs f3, 0x40(r4)
    bl fn_800E01F4
    lwz r4, 0x8(r31)
    addi r3, r31, 0x24
    lfs f1, 0x1c(r4)
    lfs f2, 0x20(r4)
    lfs f3, 0x24(r4)
    bl fn_800E01F4
    lwz r4, 0x8(r31)
    addi r3, r31, 0x30
    lfs f1, 0x2c(r4)
    lfs f2, 0x30(r4)
    lfs f3, 0x34(r4)
    bl fn_800E01F4
    lwz r4, 0x8(r31)
    addi r3, r31, 0x3c
    lfs f1, 0x38(r4)
    lfs f2, 0x3c(r4)
    lfs f3, 0x40(r4)
    bl fn_800E01F4
    li r3, 0x0
    li r0, -0x1
    stw r3, 0xe4(r31)
    stw r0, 0xe8(r31)
    stw r3, 0xf0(r31)
    stw r0, 0xf4(r31)
    stw r3, 0xfc(r31)
    stw r0, 0x100(r31)
    stw r3, 0x108(r31)
    stw r0, 0x10c(r31)
    lwz r3, 0x4(r31)
    lwz r0, 0xc(r3)
    cmplwi r0, 0x0
    beq @L_800E531C
    lwz r0, 0x0(r31)
    ori r0, r0, 0x10
    stw r0, 0x0(r31)
    @L_800E531C:
    lwz r3, 0x4(r31)
    lwz r0, 0x4(r3)
    cmplwi r0, 0x0
    beq @L_800E5390
    lwz r3, 0x0(r31)
    li r0, -0x1
    ori r3, r3, 0x4
    stw r3, 0x0(r31)
    stw r0, 0x90(r31)
    b @L_800E5350
    @L_800E5344:
    lwz r3, 0x84(r31)
    addi r0, r3, 0x1
    stw r0, 0x84(r31)
    @L_800E5350:
    lwz r3, 0x4(r31)
    lwz r0, 0x84(r31)
    lwz r3, 0x4(r3)
    slwi r0, r0, 2
    lwzx r0, r3, r0
    cmplwi r0, 0x0
    bne @L_800E5344
    mr r3, r31
    li r4, 0x0
    bl fn_800ECCA8
    lfs f1, lbl_8047CB84(r2)
    mr r3, r31
    bl fn_800EC9DC
    mr r3, r31
    li r4, 0x1
    bl fn_800ECB74
    @L_800E5390:
    lwz r3, 0x4(r31)
    lwz r0, 0x8(r3)
    cmplwi r0, 0x0
    beq @L_800E5404
    lwz r3, 0x0(r31)
    li r0, -0x1
    ori r3, r3, 0x8
    stw r3, 0x0(r31)
    stw r0, 0xa8(r31)
    b @L_800E53C4
    @L_800E53B8:
    lwz r3, 0x88(r31)
    addi r0, r3, 0x1
    stw r0, 0x88(r31)
    @L_800E53C4:
    lwz r3, 0x4(r31)
    lwz r0, 0x88(r31)
    lwz r3, 0x8(r3)
    slwi r0, r0, 2
    lwzx r0, r3, r0
    cmplwi r0, 0x0
    bne @L_800E53B8
    mr r3, r31
    li r4, 0x0
    bl fn_800EC35C
    lfs f1, lbl_8047CB84(r2)
    mr r3, r31
    bl fn_800EC308
    mr r3, r31
    li r4, 0x1
    bl fn_800EC208
    @L_800E5404:
    addi r3, r31, 0x4c
    addi r4, r31, 0x3c
    bl fn_8019147C
    addi r3, r31, 0x4c
    addi r4, r31, 0x24
    bl fn_80191474
    addi r3, r31, 0x4c
    addi r4, r31, 0x30
    bl fn_8019146C
    mr r3, r31
    bl fn_800EB340
    lfs f0, lbl_8047CB7C(r2)
    stfs f0, 0x48(r31)
    lwz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_800E5458
    addi r3, r31, 0x3c
    addi r4, r31, 0x18
    li r5, 0xc
    bl memcpy
    b @L_800E54A8
    @L_800E5458:
    mr r3, r31
    li r4, 0x1
    bl fn_800EE150
    mr. r30, r3
    bne @L_800E5480
    addi r3, r31, 0x3c
    addi r4, r31, 0x18
    li r5, 0xc
    bl memcpy
    b @L_800E54A8
    @L_800E5480:
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
    @L_800E54A8:
    lfs f0, 0x40(r31)
    stfs f0, 0x48(r31)
    lwz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_800E54D0
    addi r3, r31, 0x3c
    addi r4, r31, 0x18
    li r5, 0xc
    bl memcpy
    b @L_800E5520
    @L_800E54D0:
    mr r3, r31
    li r4, 0x1
    bl fn_800EE150
    mr. r30, r3
    bne @L_800E54F8
    addi r3, r31, 0x3c
    addi r4, r31, 0x18
    li r5, 0xc
    bl memcpy
    b @L_800E5520
    @L_800E54F8:
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
    @L_800E5520:
    lwz r3, 0x8(r31)
    addi r4, r31, 0x168
    addi r5, r31, 0x16c
    bl fn_8019F1C4
    mr r3, r31
    @L_800E5534:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

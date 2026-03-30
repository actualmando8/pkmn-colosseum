    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stfd f31, 0x80(r1)
    psq_st f31, 0x88(r1), 0, 0
    stfd f30, 0x70(r1)
    psq_st f30, 0x78(r1), 0, 0
    stfd f29, 0x60(r1)
    psq_st f29, 0x68(r1), 0, 0
    stfd f28, 0x50(r1)
    psq_st f28, 0x58(r1), 0, 0
    stmw r26, 0x38(r1)
    mr r31, r3
    lwz r3, 0x0(r3)
    lwz r30, 0x8(r31)
    mr r26, r4
    rlwinm. r0, r3, 0, 24, 24
    mr r27, r5
    mr r28, r6
    bne @L_800E85B4
    rlwinm. r0, r3, 0, 14, 14
    bne @L_800E7F08
    bl fn_8019F718
    mr. r29, r3
    beq @L_800E85B4
    cmplwi r30, 0x0
    bne @L_800E73A8
    la r3, lbl_8047CB9C(r2)
    li r4, 0x3e4
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E73A8:
    addic. r0, r1, 0x14
    bne @L_800E73C4
    lis r4, lbl_80270E60@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E60@l
    li r4, 0x3e5
    bl fn_80196E10
    @L_800E73C4:
    lwz r4, 0x38(r30)
    cmplwi r30, 0x0
    lwz r3, 0x3c(r30)
    lwz r0, 0x40(r30)
    stw r4, 0x14(r1)
    stw r3, 0x18(r1)
    stw r0, 0x1c(r1)
    bne @L_800E73F4
    la r3, lbl_8047CB9C(r2)
    li r4, 0x2ec
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E73F4:
    cmplwi r30, 0x0
    lfs f30, 0x1c(r30)
    bne @L_800E7410
    la r3, lbl_8047CB9C(r2)
    li r4, 0x2fa
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7410:
    cmplwi r30, 0x0
    lfs f29, 0x20(r30)
    bne @L_800E742C
    la r3, lbl_8047CB9C(r2)
    li r4, 0x308
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E742C:
    cmplwi r30, 0x0
    lfs f28, 0x24(r30)
    bne @L_800E7448
    la r3, lbl_8047CB9C(r2)
    li r4, 0x351
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7448:
    addic. r0, r1, 0x8
    bne @L_800E7460
    la r3, lbl_8047CB9C(r2)
    li r4, 0x352
    la r5, lbl_8047CBAC(r2)
    bl fn_80196E10
    @L_800E7460:
    lwz r4, 0x2c(r30)
    cmplwi r26, 0x0
    lwz r3, 0x30(r30)
    lwz r0, 0x34(r30)
    stw r4, 0x8(r1)
    stw r3, 0xc(r1)
    stw r0, 0x10(r1)
    beq @L_800E7524
    cmplwi r30, 0x0
    bne @L_800E7498
    la r3, lbl_8047CB9C(r2)
    li r4, 0x3a9
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7498:
    cmplwi r26, 0x0
    bne @L_800E74B4
    lis r4, lbl_80270E60@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E60@l
    li r4, 0x3aa
    bl fn_80196E10
    @L_800E74B4:
    lwz r3, 0x0(r26)
    lwz r0, 0x4(r26)
    stw r3, 0x38(r30)
    stw r0, 0x3c(r30)
    lwz r0, 0x8(r26)
    stw r0, 0x40(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E75CC
    cmplwi r30, 0x0
    beq @L_800E75CC
    bne @L_800E74F4
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E74F4:
    lwz r4, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E7510
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E7510
    li r3, 0x1
    @L_800E7510:
    cmpwi r3, 0x0
    bne @L_800E75CC
    mr r3, r30
    bl fn_8019D620
    b @L_800E75CC
    @L_800E7524:
    addi r3, r1, 0x20
    bl fn_800E0204
    cmplwi r30, 0x0
    bne @L_800E7544
    la r3, lbl_8047CB9C(r2)
    li r4, 0x3a9
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7544:
    addic. r0, r1, 0x20
    bne @L_800E7560
    lis r4, lbl_80270E60@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E60@l
    li r4, 0x3aa
    bl fn_80196E10
    @L_800E7560:
    lwz r3, 0x20(r1)
    lwz r0, 0x24(r1)
    stw r3, 0x38(r30)
    stw r0, 0x3c(r30)
    lwz r0, 0x28(r1)
    stw r0, 0x40(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E75CC
    cmplwi r30, 0x0
    beq @L_800E75CC
    bne @L_800E75A0
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E75A0:
    lwz r4, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E75BC
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E75BC
    li r3, 0x1
    @L_800E75BC:
    cmpwi r3, 0x0
    bne @L_800E75CC
    mr r3, r30
    bl fn_8019D620
    @L_800E75CC:
    cmplwi r27, 0x0
    beq @L_800E7794
    cmplwi r30, 0x0
    lfs f31, 0x0(r27)
    bne @L_800E75F0
    la r3, lbl_8047CB9C(r2)
    li r4, 0x2a4
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E75F0:
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E7610
    lis r4, lbl_80270E6C@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E6C@l
    li r4, 0x2a5
    bl fn_80196E10
    @L_800E7610:
    stfs f31, 0x1c(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E7668
    cmplwi r30, 0x0
    beq @L_800E7668
    bne @L_800E763C
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E763C:
    lwz r4, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E7658
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E7658
    li r3, 0x1
    @L_800E7658:
    cmpwi r3, 0x0
    bne @L_800E7668
    mr r3, r30
    bl fn_8019D620
    @L_800E7668:
    cmplwi r30, 0x0
    lfs f31, 0x4(r27)
    bne @L_800E7684
    la r3, lbl_8047CB9C(r2)
    li r4, 0x2b8
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7684:
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E76A4
    lis r4, lbl_80270E6C@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E6C@l
    li r4, 0x2b9
    bl fn_80196E10
    @L_800E76A4:
    stfs f31, 0x20(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E76FC
    cmplwi r30, 0x0
    beq @L_800E76FC
    bne @L_800E76D0
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E76D0:
    lwz r4, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E76EC
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E76EC
    li r3, 0x1
    @L_800E76EC:
    cmpwi r3, 0x0
    bne @L_800E76FC
    mr r3, r30
    bl fn_8019D620
    @L_800E76FC:
    cmplwi r30, 0x0
    lfs f31, 0x8(r27)
    bne @L_800E7718
    la r3, lbl_8047CB9C(r2)
    li r4, 0x2cc
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7718:
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E7738
    lis r4, lbl_80270E6C@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E6C@l
    li r4, 0x2cd
    bl fn_80196E10
    @L_800E7738:
    stfs f31, 0x24(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E7950
    cmplwi r30, 0x0
    beq @L_800E7950
    bne @L_800E7764
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7764:
    lwz r4, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E7780
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E7780
    li r3, 0x1
    @L_800E7780:
    cmpwi r3, 0x0
    bne @L_800E7950
    mr r3, r30
    bl fn_8019D620
    b @L_800E7950
    @L_800E7794:
    cmplwi r30, 0x0
    bne @L_800E77AC
    la r3, lbl_8047CB9C(r2)
    li r4, 0x2a4
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E77AC:
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E77CC
    lis r4, lbl_80270E6C@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E6C@l
    li r4, 0x2a5
    bl fn_80196E10
    @L_800E77CC:
    lfs f0, lbl_8047CBB4(r2)
    stfs f0, 0x1c(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E7828
    cmplwi r30, 0x0
    beq @L_800E7828
    bne @L_800E77FC
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E77FC:
    lwz r4, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E7818
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E7818
    li r3, 0x1
    @L_800E7818:
    cmpwi r3, 0x0
    bne @L_800E7828
    mr r3, r30
    bl fn_8019D620
    @L_800E7828:
    cmplwi r30, 0x0
    bne @L_800E7840
    la r3, lbl_8047CB9C(r2)
    li r4, 0x2b8
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7840:
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E7860
    lis r4, lbl_80270E6C@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E6C@l
    li r4, 0x2b9
    bl fn_80196E10
    @L_800E7860:
    lfs f0, lbl_8047CBB4(r2)
    stfs f0, 0x20(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E78BC
    cmplwi r30, 0x0
    beq @L_800E78BC
    bne @L_800E7890
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7890:
    lwz r4, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E78AC
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E78AC
    li r3, 0x1
    @L_800E78AC:
    cmpwi r3, 0x0
    bne @L_800E78BC
    mr r3, r30
    bl fn_8019D620
    @L_800E78BC:
    cmplwi r30, 0x0
    bne @L_800E78D4
    la r3, lbl_8047CB9C(r2)
    li r4, 0x2cc
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E78D4:
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E78F4
    lis r4, lbl_80270E6C@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E6C@l
    li r4, 0x2cd
    bl fn_80196E10
    @L_800E78F4:
    lfs f0, lbl_8047CBB4(r2)
    stfs f0, 0x24(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E7950
    cmplwi r30, 0x0
    beq @L_800E7950
    bne @L_800E7924
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7924:
    lwz r4, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E7940
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E7940
    li r3, 0x1
    @L_800E7940:
    cmpwi r3, 0x0
    bne @L_800E7950
    mr r3, r30
    bl fn_8019D620
    @L_800E7950:
    cmplwi r28, 0x0
    beq @L_800E79F8
    cmplwi r30, 0x0
    bne @L_800E7970
    la r3, lbl_8047CB9C(r2)
    li r4, 0x316
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7970:
    cmplwi r28, 0x0
    bne @L_800E7988
    la r3, lbl_8047CB9C(r2)
    li r4, 0x317
    la r5, lbl_8047CBAC(r2)
    bl fn_80196E10
    @L_800E7988:
    lwz r3, 0x0(r28)
    lwz r0, 0x4(r28)
    stw r3, 0x2c(r30)
    stw r0, 0x30(r30)
    lwz r0, 0x8(r28)
    stw r0, 0x34(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E7AA8
    cmplwi r30, 0x0
    beq @L_800E7AA8
    bne @L_800E79C8
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E79C8:
    lwz r4, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E79E4
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E79E4
    li r3, 0x1
    @L_800E79E4:
    cmpwi r3, 0x0
    bne @L_800E7AA8
    mr r3, r30
    bl fn_8019D620
    b @L_800E7AA8
    @L_800E79F8:
    lfs f1, lbl_8047CBB8(r2)
    addi r3, r1, 0x20
    fmr f2, f1
    fmr f3, f1
    bl fn_800E01F4
    cmplwi r30, 0x0
    bne @L_800E7A24
    la r3, lbl_8047CB9C(r2)
    li r4, 0x316
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7A24:
    addic. r0, r1, 0x20
    bne @L_800E7A3C
    la r3, lbl_8047CB9C(r2)
    li r4, 0x317
    la r5, lbl_8047CBAC(r2)
    bl fn_80196E10
    @L_800E7A3C:
    lwz r3, 0x20(r1)
    lwz r0, 0x24(r1)
    stw r3, 0x2c(r30)
    stw r0, 0x30(r30)
    lwz r0, 0x28(r1)
    stw r0, 0x34(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E7AA8
    cmplwi r30, 0x0
    beq @L_800E7AA8
    bne @L_800E7A7C
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7A7C:
    lwz r4, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E7A98
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E7A98
    li r3, 0x1
    @L_800E7A98:
    cmpwi r3, 0x0
    bne @L_800E7AA8
    mr r3, r30
    bl fn_8019D620
    @L_800E7AA8:
    cmplwi r30, 0x0
    beq @L_800E7AF0
    bne @L_800E7AC4
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7AC4:
    lwz r4, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E7AE0
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E7AE0
    li r3, 0x1
    @L_800E7AE0:
    cmpwi r3, 0x0
    beq @L_800E7AF0
    mr r3, r30
    bl fn_8019D9DC
    @L_800E7AF0:
    lwz r4, 0x8(r31)
    mr r3, r29
    bl fn_801A015C
    lwz r0, 0x0(r31)
    mr r3, r30
    oris r0, r0, 0x2
    stw r0, 0x0(r31)
    stw r29, 0x8(r31)
    bl fn_8019FF30
    mr r4, r3
    mr r3, r29
    bl fn_8019FE8C
    cmplwi r29, 0x0
    bne @L_800E7B38
    la r3, lbl_8047CB9C(r2)
    li r4, 0x3a9
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7B38:
    addic. r0, r1, 0x14
    bne @L_800E7B54
    lis r4, lbl_80270E60@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E60@l
    li r4, 0x3aa
    bl fn_80196E10
    @L_800E7B54:
    lwz r0, 0x14(r1)
    lwz r3, 0x18(r1)
    stw r0, 0x38(r29)
    lwz r0, 0x1c(r1)
    stw r3, 0x3c(r29)
    stw r0, 0x40(r29)
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E7BC0
    cmplwi r29, 0x0
    beq @L_800E7BC0
    bne @L_800E7B94
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7B94:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E7BB0
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E7BB0
    li r3, 0x1
    @L_800E7BB0:
    cmpwi r3, 0x0
    bne @L_800E7BC0
    mr r3, r29
    bl fn_8019D620
    @L_800E7BC0:
    cmplwi r29, 0x0
    bne @L_800E7BD8
    la r3, lbl_8047CB9C(r2)
    li r4, 0x2a4
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7BD8:
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E7BF8
    lis r4, lbl_80270E6C@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E6C@l
    li r4, 0x2a5
    bl fn_80196E10
    @L_800E7BF8:
    stfs f30, 0x1c(r29)
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E7C50
    cmplwi r29, 0x0
    beq @L_800E7C50
    bne @L_800E7C24
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7C24:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E7C40
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E7C40
    li r3, 0x1
    @L_800E7C40:
    cmpwi r3, 0x0
    bne @L_800E7C50
    mr r3, r29
    bl fn_8019D620
    @L_800E7C50:
    cmplwi r29, 0x0
    bne @L_800E7C68
    la r3, lbl_8047CB9C(r2)
    li r4, 0x2b8
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7C68:
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E7C88
    lis r4, lbl_80270E6C@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E6C@l
    li r4, 0x2b9
    bl fn_80196E10
    @L_800E7C88:
    stfs f29, 0x20(r29)
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E7CE0
    cmplwi r29, 0x0
    beq @L_800E7CE0
    bne @L_800E7CB4
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7CB4:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E7CD0
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E7CD0
    li r3, 0x1
    @L_800E7CD0:
    cmpwi r3, 0x0
    bne @L_800E7CE0
    mr r3, r29
    bl fn_8019D620
    @L_800E7CE0:
    cmplwi r29, 0x0
    bne @L_800E7CF8
    la r3, lbl_8047CB9C(r2)
    li r4, 0x2cc
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7CF8:
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E7D18
    lis r4, lbl_80270E6C@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E6C@l
    li r4, 0x2cd
    bl fn_80196E10
    @L_800E7D18:
    stfs f28, 0x24(r29)
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E7D70
    cmplwi r29, 0x0
    beq @L_800E7D70
    bne @L_800E7D44
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7D44:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E7D60
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E7D60
    li r3, 0x1
    @L_800E7D60:
    cmpwi r3, 0x0
    bne @L_800E7D70
    mr r3, r29
    bl fn_8019D620
    @L_800E7D70:
    cmplwi r29, 0x0
    bne @L_800E7D88
    la r3, lbl_8047CB9C(r2)
    li r4, 0x316
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7D88:
    addic. r0, r1, 0x8
    bne @L_800E7DA0
    la r3, lbl_8047CB9C(r2)
    li r4, 0x317
    la r5, lbl_8047CBAC(r2)
    bl fn_80196E10
    @L_800E7DA0:
    lwz r0, 0x8(r1)
    lwz r3, 0xc(r1)
    stw r0, 0x2c(r29)
    lwz r0, 0x10(r1)
    stw r3, 0x30(r29)
    stw r0, 0x34(r29)
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E7E0C
    cmplwi r29, 0x0
    beq @L_800E7E0C
    bne @L_800E7DE0
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7DE0:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E7DFC
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E7DFC
    li r3, 0x1
    @L_800E7DFC:
    cmpwi r3, 0x0
    bne @L_800E7E0C
    mr r3, r29
    bl fn_8019D620
    @L_800E7E0C:
    cmplwi r30, 0x0
    beq @L_800E7E54
    bne @L_800E7E28
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7E28:
    lwz r4, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E7E44
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E7E44
    li r3, 0x1
    @L_800E7E44:
    cmpwi r3, 0x0
    bne @L_800E7E54
    mr r3, r30
    bl fn_8019D620
    @L_800E7E54:
    cmplwi r30, 0x0
    beq @L_800E7E9C
    bne @L_800E7E70
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7E70:
    lwz r4, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E7E8C
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E7E8C
    li r3, 0x1
    @L_800E7E8C:
    cmpwi r3, 0x0
    beq @L_800E7E9C
    mr r3, r30
    bl fn_8019D9DC
    @L_800E7E9C:
    cmplwi r29, 0x0
    beq @L_800E7EE4
    bne @L_800E7EB8
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7EB8:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E7ED4
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E7ED4
    li r3, 0x1
    @L_800E7ED4:
    cmpwi r3, 0x0
    beq @L_800E7EE4
    mr r3, r29
    bl fn_8019D9DC
    @L_800E7EE4:
    lwz r4, 0x90(r31)
    mr r3, r31
    bl fn_800ECCA8
    lwz r4, 0x8c(r31)
    mr r3, r31
    bl fn_800ECB74
    mr r3, r31
    bl fn_800EC990
    b @L_800E85B4
    @L_800E7F08:
    cmplwi r30, 0x0
    bne @L_800E7F18
    li r29, 0x0
    b @L_800E7F1C
    @L_800E7F18:
    lwz r29, 0x10(r30)
    @L_800E7F1C:
    cmplwi r26, 0x0
    beq @L_800E7FC8
    cmplwi r29, 0x0
    bne @L_800E7F3C
    la r3, lbl_8047CB9C(r2)
    li r4, 0x3a9
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7F3C:
    cmplwi r26, 0x0
    bne @L_800E7F58
    lis r4, lbl_80270E60@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E60@l
    li r4, 0x3aa
    bl fn_80196E10
    @L_800E7F58:
    lwz r3, 0x0(r26)
    lwz r0, 0x4(r26)
    stw r3, 0x38(r29)
    stw r0, 0x3c(r29)
    lwz r0, 0x8(r26)
    stw r0, 0x40(r29)
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E8070
    cmplwi r29, 0x0
    beq @L_800E8070
    bne @L_800E7F98
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7F98:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E7FB4
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E7FB4
    li r3, 0x1
    @L_800E7FB4:
    cmpwi r3, 0x0
    bne @L_800E8070
    mr r3, r29
    bl fn_8019D620
    b @L_800E8070
    @L_800E7FC8:
    addi r3, r1, 0x20
    bl fn_800E0204
    cmplwi r29, 0x0
    bne @L_800E7FE8
    la r3, lbl_8047CB9C(r2)
    li r4, 0x3a9
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E7FE8:
    addic. r0, r1, 0x20
    bne @L_800E8004
    lis r4, lbl_80270E60@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E60@l
    li r4, 0x3aa
    bl fn_80196E10
    @L_800E8004:
    lwz r3, 0x20(r1)
    lwz r0, 0x24(r1)
    stw r3, 0x38(r29)
    stw r0, 0x3c(r29)
    lwz r0, 0x28(r1)
    stw r0, 0x40(r29)
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E8070
    cmplwi r29, 0x0
    beq @L_800E8070
    bne @L_800E8044
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E8044:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E8060
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E8060
    li r3, 0x1
    @L_800E8060:
    cmpwi r3, 0x0
    bne @L_800E8070
    mr r3, r29
    bl fn_8019D620
    @L_800E8070:
    cmplwi r27, 0x0
    beq @L_800E8238
    cmplwi r29, 0x0
    lfs f31, 0x0(r27)
    bne @L_800E8094
    la r3, lbl_8047CB9C(r2)
    li r4, 0x2a4
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E8094:
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E80B4
    lis r4, lbl_80270E6C@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E6C@l
    li r4, 0x2a5
    bl fn_80196E10
    @L_800E80B4:
    stfs f31, 0x1c(r29)
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E810C
    cmplwi r29, 0x0
    beq @L_800E810C
    bne @L_800E80E0
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E80E0:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E80FC
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E80FC
    li r3, 0x1
    @L_800E80FC:
    cmpwi r3, 0x0
    bne @L_800E810C
    mr r3, r29
    bl fn_8019D620
    @L_800E810C:
    cmplwi r29, 0x0
    lfs f31, 0x4(r27)
    bne @L_800E8128
    la r3, lbl_8047CB9C(r2)
    li r4, 0x2b8
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E8128:
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E8148
    lis r4, lbl_80270E6C@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E6C@l
    li r4, 0x2b9
    bl fn_80196E10
    @L_800E8148:
    stfs f31, 0x20(r29)
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E81A0
    cmplwi r29, 0x0
    beq @L_800E81A0
    bne @L_800E8174
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E8174:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E8190
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E8190
    li r3, 0x1
    @L_800E8190:
    cmpwi r3, 0x0
    bne @L_800E81A0
    mr r3, r29
    bl fn_8019D620
    @L_800E81A0:
    cmplwi r29, 0x0
    lfs f31, 0x8(r27)
    bne @L_800E81BC
    la r3, lbl_8047CB9C(r2)
    li r4, 0x2cc
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E81BC:
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E81DC
    lis r4, lbl_80270E6C@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E6C@l
    li r4, 0x2cd
    bl fn_80196E10
    @L_800E81DC:
    stfs f31, 0x24(r29)
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E83F4
    cmplwi r29, 0x0
    beq @L_800E83F4
    bne @L_800E8208
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E8208:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E8224
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E8224
    li r3, 0x1
    @L_800E8224:
    cmpwi r3, 0x0
    bne @L_800E83F4
    mr r3, r29
    bl fn_8019D620
    b @L_800E83F4
    @L_800E8238:
    cmplwi r29, 0x0
    bne @L_800E8250
    la r3, lbl_8047CB9C(r2)
    li r4, 0x2a4
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E8250:
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E8270
    lis r4, lbl_80270E6C@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E6C@l
    li r4, 0x2a5
    bl fn_80196E10
    @L_800E8270:
    lfs f0, lbl_8047CBB4(r2)
    stfs f0, 0x1c(r29)
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E82CC
    cmplwi r29, 0x0
    beq @L_800E82CC
    bne @L_800E82A0
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E82A0:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E82BC
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E82BC
    li r3, 0x1
    @L_800E82BC:
    cmpwi r3, 0x0
    bne @L_800E82CC
    mr r3, r29
    bl fn_8019D620
    @L_800E82CC:
    cmplwi r29, 0x0
    bne @L_800E82E4
    la r3, lbl_8047CB9C(r2)
    li r4, 0x2b8
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E82E4:
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E8304
    lis r4, lbl_80270E6C@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E6C@l
    li r4, 0x2b9
    bl fn_80196E10
    @L_800E8304:
    lfs f0, lbl_8047CBB4(r2)
    stfs f0, 0x20(r29)
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E8360
    cmplwi r29, 0x0
    beq @L_800E8360
    bne @L_800E8334
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E8334:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E8350
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E8350
    li r3, 0x1
    @L_800E8350:
    cmpwi r3, 0x0
    bne @L_800E8360
    mr r3, r29
    bl fn_8019D620
    @L_800E8360:
    cmplwi r29, 0x0
    bne @L_800E8378
    la r3, lbl_8047CB9C(r2)
    li r4, 0x2cc
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E8378:
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E8398
    lis r4, lbl_80270E6C@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E6C@l
    li r4, 0x2cd
    bl fn_80196E10
    @L_800E8398:
    lfs f0, lbl_8047CBB4(r2)
    stfs f0, 0x24(r29)
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E83F4
    cmplwi r29, 0x0
    beq @L_800E83F4
    bne @L_800E83C8
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E83C8:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E83E4
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E83E4
    li r3, 0x1
    @L_800E83E4:
    cmpwi r3, 0x0
    bne @L_800E83F4
    mr r3, r29
    bl fn_8019D620
    @L_800E83F4:
    cmplwi r28, 0x0
    beq @L_800E849C
    cmplwi r29, 0x0
    bne @L_800E8414
    la r3, lbl_8047CB9C(r2)
    li r4, 0x316
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E8414:
    cmplwi r28, 0x0
    bne @L_800E842C
    la r3, lbl_8047CB9C(r2)
    li r4, 0x317
    la r5, lbl_8047CBAC(r2)
    bl fn_80196E10
    @L_800E842C:
    lwz r3, 0x0(r28)
    lwz r0, 0x4(r28)
    stw r3, 0x2c(r29)
    stw r0, 0x30(r29)
    lwz r0, 0x8(r28)
    stw r0, 0x34(r29)
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E854C
    cmplwi r29, 0x0
    beq @L_800E854C
    bne @L_800E846C
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E846C:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E8488
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E8488
    li r3, 0x1
    @L_800E8488:
    cmpwi r3, 0x0
    bne @L_800E854C
    mr r3, r29
    bl fn_8019D620
    b @L_800E854C
    @L_800E849C:
    lfs f1, lbl_8047CBB8(r2)
    addi r3, r1, 0x20
    fmr f2, f1
    fmr f3, f1
    bl fn_800E01F4
    cmplwi r29, 0x0
    bne @L_800E84C8
    la r3, lbl_8047CB9C(r2)
    li r4, 0x316
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E84C8:
    addic. r0, r1, 0x20
    bne @L_800E84E0
    la r3, lbl_8047CB9C(r2)
    li r4, 0x317
    la r5, lbl_8047CBAC(r2)
    bl fn_80196E10
    @L_800E84E0:
    lwz r3, 0x20(r1)
    lwz r0, 0x24(r1)
    stw r3, 0x2c(r29)
    stw r0, 0x30(r29)
    lwz r0, 0x28(r1)
    stw r0, 0x34(r29)
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E854C
    cmplwi r29, 0x0
    beq @L_800E854C
    bne @L_800E8520
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E8520:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E853C
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E853C
    li r3, 0x1
    @L_800E853C:
    cmpwi r3, 0x0
    bne @L_800E854C
    mr r3, r29
    bl fn_8019D620
    @L_800E854C:
    cmplwi r29, 0x0
    beq @L_800E8594
    bne @L_800E8568
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E8568:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E8584
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E8584
    li r3, 0x1
    @L_800E8584:
    cmpwi r3, 0x0
    beq @L_800E8594
    mr r3, r29
    bl fn_8019D9DC
    @L_800E8594:
    lwz r4, 0x90(r31)
    mr r3, r31
    bl fn_800ECCA8
    lwz r4, 0x8c(r31)
    mr r3, r31
    bl fn_800ECB74
    mr r3, r31
    bl fn_800EC990
    @L_800E85B4:
    psq_l f31, 0x88(r1), 0, 0
    lfd f31, 0x80(r1)
    psq_l f30, 0x78(r1), 0, 0
    lfd f30, 0x70(r1)
    psq_l f29, 0x68(r1), 0, 0
    lfd f29, 0x60(r1)
    psq_l f28, 0x58(r1), 0, 0
    lfd f28, 0x50(r1)
    lmw r26, 0x38(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr

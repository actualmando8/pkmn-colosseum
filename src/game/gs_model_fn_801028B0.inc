    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r26, 0x18(r1)
    li r27, 0x1
    bl fn_801D2404
    bl fn_801046B8
    bl fn_80104704
    lis r3, lbl_80404ACC@ha
    mr r29, r3
    addi r3, r3, lbl_80404ACC@l
    bl fn_801038F8
    li r3, 0x0
    bl fn_800D9E4C
    bl fn_801093C8
    li r3, 0x1
    bl fn_800D9ED8
    lis r30, lbl_80404ACC@ha
    addi r30, r30, lbl_80404ACC@l
    lwz r28, 0xc(r30)
    b @L_80102D84
@L_80102904:
    lwz r3, 0x4(r28)
    bl fn_8005DA18
    lbz r0, 0x1(r28)
    mr r31, r3
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_80102998
    bge @L_80102934
    cmpwi r0, 0x0
    beq @L_80102944
    bge @L_80102958
    b @L_80102A10
@L_80102934:
    cmpwi r0, 0x4
    beq @L_80102A10
    bge @L_80102A10
    b @L_801029EC
@L_80102944:
    li r3, 0x1
    li r0, 0x0
    stb r3, 0x1(r28)
    stb r0, 0x2(r28)
    b @L_80102A10
@L_80102958:
    lwz r3, 0x4(r28)
    bl fn_801070F4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80102A10
    lbz r0, 0x1(r31)
    rlwinm r0, r0, 30, 29, 31
    cmplwi r0, 0x0
    bne @L_80102984
    li r0, 0x1
    stb r0, 0x98(r28)
@L_80102984:
    li r3, 0x2
    li r0, 0x0
    stb r3, 0x1(r28)
    stb r0, 0x2(r28)
    b @L_80102A10
@L_80102998:
    lbz r0, 0xa(r28)
    cmplwi r0, 0x0
    beq @L_80102A10
    lwz r3, 0x4(r28)
    bl fn_8005DA18
    lbz r0, 0x0(r3)
    rlwinm r3, r0, 0, 29, 31
    bl fn_8005D7F8
    cmplwi r3, 0x0
    beq @L_801029D8
    li r4, 0x5
    bl fn_8005D798
    rlwinm r3, r3, 0, 16, 31
    cmplwi r3, 0x0
    beq @L_801029D8
    bl fn_80166A28
@L_801029D8:
    li r3, 0x3
    li r0, 0x0
    stb r3, 0x1(r28)
    stb r0, 0x2(r28)
    b @L_80102A10
@L_801029EC:
    lwz r3, 0x4(r28)
    bl fn_801070F4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80102A10
    li r3, 0x4
    li r0, 0x0
    stb r3, 0x1(r28)
    stb r0, 0x2(r28)
@L_80102A10:
    cmplwi r29, 0x0
    beq @L_80102D30
    cmplw r28, r29
    bne @L_80102D30
    lbz r0, 0x98(r28)
    cmplwi r0, 0x0
    bne @L_80102C0C
    lbz r0, 0x1(r28)
    extsb r0, r0
    cmpwi r0, 0x3
    beq @L_80102B74
    bge @L_80102B74
    cmpwi r0, 0x2
    bge @L_80102A4C
    b @L_80102B74
@L_80102A4C:
    lbz r0, 0x1(r31)
    rlwinm r0, r0, 30, 29, 31
    cmpwi r0, 0x2
    beq @L_80102AF0
    bge @L_80102A70
    cmpwi r0, 0x0
    beq @L_80102A80
    bge @L_80102A8C
    b @L_80102AF0
@L_80102A70:
    cmpwi r0, 0x4
    beq @L_80102AE0
    bge @L_80102AF0
    b @L_80102ACC
@L_80102A80:
    li r0, 0x1
    stb r0, 0x98(r28)
    b @L_80102AF0
@L_80102A8C:
    cmplwi r28, 0x0
    beq @L_80102AF0
    bl fn_80105624
    lhz r3, 0x4(r3)
    rlwinm r0, r3, 0, 27, 27
    cmpwi r0, 0x0
    beq @L_80102AB0
    li r0, 0x1
    stb r0, 0x98(r28)
@L_80102AB0:
    rlwinm r0, r3, 0, 26, 26
    cmpwi r0, 0x0
    beq @L_80102AF0
    li r0, 0x1
    stb r0, 0x98(r28)
    stb r0, 0x99(r28)
    b @L_80102AF0
@L_80102ACC:
    lwz r3, 0x10(r31)
    li r4, 0x0
    crxor 6, 6, 6
    bl fn_800F7434
    b @L_80102AF0
@L_80102AE0:
    lwz r12, 0x10(r31)
    mr r3, r28
    mtctr r12
    bctrl
@L_80102AF0:
    lbz r0, 0x1b(r28)
    rlwinm r0, r0, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_80102B18
    lbz r0, 0x99(r28)
    cmplwi r0, 0x0
    beq @L_80102B18
    li r0, 0x0
    stb r0, 0x99(r28)
    stb r0, 0x98(r28)
@L_80102B18:
    lbz r0, 0x98(r28)
    cmplwi r0, 0x0
    beq @L_80102B38
    li r0, 0x0
    stw r0, 0x94(r30)
    lwz r0, 0x4(r28)
    stw r0, 0x98(r30)
    b @L_80102B74
@L_80102B38:
    bl fn_80105624
    lhz r0, 0x0(r3)
    rlwinm r0, r0, 0, 16, 16
    cmpwi r0, 0x0
    beq @L_80102B74
    li r3, 0x1
    stw r3, 0x94(r30)
    lwz r0, 0x4(r28)
    stw r0, 0x98(r30)
    lbz r0, 0x1b(r28)
    rlwinm r0, r0, 0, 27, 27
    cmpwi r0, 0x0
    bne @L_80102B74
    stb r3, 0x98(r28)
    stb r3, 0x99(r28)
@L_80102B74:
    lbz r0, 0x99(r28)
    cmplwi r0, 0x0
    beq @L_80102BB8
    lwz r3, 0x4(r28)
    bl fn_8005DA18
    lbz r0, 0x0(r3)
    rlwinm r3, r0, 0, 29, 31
    bl fn_8005D7F8
    cmplwi r3, 0x0
    beq @L_80102BF8
    li r4, 0x3
    bl fn_8005D798
    rlwinm r3, r3, 0, 16, 31
    cmplwi r3, 0x0
    beq @L_80102BF8
    bl fn_80166A28
    b @L_80102BF8
@L_80102BB8:
    lbz r0, 0x98(r28)
    cmplwi r0, 0x0
    beq @L_80102BF8
    lwz r3, 0x4(r28)
    bl fn_8005DA18
    lbz r0, 0x0(r3)
    rlwinm r3, r0, 0, 29, 31
    bl fn_8005D7F8
    cmplwi r3, 0x0
    beq @L_80102BF8
    li r4, 0x2
    bl fn_8005D798
    rlwinm r3, r3, 0, 16, 31
    cmplwi r3, 0x0
    beq @L_80102BF8
    bl fn_80166A28
@L_80102BF8:
    lbz r0, 0x98(r28)
    cmplwi r0, 0x0
    beq @L_80102C0C
    lwz r3, 0x4(r28)
    bl fn_80106F98
@L_80102C0C:
    lbz r0, 0x98(r28)
    cmplwi r0, 0x0
    bne @L_80102D30
    lbz r0, 0x1(r28)
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_80102C2C
    b @L_80102D30
@L_80102C2C:
    lhz r0, 0x94(r28)
    sth r0, 0x96(r28)
    lbz r0, 0x19(r28)
    extsb r0, r0
    cmpwi r0, 0x0
    ble @L_80102D30
    lbz r0, 0x1a(r28)
    cmpwi r0, 0x3
    beq @L_80102C78
    bge @L_80102C60
    cmpwi r0, 0x1
    bge @L_80102C6C
    b @L_80102C9C
@L_80102C60:
    cmpwi r0, 0x5
    bge @L_80102C9C
    b @L_80102C8C
@L_80102C6C:
    mr r3, r28
    bl fn_80102F38
    b @L_80102C9C
@L_80102C78:
    lwz r3, 0xc(r31)
    li r4, 0x0
    crxor 6, 6, 6
    bl fn_800F7434
    b @L_80102C9C
@L_80102C8C:
    lwz r12, 0xc(r31)
    mr r3, r28
    mtctr r12
    bctrl
@L_80102C9C:
    lhz r3, 0x96(r28)
    lhz r0, 0x94(r28)
    sth r3, 0xc(r1)
    sth r0, 0x8(r1)
    lbz r3, 0xc(r1)
    lbz r0, 0x8(r1)
    extsb r3, r3
    extsb r0, r0
    cmpw r3, r0
    bne @L_80102CE4
    lbz r3, 0xd(r1)
    lbz r0, 0x9(r1)
    extsb r3, r3
    extsb r0, r0
    cmpw r3, r0
    bne @L_80102CE4
    li r0, 0x0
    b @L_80102CE8
@L_80102CE4:
    li r0, 0x1
@L_80102CE8:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80102D30
    mr r3, r28
    bl fn_80104CA0
    lwz r3, 0x4(r28)
    bl fn_8005DA18
    lbz r0, 0x0(r3)
    rlwinm r3, r0, 0, 29, 31
    bl fn_8005D7F8
    cmplwi r3, 0x0
    beq @L_80102D30
    li r4, 0x1
    bl fn_8005D798
    rlwinm r3, r3, 0, 16, 31
    cmplwi r3, 0x0
    beq @L_80102D30
    bl fn_80166A28
@L_80102D30:
    lbz r0, 0x0(r28)
    extsb r0, r0
    rlwinm r0, r0, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_80102D80
    mr r3, r28
    bl fn_801071D0
    lwz r0, 0x14(r31)
    cmplwi r0, 0x0
    beq @L_80102D80
    li r26, 0x0
    b @L_80102D74
@L_80102D60:
    lwz r12, 0x14(r31)
    mr r3, r28
    mtctr r12
    bctrl
    addi r26, r26, 0x1
@L_80102D74:
    bl fn_800D3088
    cmplw r26, r3
    blt @L_80102D60
@L_80102D80:
    lwz r28, 0x10(r28)
@L_80102D84:
    cmplwi r28, 0x0
    bne @L_80102904
    lwz r28, 0xc(r30)
    b @L_80102DB0
@L_80102D94:
    lbz r0, 0x1(r28)
    extsb r0, r0
    cmpwi r0, 0x4
    bne @L_80102DAC
    mr r3, r28
    bl fn_8010474C
@L_80102DAC:
    lwz r28, 0x10(r28)
@L_80102DB0:
    cmplwi r28, 0x0
    bne @L_80102D94
    lwz r28, 0xc(r30)
    b @L_80102E78
@L_80102DC0:
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80102DEC
    lbz r0, 0x9(r28)
    extsb r0, r0
    cmpwi r0, 0x50
    blt @L_80102DEC
    li r27, 0x0
    bl fn_801C43F4
    li r3, 0x1
    bl fn_800D9ED8
@L_80102DEC:
    lbz r0, 0x0(r28)
    extsb r0, r0
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_80102E74
    lha r3, 0x84(r28)
    lha r4, 0x86(r28)
    bl fn_800FE6D0
    bl fn_800FE35C
    bl fn_800FE4D4
    lwz r29, 0x1c(r28)
    b @L_80102E2C
@L_80102E1C:
    mr r3, r28
    mr r4, r29
    bl fn_80108C14
    lwz r29, 0x0(r29)
@L_80102E2C:
    cmplwi r29, 0x0
    bne @L_80102E1C
    lwz r29, 0x20(r28)
    b @L_80102E4C
@L_80102E3C:
    mr r3, r28
    mr r4, r29
    bl fn_80108C14
    lwz r29, 0x0(r29)
@L_80102E4C:
    cmplwi r29, 0x0
    bne @L_80102E3C
    lwz r3, 0x4(r28)
    bl fn_8005DA18
    lwz r12, 0x18(r3)
    cmplwi r12, 0x0
    beq @L_80102E74
    mr r3, r28
    mtctr r12
    bctrl
@L_80102E74:
    lwz r28, 0x10(r28)
@L_80102E78:
    cmplwi r28, 0x0
    bne @L_80102DC0
    li r3, 0x0
    li r4, 0x0
    bl fn_800FE6D0
    bl fn_800FE35C
    li r3, 0x1
    li r4, 0x6
    li r5, 0x7
    bl fn_800DA4C4
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80102EB0
    bl fn_801C43F4
@L_80102EB0:
    li r3, 0x0
    bl fn_800D9ED8
    li r3, 0x1
    bl fn_800D9E4C
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

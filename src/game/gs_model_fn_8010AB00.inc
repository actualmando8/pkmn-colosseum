    nofralloc
    stwu r1, -0xa0(r1)
    mflr r0
    stw r0, 0xa4(r1)
    stfd f31, 0x90(r1)
    psq_st f31, 0x98(r1), 0, 0
    stfd f30, 0x80(r1)
    psq_st f30, 0x88(r1), 0, 0
    stw r31, 0x7c(r1)
    stw r30, 0x78(r1)
    stw r29, 0x74(r1)
    mr r31, r3
    cmplwi r31, 0x0
    bne @L_8010AB3C
    li r3, 0x0
    b @L_8010AE00
@L_8010AB3C:
    lbz r0, 0x14(r31)
    cmplwi r0, 0x0
    beq @L_8010ABDC
    lwz r3, 0x24(r31)
    bl fn_801DAC3C
    mr r0, r3
    lwz r3, 0x24(r31)
    mr r30, r0
    bl fn_801DAC24
    cmpwi r3, 0x1
    beq @L_8010ABAC
    bge @L_8010AB84
    cmpwi r3, -0x1
    beq @L_8010AB9C
    bge @L_8010ABA4
    cmpwi r3, -0x2
    bge @L_8010AB94
    b @L_8010ABC4
@L_8010AB84:
    cmpwi r3, 0x3
    beq @L_8010ABBC
    bge @L_8010ABC4
    b @L_8010ABB4
@L_8010AB94:
    lfs f31, lbl_8047CE84(r13)
    b @L_8010ABC8
@L_8010AB9C:
    lfs f31, lbl_8047CE88(r13)
    b @L_8010ABC8
@L_8010ABA4:
    lfs f31, lbl_8047CE8C(r13)
    b @L_8010ABC8
@L_8010ABAC:
    lfs f31, lbl_8047CE90(r13)
    b @L_8010ABC8
@L_8010ABB4:
    lfs f31, lbl_8047CE94(r13)
    b @L_8010ABC8
@L_8010ABBC:
    lfs f31, lbl_8047CE98(r13)
    b @L_8010ABC8
@L_8010ABC4:
    lfs f31, lbl_8047CE8C(r13)
@L_8010ABC8:
    lhz r3, 0x20(r31)
    addi r4, r1, 0xc
    addi r5, r1, 0x8
    bl fn_8025FA20
    b @L_8010ABE8
@L_8010ABDC:
    lwz r30, 0x24(r31)
    lfs f31, lbl_8047CE8C(r13)
    stfs f31, 0xc(r1)
@L_8010ABE8:
    cmplwi r30, 0x0
    bne @L_8010AC00
    li r0, 0x0
    li r3, 0x0
    stb r0, 0x0(r31)
    b @L_8010AE00
@L_8010AC00:
    mr r3, r30
    bl fn_800E6BC8
    mr r3, r30
    bl fn_800EE0E8
    mr r4, r3
    mr r3, r30
    subi r4, r4, 0x1
    bl fn_800EE150
    mr r29, r3
    cmplwi r29, 0x0
    beq @L_8010AC44
    addi r4, r1, 0x38
    li r5, 0x0
    li r6, 0x0
    bl fn_800EE3BC
    mr r3, r29
    bl fn_800EE828
@L_8010AC44:
    mr r3, r30
    bl fn_800E6B20
    mr r3, r30
    bl fn_800E3C5C
    addi r4, r1, 0x50
    bl fn_80190E34
    lwz r3, 0x38(r31)
    cmplwi r3, 0x0
    bne @L_8010AC70
    li r3, 0x0
    b @L_8010AE00
@L_8010AC70:
    addi r4, r1, 0x1c
    addi r5, r1, 0x18
    addi r6, r1, 0x14
    addi r7, r1, 0x10
    bl fn_800D1FDC
    lwz r4, 0x2c(r31)
    lis r3, 0x4330
    lwz r0, 0x30(r31)
    xoris r4, r4, 0x8000
    lfs f0, lbl_8047CE9C(r13)
    xoris r0, r0, 0x8000
    stw r4, 0x64(r1)
    lfd f3, lbl_8047CEB0(r13)
    stw r3, 0x60(r1)
    lfs f4, 0x50(r1)
    lfd f1, 0x60(r1)
    stw r0, 0x6c(r1)
    fsubs f2, f1, f3
    lfs f5, 0x54(r1)
    stw r3, 0x68(r1)
    fcmpo cr0, f4, f5
    lfd f1, 0x68(r1)
    stfs f0, 0x1c(r1)
    fsubs f0, f1, f3
    fdivs f0, f2, f0
    stfs f0, 0x18(r1)
    ble @L_8010ACE4
    fmr f30, f4
    b @L_8010ACE8
@L_8010ACE4:
    fmr f30, f5
@L_8010ACE8:
    lfs f1, lbl_8047CEA0(r13)
    bl fn_800CE220
    fdivs f3, f30, f31
    lfs f4, lbl_8047CE84(r13)
    lfs f0, 0xc(r1)
    addi r3, r1, 0x44
    lfs f2, 0x3c(r1)
    frsp f5, f1
    fmuls f3, f4, f3
    lfs f1, lbl_8047CE8C(r13)
    fdivs f3, f3, f5
    fmuls f3, f3, f0
    bl fn_800E01F4
    lwz r3, 0x38(r31)
    addi r4, r1, 0x44
    bl fn_800D207C
    lwz r3, 0x38(r31)
    lfs f1, 0x1c(r1)
    lfs f2, 0x18(r1)
    lfs f3, 0x14(r1)
    lfs f4, 0x10(r1)
    bl fn_800D20CC
    lis r3, lbl_8035B43C@ha
    lfs f1, lbl_8047CE80(r13)
    lfs f2, 0x3c(r1)
    addi r3, r3, lbl_8035B43C@l
    lfs f3, 0x40(r1)
    bl fn_800E01F4
    lis r4, lbl_8035B430@ha
    lis r5, lbl_8035B43C@ha
    addi r4, r4, lbl_8035B430@l
    lwz r3, 0x38(r31)
    addi r5, r5, lbl_8035B43C@l
    bl fn_800D1F04
    lis r8, lbl_80271F38@ha
    addi r3, r1, 0x20
    addi r8, r8, lbl_80271F38@l
    addi r4, r1, 0x44
    lwz r7, 0x0(r8)
    li r5, 0xc
    lwz r6, 0x4(r8)
    lwz r0, 0x8(r8)
    stw r7, 0x2c(r1)
    stw r6, 0x30(r1)
    stw r0, 0x34(r1)
    bl memcpy
    lfs f3, 0x20(r1)
    li r4, 0x2
    lfs f2, lbl_8047CEA4(r13)
    lfs f1, 0x24(r1)
    lfs f0, lbl_8047CEA8(r13)
    fsubs f2, f3, f2
    fadds f0, f1, f0
    stfs f2, 0x20(r1)
    stfs f0, 0x24(r1)
    lwz r3, 0x44(r31)
    bl fn_800DCCF0
    lwz r3, 0x44(r31)
    addi r4, r1, 0x2c
    bl fn_800DCC84
    lwz r3, 0x44(r31)
    addi r4, r1, 0x20
    bl fn_800DCC60
    lwz r3, 0x44(r31)
    addi r4, r1, 0x38
    bl fn_800DCC3C
    lwz r3, 0x44(r31)
    li r4, 0x1
    bl fn_800DCC34
    li r3, 0x1
@L_8010AE00:
    psq_l f31, 0x98(r1), 0, 0
    lfd f31, 0x90(r1)
    psq_l f30, 0x88(r1), 0, 0
    lfd f30, 0x80(r1)
    lwz r31, 0x7c(r1)
    lwz r30, 0x78(r1)
    lwz r0, 0xa4(r1)
    lwz r29, 0x74(r1)
    mtlr r0
    addi r1, r1, 0xa0
    blr

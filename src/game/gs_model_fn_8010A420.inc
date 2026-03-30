    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r31, r3
    cmplwi r31, 0x0
    bne @L_8010A450
    li r3, 0x0
    b @L_8010A59C
@L_8010A450:
    lwz r0, lbl_8047AD40(r13)
    cmpwi r0, 0x0
    bgt @L_8010A47C
    lis r3, lbl_80271F80@ha
    lis r4, lbl_8035B458@ha
    addi r3, r3, lbl_80271F80@l
    addi r4, r4, lbl_8035B458@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_8010A59C
@L_8010A47C:
    li r0, 0x0
    stb r0, 0x1(r31)
@L_8010A484:
    lwz r3, 0x28(r31)
    cmplwi r3, 0x0
    beq @L_8010A4B4
    bl fn_800F04BC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8010A4AC
    lwz r3, 0x28(r31)
    bl fn_800F0494
    b @L_8010A4B4
@L_8010A4AC:
    bl fn_800F0308
    b @L_8010A484
@L_8010A4B4:
    cmplwi r31, 0x0
    beq @L_8010A50C
    li r0, 0x0
    stb r0, 0x1(r31)
    lbz r0, 0x14(r31)
    cmplwi r0, 0x0
    beq @L_8010A4EC
    lwz r3, 0x24(r31)
    cmplwi r3, 0x0
    beq @L_8010A504
    bl fn_801DB100
    li r0, 0x0
    stw r0, 0x24(r31)
    b @L_8010A504
@L_8010A4EC:
    lwz r3, 0x24(r31)
    cmplwi r3, 0x0
    beq @L_8010A504
    bl fn_800E4BF4
    li r0, 0x0
    stw r0, 0x24(r31)
@L_8010A504:
    li r0, 0x0
    stb r0, 0x0(r31)
@L_8010A50C:
    mr r29, r31
    li r28, 0x0
    li r30, 0x0
@L_8010A518:
    lwz r3, 0x3c(r29)
    cmplwi r3, 0x0
    beq @L_8010A52C
    bl fn_800DCD98
    stw r30, 0x3c(r29)
@L_8010A52C:
    addi r29, r29, 0x4
    addi r28, r28, 0x1
    cmpwi r28, 0x3
    blt @L_8010A518
    lwz r3, 0x38(r31)
    cmplwi r3, 0x0
    beq @L_8010A554
    bl fn_800D2738
    li r0, 0x0
    stw r0, 0x38(r31)
@L_8010A554:
    lwz r3, 0x34(r31)
    cmplwi r3, 0x0
    beq @L_8010A56C
    bl fn_800EF5A4
    li r0, 0x0
    stw r0, 0x34(r31)
@L_8010A56C:
    lwz r3, lbl_8047AD40(r13)
    subi r0, r3, 0x1
    cmpwi r0, 0x0
    stw r0, lbl_8047AD40(r13)
    bne @L_8010A598
    lbz r0, lbl_8047AD44(r13)
    cmplwi r0, 0x0
    beq @L_8010A598
    bl fn_801DAC90
    li r0, 0x0
    stb r0, lbl_8047AD44(r13)
@L_8010A598:
    li r3, 0x1
@L_8010A59C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr


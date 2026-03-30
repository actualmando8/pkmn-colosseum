    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stmw r26, 0x78(r1)
    mr r27, r3
    mr r11, r4
    mr r28, r5
    mr r29, r6
    mr r30, r7
    bne cr1, @L_801026EC
    stfd f1, 0x28(r1)
    stfd f2, 0x30(r1)
    stfd f3, 0x38(r1)
    stfd f4, 0x40(r1)
    stfd f5, 0x48(r1)
    stfd f6, 0x50(r1)
    stfd f7, 0x58(r1)
    stfd f8, 0x60(r1)
@L_801026EC:
    stw r3, 0x8(r1)
    stw r4, 0xc(r1)
    stw r5, 0x10(r1)
    stw r6, 0x14(r1)
    stw r7, 0x18(r1)
    stw r8, 0x1c(r1)
    stw r9, 0x20(r1)
    stw r10, 0x24(r1)
    addi r6, r1, 0x98
    addi r0, r1, 0x8
    lis r3, 0x600
    addi r9, r1, 0x6c
    stw r3, 0x6c(r1)
    mr r3, r28
    mr r4, r27
    mr r5, r11
    stw r6, 0x70(r1)
    mr r6, r29
    mr r7, r8
    mr r8, r9
    stw r0, 0x74(r1)
    li r31, 0x0
    bl fn_80104E80
    rlwinm r0, r30, 0, 24, 31
    mr r26, r3
    cmplwi r0, 0x0
    beq @L_801027D4
    mr r3, r27
    mr r4, r30
    bl fn_801045A8
    mr r3, r27
    bl fn_801043A4
    mr r0, r3
    mr r3, r27
    mr r31, r0
    bl fn_8005DA18
    lbz r0, 0x3(r3)
    cmplwi r0, 0x0
    beq @L_8010279C
    lhz r0, 0x94(r26)
    addi r4, r1, 0x68
    sth r0, 0x68(r1)
    lbz r3, 0x3(r3)
    bl fn_80103EAC
@L_8010279C:
    cmplwi r28, 0x0
    beq @L_801027D4
    mr r3, r27
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_801027CC
    lbz r4, 0x95(r3)
    lbz r0, 0x94(r3)
    extsb r3, r4
    extsb r0, r0
    add r0, r3, r0
    b @L_801027D0
@L_801027CC:
    li r0, -0x1
@L_801027D0:
    stw r0, 0x0(r28)
@L_801027D4:
    rlwinm r0, r29, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_80102850
    mr r3, r27
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_80102850
    li r4, 0x0
    bl fn_80104828
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80102848
@L_80102804:
    mr r3, r27
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_80102850
    bl fn_800F037C
    cmplwi r3, 0x0
    bne @L_80102840
    lis r3, lbl_80271E10@ha
    lis r4, lbl_8035B060@ha
    addi r3, r3, lbl_80271E10@l
    mr r5, r27
    addi r4, r4, lbl_8035B060@l
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_80102850
@L_80102840:
    bl fn_800F0308
    b @L_80102804
@L_80102848:
    mr r3, r27
    bl fn_80104704
@L_80102850:
    mr r3, r31
    lmw r26, 0x78(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr

    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stmw r23, 0x4c(r1)
    mr r28, r3
    mr r29, r4
    mr r30, r5
    lis r4, lbl_80271F38@ha
    cmplwi r28, 0x0
    addi r31, r4, lbl_80271F38@l
    lwz r23, 0xc(r31)
    lwz r24, 0x10(r31)
    lwz r25, 0x14(r31)
    lwz r26, 0x18(r31)
    lwz r27, 0x1c(r31)
    lwz r12, 0x20(r31)
    lwz r11, 0x24(r31)
    lwz r10, 0x28(r31)
    lwz r9, 0x2c(r31)
    lwz r8, 0x30(r31)
    lwz r7, 0x34(r31)
    lwz r6, 0x38(r31)
    lwz r5, 0x3c(r31)
    lwz r4, 0x40(r31)
    lwz r0, 0x44(r31)
    stw r23, 0x38(r1)
    stw r24, 0x3c(r1)
    stw r25, 0x40(r1)
    stw r26, 0x2c(r1)
    stw r27, 0x30(r1)
    stw r12, 0x34(r1)
    stw r11, 0x20(r1)
    stw r10, 0x24(r1)
    stw r9, 0x28(r1)
    stw r8, 0x14(r1)
    stw r7, 0x18(r1)
    stw r6, 0x1c(r1)
    stw r5, 0x8(r1)
    stw r4, 0xc(r1)
    stw r0, 0x10(r1)
    bne @L_8010A668
    li r3, 0x0
    b @L_8010A878
@L_8010A668:
    li r4, 0x0
    li r5, 0x48
    bl memset
    lwz r0, lbl_8047AD40(r13)
    cmpwi r0, 0x4
    blt @L_8010A69C
    lis r4, lbl_8035B448@ha
    addi r3, r31, 0x70
    addi r4, r4, lbl_8035B448@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_8010A878
@L_8010A69C:
    stw r29, 0x2c(r28)
    cmpwi r29, 0x100
    stw r30, 0x30(r28)
    bge @L_8010A6B0
    li r29, 0x100
@L_8010A6B0:
    cmpwi r29, 0x280
    ble @L_8010A6BC
    li r29, 0x280
@L_8010A6BC:
    cmpwi r30, 0x100
    bge @L_8010A6C8
    li r30, 0x100
@L_8010A6C8:
    cmpwi r30, 0x1e0
    ble @L_8010A6D4
    li r30, 0x1e0
@L_8010A6D4:
    rlwinm r3, r29, 0, 16, 31
    rlwinm r4, r30, 0, 16, 31
    li r5, 0x45
    li r6, 0x0
    li r7, 0x0
    bl fn_800EF5FC
    stw r3, 0x34(r28)
    lwz r3, 0x34(r28)
    cmplwi r3, 0x0
    bne @L_8010A718
    lis r4, lbl_8035B448@ha
    addi r3, r31, 0x8c
    addi r4, r4, lbl_8035B448@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_8010A878
@L_8010A718:
    li r4, 0x2
    li r5, 0x2
    li r6, 0x0
    bl fn_800EF578
    bl fn_800D29A0
    stw r3, 0x38(r28)
    lwz r3, 0x38(r28)
    cmplwi r3, 0x0
    bne @L_8010A758
    lis r4, lbl_8035B448@ha
    addi r3, r31, 0xac
    addi r4, r4, lbl_8035B448@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_8010A878
@L_8010A758:
    addi r4, r1, 0x38
    bl fn_800D207C
    lwz r3, 0x38(r28)
    addi r4, r1, 0x2c
    bl fn_800D203C
    mr r23, r28
    li r24, 0x0
@L_8010A774:
    bl fn_800DCFBC
    mr r25, r3
    cmplwi r25, 0x0
    beq @L_8010A844
    cmpwi r24, 0x1
    beq @L_8010A7C0
    bge @L_8010A79C
    cmpwi r24, 0x0
    bge @L_8010A7A8
    b @L_8010A834
@L_8010A79C:
    cmpwi r24, 0x3
    bge @L_8010A834
    b @L_8010A7F0
@L_8010A7A8:
    li r4, 0x0
    bl fn_800DCCF0
    mr r3, r25
    addi r4, r1, 0x8
    bl fn_800DCC84
    b @L_8010A834
@L_8010A7C0:
    li r4, 0x2
    bl fn_800DCCF0
    mr r3, r25
    addi r4, r1, 0x14
    bl fn_800DCC84
    mr r3, r25
    addi r4, r1, 0x38
    bl fn_800DCC60
    mr r3, r25
    addi r4, r1, 0x20
    bl fn_800DCC3C
    b @L_8010A834
@L_8010A7F0:
    lfs f1, lbl_8047CE74(r13)
    addi r3, r1, 0x38
    fmr f2, f1
    fmr f3, f1
    bl fn_800E01F4
    mr r3, r25
    li r4, 0x2
    bl fn_800DCCF0
    mr r3, r25
    addi r4, r1, 0x14
    bl fn_800DCC84
    mr r3, r25
    addi r4, r1, 0x38
    bl fn_800DCC60
    mr r3, r25
    addi r4, r1, 0x20
    bl fn_800DCC3C
@L_8010A834:
    mr r3, r25
    li r4, 0x0
    bl fn_800DCC34
    stw r25, 0x3c(r23)
@L_8010A844:
    addi r23, r23, 0x4
    addi r24, r24, 0x1
    cmpwi r24, 0x3
    blt @L_8010A774
    lwz r0, lbl_8047AD40(r13)
    cmpwi r0, 0x0
    bne @L_8010A868
    li r0, 0x0
    stb r0, lbl_8047AD44(r13)
@L_8010A868:
    lwz r4, lbl_8047AD40(r13)
    li r3, 0x1
    addi r0, r4, 0x1
    stw r0, lbl_8047AD40(r13)
@L_8010A878:
    lmw r23, 0x4c(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr


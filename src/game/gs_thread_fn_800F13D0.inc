    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stw r31, 0x3c(r1)
    mr r31, r3
    stw r30, 0x38(r1)
    stw r29, 0x34(r1)
    stw r28, 0x30(r1)
    lwz r3, 0x14(r3)
    addi r0, r3, 0x2
    stw r0, 0x14(r31)
    lwz r3, 0x14(r31)
    lhz r29, 0x0(r3)
    addi r0, r3, 0x2
    stw r0, 0x14(r31)
    lwz r4, 0x28(r31)
    lwz r5, 0x1c(r31)
    cmpwi r4, 0x40
    ble @L_800F1430
    lis r3, lbl_80271068@ha
    addi r3, r3, lbl_80271068@l
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F1444
    @L_800F1430:
    addi r3, r4, 0x1
    slwi r0, r4, 2
    stw r3, 0x28(r31)
    add r3, r31, r0
    stw r5, 0x6c(r3)
    @L_800F1444:
    lwz r4, 0x28(r31)
    cmpwi r4, 0x40
    ble @L_800F1464
    lis r3, lbl_80271068@ha
    addi r3, r3, lbl_80271068@l
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F1478
    @L_800F1464:
    addi r3, r4, 0x1
    slwi r0, r4, 2
    stw r3, 0x28(r31)
    add r3, r31, r0
    stw r29, 0x6c(r3)
    @L_800F1478:
    lwz r3, 0x28(r31)
    addi r4, r29, 0x2
    li r0, 0x2
    li r30, 0x0
    subf r3, r4, r3
    li r29, 0x3
    stw r3, 0x1c(r31)
    lwz r3, 0x1c(r31)
    slwi r3, r3, 2
    addi r3, r3, 0x6c
    lwzx r28, r31, r3
    stb r0, 0x4(r31)
    b @L_800F15B4
    @L_800F14AC:
    lbz r0, 0x4(r31)
    cmplwi r0, 0x4
    bne @L_800F14BC
    stb r29, 0x4(r31)
    @L_800F14BC:
    lbz r0, 0x4(r31)
    cmplwi r0, 0x3
    bne @L_800F15A8
    lwz r3, 0x28(r31)
    cmpwi r3, 0x0
    bgt @L_800F14F0
    lis r3, lbl_8027107C@ha
    addi r3, r3, lbl_8027107C@l
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r31)
    stw r0, 0x14(r1)
    b @L_800F1508
    @L_800F14F0:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r31)
    add r3, r31, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x14(r1)
    @L_800F1508:
    lwz r3, 0x28(r31)
    lwz r0, 0x14(r1)
    cmpwi r3, 0x0
    stw r0, 0x20(r1)
    bgt @L_800F1538
    lis r3, lbl_8027107C@ha
    addi r3, r3, lbl_8027107C@l
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r31)
    stw r0, 0x10(r1)
    b @L_800F1550
    @L_800F1538:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r31)
    add r3, r31, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x10(r1)
    @L_800F1550:
    lwz r0, 0x10(r1)
    lis r3, lbl_8027107C@ha
    lwz r29, 0x20(r1)
    addi r30, r3, lbl_8027107C@l
    stw r0, 0x24(r1)
    li r28, 0x0
    stw r0, 0x1c(r31)
    b @L_800F1598
    @L_800F1570:
    lwz r3, 0x28(r31)
    cmpwi r3, 0x0
    bgt @L_800F158C
    mr r3, r30
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F1594
    @L_800F158C:
    subi r0, r3, 0x1
    stw r0, 0x28(r31)
    @L_800F1594:
    addi r28, r28, 0x1
    @L_800F1598:
    cmpw r28, r29
    blt @L_800F1570
    li r3, 0x0
    b @L_800F16A0
    @L_800F15A8:
    bl fn_800F0308
    bl fn_800D3088
    add r30, r30, r3
    @L_800F15B4:
    cmpw r30, r28
    blt @L_800F14AC
    li r0, 0x1
    stb r0, 0x4(r31)
    lwz r3, 0x28(r31)
    cmpwi r3, 0x0
    bgt @L_800F15EC
    lis r3, lbl_8027107C@ha
    addi r3, r3, lbl_8027107C@l
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r31)
    stw r0, 0xc(r1)
    b @L_800F1604
    @L_800F15EC:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r31)
    add r3, r31, r0
    lwz r0, 0x6c(r3)
    stw r0, 0xc(r1)
    @L_800F1604:
    lwz r3, 0x28(r31)
    lwz r0, 0xc(r1)
    cmpwi r3, 0x0
    stw r0, 0x18(r1)
    bgt @L_800F1634
    lis r3, lbl_8027107C@ha
    addi r3, r3, lbl_8027107C@l
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r31)
    stw r0, 0x8(r1)
    b @L_800F164C
    @L_800F1634:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r31)
    add r3, r31, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x8(r1)
    @L_800F164C:
    lwz r0, 0x8(r1)
    lis r3, lbl_8027107C@ha
    lwz r30, 0x18(r1)
    addi r29, r3, lbl_8027107C@l
    stw r0, 0x1c(r1)
    li r28, 0x0
    stw r0, 0x1c(r31)
    b @L_800F1694
    @L_800F166C:
    lwz r3, 0x28(r31)
    cmpwi r3, 0x0
    bgt @L_800F1688
    mr r3, r29
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F1690
    @L_800F1688:
    subi r0, r3, 0x1
    stw r0, 0x28(r31)
    @L_800F1690:
    addi r28, r28, 0x1
    @L_800F1694:
    cmpw r28, r30
    blt @L_800F166C
    li r3, 0x1
    @L_800F16A0:
    lwz r0, 0x44(r1)
    lwz r31, 0x3c(r1)
    lwz r30, 0x38(r1)
    lwz r29, 0x34(r1)
    lwz r28, 0x30(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr

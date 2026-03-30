    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    beq @L_80138DB0
    mr r30, r3
    lhz r28, 0x38(r3)
    cmplwi r28, 0x20
    ble @L_80138D0C
    li r0, 0x20
    li r28, 0x20
    sth r0, 0x38(r30)
    @L_80138D0C:
    lhz r3, 0x40(r30)
    lhz r4, 0x44(r30)
    bl fn_800F9318
    stw r3, 0x8(r30)
    lwz r0, 0x8(r30)
    cmplwi r0, 0x0
    beq @L_80138D90
    mulli r31, r28, 0x5c
    mr r3, r31
    bl fn_800E3534
    clrlwi. r0, r3, 16
    sth r3, 0x4(r30)
    beq @L_80138DB0
    bl fn_800E27B0
    stw r3, 0x0(r30)
    mr r5, r31
    mr r31, r3
    li r4, 0x0
    lwz r3, 0x0(r30)
    bl memset
    li r0, 0x0
    li r29, 0x0
    sth r0, 0x3c(r30)
    b @L_80138D80
    @L_80138D6C:
    mr r3, r30
    mr r4, r31
    bl fn_80139378
    addi r29, r29, 0x1
    addi r31, r31, 0x5c
    @L_80138D80:
    cmplw r29, r28
    blt @L_80138D6C
    li r3, 0x1
    b @L_80138DC4
    @L_80138D90:
    lis r3, lbl_80272C30@ha
    lhz r4, 0x40(r30)
    lhz r5, 0x44(r30)
    addi r3, r3, lbl_80272C30@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_80138DC4
    @L_80138DB0:
    lis r3, lbl_80272C90@ha
    addi r3, r3, lbl_80272C90@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    @L_80138DC4:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

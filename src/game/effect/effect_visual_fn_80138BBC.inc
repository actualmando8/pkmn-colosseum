    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr. r28, r3
    beq @L_80138CB4
    lhz r26, 0x38(r28)
    li r25, 0x0
    lwz r27, 0x0(r28)
    li r30, 0x0
    lhz r24, 0x4(r28)
    b @L_80138C14
    @L_80138BEC:
    lwz r3, 0x58(r27)
    cmplwi r3, 0x0
    beq @L_80138C0C
    li r4, 0x0
    bl fn_800E4014
    lwz r3, 0x58(r27)
    bl fn_800E4BF4
    stw r30, 0x58(r27)
    @L_80138C0C:
    addi r25, r25, 0x1
    addi r27, r27, 0x5c
    @L_80138C14:
    cmplw r25, r26
    blt @L_80138BEC
    lwz r25, 0x8(r28)
    mr r3, r25
    bl fn_800EE0E8
    mr r30, r3
    li r28, 0x0
    b @L_80138C94
    @L_80138C34:
    mr r3, r25
    mr r4, r28
    bl fn_800EE150
    mr. r26, r3
    beq @L_80138C90
    bl fn_800EE758
    mr r31, r3
    li r29, 0x0
    b @L_80138C80
    @L_80138C58:
    mr r3, r26
    mr r4, r29
    bl fn_800EE6B4
    mr. r27, r3
    beq @L_80138C7C
    lfs f1, lbl_8047D160(r2)
    bl fn_800DF21C
    mr r3, r27
    bl fn_800DF608
    @L_80138C7C:
    addi r29, r29, 0x1
    @L_80138C80:
    cmplw r29, r31
    blt @L_80138C58
    mr r3, r26
    bl fn_800EE828
    @L_80138C90:
    addi r28, r28, 0x1
    @L_80138C94:
    cmplw r28, r30
    blt @L_80138C34
    cmplwi r24, 0x0
    beq @L_80138CB4
    mr r3, r24
    bl fn_800E24B0
    mr r3, r24
    bl fn_800E209C
    @L_80138CB4:
    lmw r24, 0x10(r1)
    li r3, 0x1
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

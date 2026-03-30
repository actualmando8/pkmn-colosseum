    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r23, 0xc(r1)
    mr. r31, r3
    beq @L_8019B73C
    lwz r30, 0x0(r31)
    cmplwi r30, 0x0
    beq @L_8019B724
    lwz r29, 0x0(r30)
    cmplwi r29, 0x0
    beq @L_8019B70C
    lwz r28, 0x0(r29)
    cmplwi r28, 0x0
    beq @L_8019B6F4
    lwz r27, 0x0(r28)
    cmplwi r27, 0x0
    beq @L_8019B6DC
    lwz r26, 0x0(r27)
    cmplwi r26, 0x0
    beq @L_8019B6C4
    lwz r25, 0x0(r26)
    cmplwi r25, 0x0
    beq @L_8019B6AC
    lwz r24, 0x0(r25)
    cmplwi r24, 0x0
    beq @L_8019B680
    lwz r23, 0x0(r24)
    cmplwi r23, 0x0
    beq @L_8019B670
    lwz r3, 0x0(r23)
    bl fn_8019B5E8
    mr r3, r23
    bl fn_8019B750
    @L_8019B670:
    cmplwi r24, 0x0
    beq @L_8019B680
    mr r3, r24
    bl fn_8019970C
    @L_8019B680:
    cmplwi r25, 0x0
    beq @L_8019B6AC
    lis r3, lbl_80465378@ha
    mr r4, r25
    addi r3, r3, lbl_80465378@l
    bl fn_801AA498
    lis r3, lbl_80465378@ha
    lis r4, lbl_802747AC@ha
    addi r3, r3, lbl_80465378@l
    addi r4, r4, lbl_802747AC@l
    bl fn_80199A84
    @L_8019B6AC:
    cmplwi r26, 0x0
    beq @L_8019B6C4
    lis r3, lbl_80465378@ha
    mr r4, r26
    addi r3, r3, lbl_80465378@l
    bl fn_801AA498
    @L_8019B6C4:
    cmplwi r27, 0x0
    beq @L_8019B6DC
    lis r3, lbl_80465378@ha
    mr r4, r27
    addi r3, r3, lbl_80465378@l
    bl fn_801AA498
    @L_8019B6DC:
    cmplwi r28, 0x0
    beq @L_8019B6F4
    lis r3, lbl_80465378@ha
    mr r4, r28
    addi r3, r3, lbl_80465378@l
    bl fn_801AA498
    @L_8019B6F4:
    cmplwi r29, 0x0
    beq @L_8019B70C
    lis r3, lbl_80465378@ha
    mr r4, r29
    addi r3, r3, lbl_80465378@l
    bl fn_801AA498
    @L_8019B70C:
    cmplwi r30, 0x0
    beq @L_8019B724
    lis r3, lbl_80465378@ha
    mr r4, r30
    addi r3, r3, lbl_80465378@l
    bl fn_801AA498
    @L_8019B724:
    cmplwi r31, 0x0
    beq @L_8019B73C
    lis r3, lbl_80465378@ha
    mr r4, r31
    addi r3, r3, lbl_80465378@l
    bl fn_801AA498
    @L_8019B73C:
    lmw r23, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x44(r1)
    stmw r27, 0x2c(r1)
    beq @L_80139AAC
    mr r31, r3
    bl fn_800D2584
    mr r30, r3
    lhz r29, 0x44(r31)
    lwz r28, 0x0(r31)
    lhz r3, 0x4c(r31)
    lhz r4, 0x4e(r31)
    bl fn_800F9318
    lwz r0, 0x58(r31)
    mr r27, r3
    cmplwi r0, 0x0
    bne @L_80139984
    li r3, 0x0
    b @L_80139AB0
    @L_80139984:
    bl fn_800D2248
    cmplwi r27, 0x0
    beq @L_801399E4
    mr r3, r27
    addi r4, r1, 0x8
    bl fn_800E3D98
    lwz r4, 0x50(r31)
    cmpwi r4, 0x0
    ble @L_801399F8
    mr r3, r27
    bl fn_800EE150
    mr. r27, r3
    beq @L_801399F8
    bl fn_800EE7E0
    clrlwi. r0, r3, 24
    beq @L_801399F8
    mr r3, r27
    addi r4, r1, 0x8
    li r5, 0x0
    li r6, 0x0
    bl fn_800EE3BC
    mr r3, r27
    bl fn_800EE828
    b @L_801399F8
    @L_801399E4:
    lfs f1, lbl_8047D190(r2)
    addi r3, r1, 0x8
    fmr f2, f1
    fmr f3, f1
    bl fn_800E01F4
    @L_801399F8:
    mr r3, r30
    addi r4, r1, 0x14
    bl fn_800D1F84
    li r3, 0x1
    li r4, 0x6
    li r5, 0x1
    bl fn_800DA4C4
    li r3, 0x1
    li r4, 0x1
    li r5, 0x0
    bl fn_800DA2BC
    li r3, 0x1
    li r4, 0x1
    li r5, 0x1
    bl fn_800DA1E8
    li r3, 0x0
    bl fn_800DA028
    li r3, 0x3
    bl fn_800D88DC
    li r3, 0x4
    bl fn_800D888C
    lwz r3, 0x54(r31)
    bl fn_800D7820
    lwz r4, 0x58(r31)
    li r3, 0x0
    bl fn_800D85D4
    li r3, 0x6
    bl fn_800D6A00
    li r30, 0x0
    b @L_80139A98
    @L_80139A70:
    lhz r0, 0x12e0(r28)
    cmplwi r0, 0x0
    beq @L_80139A90
    mr r3, r28
    mr r4, r31
    addi r5, r1, 0x14
    addi r6, r1, 0x8
    bl fn_80139E80
    @L_80139A90:
    addi r30, r30, 0x1
    addi r28, r28, 0x12e4
    @L_80139A98:
    clrlwi r0, r30, 16
    cmplw r0, r29
    blt @L_80139A70
    li r3, 0x1
    b @L_80139AB0
    @L_80139AAC:
    li r3, 0x0
    @L_80139AB0:
    lmw r27, 0x2c(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr

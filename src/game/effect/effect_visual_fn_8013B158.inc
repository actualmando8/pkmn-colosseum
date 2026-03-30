    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr. r25, r3
    beq @L_8013B240
    lwz r3, 0x50(r25)
    cmplwi r3, 0x0
    bne @L_8013B184
    li r3, 0x0
    b @L_8013B254
    @L_8013B184:
    lbz r0, 0x4c(r25)
    cmplwi r0, 0x0
    beq @L_8013B1FC
    lwz r0, 0x48(r25)
    lis r3, lbl_80363CA8@ha
    lbz r29, 0x4e(r25)
    addi r26, r25, 0x4
    lbz r30, 0x4f(r25)
    clrlwi r27, r0, 16
    addi r31, r3, lbl_80363CA8@l
    li r28, 0x0
    b @L_8013B1EC
    @L_8013B1B4:
    lwz r3, 0x0(r26)
    cmplwi r3, 0x0
    beq @L_8013B1E4
    cmplwi r29, 0x0
    beq @L_8013B1D0
    mr r4, r31
    bl fn_800E6478
    @L_8013B1D0:
    cmplwi r30, 0x0
    bne @L_8013B1E4
    lwz r3, 0x0(r26)
    lwz r4, 0x50(r25)
    bl fn_800E5BE0
    @L_8013B1E4:
    addi r28, r28, 0x1
    addi r26, r26, 0x4
    @L_8013B1EC:
    clrlwi r0, r28, 16
    cmplw r0, r27
    blt @L_8013B1B4
    b @L_8013B228
    @L_8013B1FC:
    bl fn_80168570
    stw r3, 0x0(r25)
    lwz r0, 0x0(r25)
    cmplwi r0, 0x0
    bne @L_8013B228
    lis r3, lbl_80272E30@ha
    addi r3, r3, lbl_80272E30@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_8013B254
    @L_8013B228:
    lwz r4, 0x50(r25)
    li r0, 0x0
    li r3, 0x1
    stw r4, 0x54(r25)
    stw r0, 0x58(r25)
    b @L_8013B254
    @L_8013B240:
    lis r3, lbl_80272E70@ha
    addi r3, r3, lbl_80272E70@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    @L_8013B254:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

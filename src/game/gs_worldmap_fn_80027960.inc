    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r26, r3
    mr r27, r4
    li r28, 0x0
    li r31, 0x0
    @L_80027980:
    la r29, lbl_8047B920(r2)
    lwzx r3, r29, r31
    cmplwi r3, 0x0
    beq @L_800279DC
    bl fn_800FA314
    mr r30, r3
    lwzx r3, r29, r31
    bl fn_800FA280
    addi r5, r3, 0x2
    li r4, 0x1
    srwi r0, r30, 1
    clrlwi r3, r26, 16
    mtctr r0
    cmpwi r30, 0x1
    ble @L_800279D4
    @L_800279BC:
    lhz r0, 0x0(r5)
    cmplw r0, r3
    beq @L_800279D4
    addi r4, r4, 0x2
    addi r5, r5, 0x4
    bdnz @L_800279BC
    @L_800279D4:
    cmpw r4, r30
    blt @L_800279EC
    @L_800279DC:
    addi r28, r28, 0x1
    addi r31, r31, 0x4
    cmpwi r28, 0x2
    blt @L_80027980
    @L_800279EC:
    cmpwi r28, 0x2
    blt @L_800279F8
    li r28, 0x0
    @L_800279F8:
    cmpw r28, r27
    bne @L_80027A08
    mr r3, r26
    b @L_80027A90
    @L_80027A08:
    cmpwi r28, 0x1
    beq @L_80027A14
    bne @L_80027A18
    @L_80027A14:
    lhz r26, -0x2(r5)
    @L_80027A18:
    cmpwi r27, 0x0
    bne @L_80027A28
    mr r3, r26
    b @L_80027A90
    @L_80027A28:
    slwi r30, r27, 2
    la r29, lbl_8047B920(r2)
    lwzx r3, r29, r30
    bl fn_800FA314
    mr r31, r3
    lwzx r3, r29, r30
    bl fn_800FA280
    li r4, 0x0
    mr r5, r3
    addi r0, r31, 0x1
    clrlwi r3, r26, 16
    srwi r0, r0, 1
    mtctr r0
    cmpwi r31, 0x0
    ble @L_80027A7C
    @L_80027A64:
    lhz r0, 0x0(r5)
    cmplw r0, r3
    beq @L_80027A7C
    addi r4, r4, 0x2
    addi r5, r5, 0x4
    bdnz @L_80027A64
    @L_80027A7C:
    cmpw r4, r31
    blt @L_80027A8C
    li r3, 0x0
    b @L_80027A90
    @L_80027A8C:
    lhz r3, 0x2(r5)
    @L_80027A90:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

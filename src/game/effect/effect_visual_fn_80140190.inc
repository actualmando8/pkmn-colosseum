    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmplwi r4, 0x0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r31, r3
    mr r27, r5
    li r30, 0x0
    bne @L_801401BC
    li r3, 0x0
    b @L_80140298
    @L_801401BC:
    mr r3, r4
    li r4, 0x2
    bl fn_800EE150
    mr. r29, r3
    bne @L_801401D8
    li r3, 0x0
    b @L_80140298
    @L_801401D8:
    li r0, 0x0
    li r4, 0x0
    stw r0, 0x0(r31)
    stw r0, 0x4(r31)
    bl fn_800EE6B4
    stw r3, 0x0(r31)
    lwz r0, 0x0(r31)
    cmplwi r0, 0x0
    beq @L_80140254
    lis r4, 0x1056
    li r3, 0x387
    addi r4, r4, 0x1200
    bl fn_800F9318
    mr. r28, r3
    beq @L_80140254
    lis r4, 0x1057
    li r3, 0x387
    addi r4, r4, 0x1200
    bl fn_800F9318
    mr. r4, r3
    beq @L_80140254
    mr r3, r28
    mr r5, r27
    bl fn_801402AC
    stw r3, 0x4(r31)
    lwz r4, 0x4(r31)
    cmplwi r4, 0x0
    beq @L_80140254
    lwz r3, 0x0(r31)
    bl fn_800DF028
    li r30, 0x1
    @L_80140254:
    clrlwi. r0, r30, 24
    bne @L_8014028C
    lwz r3, 0x4(r31)
    cmplwi r3, 0x0
    beq @L_80140274
    bl fn_800EF5A4
    li r0, 0x0
    stw r0, 0x4(r31)
    @L_80140274:
    lwz r3, 0x0(r31)
    cmplwi r3, 0x0
    beq @L_8014028C
    bl fn_800DF608
    li r0, 0x0
    stw r0, 0x0(r31)
    @L_8014028C:
    mr r3, r29
    bl fn_800EE828
    mr r3, r30
    @L_80140298:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

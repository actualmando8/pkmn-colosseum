    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    mr r31, r7
    li r26, 0x0
    b @L_8018383C
    @L_80183804:
    mr r3, r26
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80183838
    lwz r0, 0x28(r3)
    cmplw r0, r27
    bne @L_80183838
    lwz r0, 0x2c(r3)
    cmplw r0, r28
    bne @L_80183838
    lwz r26, 0x4(r3)
    b @L_801838A8
    @L_80183838:
    addi r26, r26, 0x1
    @L_8018383C:
    bl fn_8018FDB4
    cmpw r26, r3
    blt @L_80183804
    li r25, 0x0
    b @L_80183898
    @L_80183850:
    mr r3, r25
    bl fn_8018FD88
    mr r26, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80183894
    lwz r0, 0x2c(r26)
    cmplw r0, r28
    bne @L_80183894
    lis r3, lbl_80273FD8@ha
    mr r4, r27
    addi r3, r3, lbl_80273FD8@l
    mr r5, r28
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r26, 0x4(r26)
    b @L_801838A8
    @L_80183894:
    addi r25, r25, 0x1
    @L_80183898:
    bl fn_8018FDB4
    cmpw r25, r3
    blt @L_80183850
    li r26, 0x0
    @L_801838A8:
    li r25, 0x0
    b @L_801838DC
    @L_801838B0:
    mr r3, r25
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801838D8
    lwz r0, 0x4(r3)
    cmplw r0, r26
    bne @L_801838D8
    mr r26, r3
    b @L_801838EC
    @L_801838D8:
    addi r25, r25, 0x1
    @L_801838DC:
    bl fn_8018FDB4
    cmpw r25, r3
    blt @L_801838B0
    li r26, 0x0
    @L_801838EC:
    cmplwi r26, 0x0
    bne @L_801838FC
    li r3, 0x0
    b @L_80183944
    @L_801838FC:
    cmplwi r29, 0x0
    bne @L_8018390C
    li r3, 0x0
    b @L_80183944
    @L_8018390C:
    stw r30, 0x8(r1)
    mr r4, r29
    mr r9, r27
    mr r10, r28
    stw r31, 0xc(r1)
    li r3, 0xf
    li r5, 0x1000
    li r6, 0x1
    li r7, 0x0
    li r8, 0x4
    crxor 6, 6, 6
    bl fn_800F7318
    sth r3, 0x4c(r26)
    li r3, 0x1
    @L_80183944:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

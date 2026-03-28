    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    lis r3, -32738
    lis r4, -32697
    stw r0, 0x54(r1)
    li r7, 0x3e8
    li r9, 0x0
    addi r3, r3, 9740
    stmw r24, 0x30(r1)
    addi r30, r4, -21408
    lis r4, 0x3
    li r0, 0x1
    li r10, 0x0
    lwz r6, 0x90(r30)
    lwz r5, 0x8c(r30)
    mulli r8, r6, 0x28
    stw r9, 0x8(r1)
    subi r5, r5, 0x2
    divwu r29, r8, r7
    stw r9, 0xc(r1)
    subic r7, r5, 0x1
    stw r4, 0x10(r1)
    li r5, 0x40
    subfe r4, r7, r7
    stw r3, 0x14(r1)
    andc r8, r5, r4
    mr r5, r29
    li r3, 0xff
    stw r0, 0x18(r1)
    li r7, 0x7f
    stw r9, 0x1c(r1)
    li r9, 0x0
    lwz r4, -21424(r13)
    bl fn_8014EE40
    addis r0, r3, 0x1
    stw r3, -31520(r13)
    cmplwi r0, 0xffff
    bne @L_801E3590
    li r3, 0x0
    b @L_801E3844
@L_801E3590:
    lwz r0, 0x8c(r30)
    cmplwi r0, 0x2
    bne @L_801E3608
    li r6, 0x0
    lis r3, -32738
    stw r6, 0x8(r1)
    lis r4, 0x3
    addi r0, r3, 9740
    mr r5, r29
    stw r6, 0xc(r1)
    li r3, 0xff
    li r7, 0x7f
    li r8, 0x7f
    stw r4, 0x10(r1)
    li r9, 0x0
    li r10, 0x0
    stw r0, 0x14(r1)
    stw r6, 0x18(r1)
    stw r6, 0x1c(r1)
    lwz r4, -21420(r13)
    lwz r6, 0x90(r30)
    bl fn_8014EE40
    addis r0, r3, 0x1
    stw r3, -31516(r13)
    cmplwi r0, 0xffff
    bne @L_801E3608
    lwz r3, -31520(r13)
    bl fn_8014F838
    li r3, 0x0
    b @L_801E3844
@L_801E3608:
    lwz r0, 0x8c(r30)
    lis r3, -32697
    addi r31, r3, -23488
    li r24, 0x0
    cmplwi r0, 0x2
    stw r24, 0x28(r31)
    stw r24, 0x2c(r31)
    stw r24, 0x34(r31)
    stw r24, 0x30(r31)
    stw r24, 0x3c(r31)
    stw r24, 0x38(r31)
    stw r24, 0x44(r31)
    stw r24, 0x40(r31)
    bne @L_801E3718
    li r25, 0x0
    lwz r27, -21424(r13)
    lwz r28, -21420(r13)
    mr r26, r29
    mr r24, r25
@L_801E3654:
    mr r3, r27
    mr r4, r28
    mr r5, r26
    addi r6, r1, 0x24
    bl fn_801E2B74
    lwz r0, 0x24(r1)
    addc r24, r24, r3
    addze r25, r25
    cmpwi r0, 0x0
    beq @L_801E36FC
    cmpwi r0, 0x1
    bne @L_801E36D0
    cmplwi r28, 0x0
    slwi r0, r3, 1
    subf r26, r3, r26
    add r27, r27, r0
    beq @L_801E369C
    add r28, r28, r0
@L_801E369C:
    lwz r0, 0x2c(r31)
    slwi r0, r0, 3
    add r3, r31, r0
    stw r24, 0x4(r3)
    stw r25, 0x0(r3)
    lwz r3, 0x2c(r31)
    addi r0, r3, 0x1
    cmpwi r0, 0x5
    stw r0, 0x2c(r31)
    blt @L_801E3654
    li r0, 0x0
    stw r0, 0x2c(r31)
    b @L_801E3654
@L_801E36D0:
    slwi r24, r26, 1
    mr r3, r27
    mr r5, r24
    li r4, 0x0
    bl memset
    cmplwi r28, 0x0
    beq @L_801E36FC
    mr r3, r28
    mr r5, r24
    li r4, 0x0
    bl memset
@L_801E36FC:
    lwz r3, 0x44(r31)
    lwz r0, 0x40(r31)
    addc r3, r3, r29
    addze r0, r0
    stw r3, 0x44(r31)
    stw r0, 0x40(r31)
    b @L_801E37E8
@L_801E3718:
    li r27, 0x0
    lwz r25, -21424(r13)
    mr r26, r29
    mr r28, r27
@L_801E3728:
    mr r3, r25
    mr r4, r24
    mr r5, r26
    addi r6, r1, 0x20
    bl fn_801E2B74
    lwz r0, 0x20(r1)
    addc r28, r28, r3
    addze r27, r27
    cmpwi r0, 0x0
    beq @L_801E37D0
    cmpwi r0, 0x1
    bne @L_801E37A4
    cmplwi r24, 0x0
    slwi r0, r3, 1
    subf r26, r3, r26
    add r25, r25, r0
    beq @L_801E3770
    add r24, r24, r0
@L_801E3770:
    lwz r0, 0x2c(r31)
    slwi r0, r0, 3
    add r3, r31, r0
    stw r28, 0x4(r3)
    stw r27, 0x0(r3)
    lwz r3, 0x2c(r31)
    addi r0, r3, 0x1
    cmpwi r0, 0x5
    stw r0, 0x2c(r31)
    blt @L_801E3728
    li r0, 0x0
    stw r0, 0x2c(r31)
    b @L_801E3728
@L_801E37A4:
    slwi r26, r26, 1
    mr r3, r25
    mr r5, r26
    li r4, 0x0
    bl memset
    cmplwi r24, 0x0
    beq @L_801E37D0
    mr r3, r24
    mr r5, r26
    li r4, 0x0
    bl memset
@L_801E37D0:
    lwz r3, 0x44(r31)
    lwz r0, 0x40(r31)
    addc r3, r3, r29
    addze r0, r0
    stw r3, 0x44(r31)
    stw r0, 0x40(r31)
@L_801E37E8:
    slwi r24, r29, 1
    lwz r3, -21424(r13)
    mr r4, r24
    bl DCFlushRange
    lwz r3, -31520(r13)
    mr r5, r29
    li r4, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_8014E9B4
    lwz r0, 0x8c(r30)
    cmplwi r0, 0x2
    bne @L_801E3840
    lwz r3, -21420(r13)
    mr r4, r24
    bl DCFlushRange
    lwz r3, -31516(r13)
    mr r5, r29
    li r4, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_8014E9B4
@L_801E3840:
    li r3, 0x1
@L_801E3844:
    lmw r24, 0x30(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr

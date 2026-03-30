    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r23, 0xc(r1)
    mr. r31, r3
    lwz r30, 0x38(r1)
    mr r23, r4
    mr r24, r5
    mr r25, r6
    mr r26, r7
    mr r27, r8
    mr r28, r9
    mr r29, r10
    bne @L_801B6534
    addi r3, r2, -22576
    li r4, 0x2d4
    addi r5, r2, -22544
    bl fn_80196E10
@L_801B6534:
    cmplwi r31, 0x0
    bne @L_801B6544
    li r0, 0x0
    b @L_801B6570
@L_801B6544:
    addis r0, r31, 0x1
    cmplwi r0, 0xffff
    bne @L_801B6558
    li r0, 0x2
    b @L_801B6570
@L_801B6558:
    addis r0, r31, 0x1
    cmplwi r0, 0xfffe
    bne @L_801B656C
    li r0, 0x3
    b @L_801B6570
@L_801B656C:
    lwz r0, 0x0(r31)
@L_801B6570:
    cmpwi r0, 0x1
    beq @L_801B658C
    lis r4, -32729
    addi r3, r2, -22576
    addi r5, r4, 21468
    li r4, 0x2d5
    bl fn_80196E10
@L_801B658C:
    mr r4, r23
    mr r3, r31
    mr r5, r24
    li r6, 0x0
    bl fn_801B65F0
    mr r3, r31
    mr r4, r25
    mr r5, r26
    li r6, 0x1
    bl fn_801B65F0
    mr r3, r31
    mr r4, r27
    mr r5, r28
    li r6, 0x2
    bl fn_801B65F0
    mr r3, r31
    mr r4, r29
    mr r5, r30
    li r6, 0x3
    bl fn_801B65F0
    lmw r23, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

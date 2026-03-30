    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr. r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    mr r31, r7
    bne @L_801B6DF8
    addi r3, r2, -22576
    li r4, 0x1bb
    addi r5, r2, -22544
    bl fn_80196E10
@L_801B6DF8:
    cmplwi r27, 0x0
    bne @L_801B6E08
    li r0, 0x0
    b @L_801B6E34
@L_801B6E08:
    addis r0, r27, 0x1
    cmplwi r0, 0xffff
    bne @L_801B6E1C
    li r0, 0x2
    b @L_801B6E34
@L_801B6E1C:
    addis r0, r27, 0x1
    cmplwi r0, 0xfffe
    bne @L_801B6E30
    li r0, 0x3
    b @L_801B6E34
@L_801B6E30:
    lwz r0, 0x0(r27)
@L_801B6E34:
    cmpwi r0, 0x1
    beq @L_801B6E50
    lis r4, -32729
    addi r3, r2, -22576
    addi r5, r4, 21468
    li r4, 0x1bc
    bl fn_80196E10
@L_801B6E50:
    stw r28, 0x24(r27)
    stw r29, 0x28(r27)
    stw r30, 0x2c(r27)
    stw r31, 0x30(r27)
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

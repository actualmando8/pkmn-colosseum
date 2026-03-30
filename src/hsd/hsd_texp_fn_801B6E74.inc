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
    bne @L_801B6EAC
    addi r3, r2, -22576
    li r4, 0x1a3
    addi r5, r2, -22544
    bl fn_80196E10
@L_801B6EAC:
    cmplwi r27, 0x0
    bne @L_801B6EBC
    li r0, 0x0
    b @L_801B6EE8
@L_801B6EBC:
    addis r0, r27, 0x1
    cmplwi r0, 0xffff
    bne @L_801B6ED0
    li r0, 0x2
    b @L_801B6EE8
@L_801B6ED0:
    addis r0, r27, 0x1
    cmplwi r0, 0xfffe
    bne @L_801B6EE4
    li r0, 0x3
    b @L_801B6EE8
@L_801B6EE4:
    lwz r0, 0x0(r27)
@L_801B6EE8:
    cmpwi r0, 0x1
    beq @L_801B6F04
    lis r4, -32729
    addi r3, r2, -22576
    addi r5, r4, 21468
    li r4, 0x1a4
    bl fn_80196E10
@L_801B6F04:
    rlwinm r3, r31, 0, 24, 31
    stb r28, 0xd(r27)
    neg r0, r3
    cmpwi r28, 0x0
    or r0, r0, r3
    rlwinm r0, r0, 1, 31, 31
    stb r0, 0xe(r27)
    beq @L_801B6F2C
    cmpwi r28, 0x1
    bne @L_801B6F38
@L_801B6F2C:
    stb r29, 0xf(r27)
    stb r30, 0x10(r27)
    b @L_801B6F48
@L_801B6F38:
    li r3, 0x0
    li r0, 0x0
    stb r3, 0xf(r27)
    stb r0, 0x10(r27)
@L_801B6F48:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

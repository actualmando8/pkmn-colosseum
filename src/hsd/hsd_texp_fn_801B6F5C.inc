    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr. r31, r6
    mr r28, r3
    mr r29, r4
    mr r30, r5
    bne @L_801B6F94
    lis r4, -32729
    addi r3, r2, -22576
    addi r5, r4, 21416
    li r4, 0x173
    bl fn_80196E10
@L_801B6F94:
    lwz r27, 0x0(r31)
    b @L_801B6FEC
@L_801B6F9C:
    lwz r0, 0x0(r27)
    cmpwi r0, 0x4
    bne @L_801B6FE8
    lwz r0, 0x8(r27)
    cmplw r0, r28
    bne @L_801B6FE8
    lbz r0, 0xc(r27)
    cmpw r0, r29
    bne @L_801B6FE8
    lbz r0, 0xd(r27)
    cmpw r0, r30
    beq @L_801B6FE0
    lis r4, -32729
    addi r3, r2, -22576
    addi r5, r4, 21968
    li r4, 0x17a
    bl fn_80196E10
@L_801B6FE0:
    mr r3, r27
    b @L_801B7068
@L_801B6FE8:
    lwz r27, 0x4(r27)
@L_801B6FEC:
    cmplwi r27, 0x0
    bne @L_801B6F9C
    cmpwi r29, 0x7
    bne @L_801B7004
    li r3, 0x0
    b @L_801B7068
@L_801B7004:
    li r3, 0x14
    bl fn_80193B10
    mr. r27, r3
    bne @L_801B7024
    addi r3, r2, -22576
    li r4, 0x47
    addi r5, r2, -22544
    bl fn_80196E10
@L_801B7024:
    li r0, 0x4
    li r6, 0x0
    stw r0, 0x0(r27)
    li r5, 0xff
    li r4, 0xff
    li r0, 0x0
    lwz r7, 0x0(r31)
    mr r3, r27
    stw r7, 0x4(r27)
    stw r27, 0x0(r31)
    stb r6, 0x10(r27)
    stw r28, 0x8(r27)
    stb r29, 0xc(r27)
    stb r30, 0xd(r27)
    stb r5, 0xe(r27)
    stb r4, 0xf(r27)
    stb r0, 0x11(r27)
@L_801B7068:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

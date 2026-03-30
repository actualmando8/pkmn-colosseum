    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r31, r3
    mr r27, r4
    mr r28, r5
    mr r30, r6
    mr r29, r7
    bl fn_8019F024
    rlwinm. r0, r29, 0, 5, 5
    rlwinm r30, r30, 1, 0, 30
    bne @L_801979E0
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 15, 15
    beq @L_801979E0
    mr r3, r28
    bl fn_801A5DCC
@L_801979E0:
    li r3, 0x0
    li r4, 0x0
    bl fn_801AB63C
    lwz r31, 0x18(r31)
    b @L_80197A38
@L_801979F4:
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 31, 31
    bne @L_80197A34
    lwz r0, 0x14(r31)
    and. r0, r0, r30
    beq @L_80197A34
    mr r3, r31
    bl fn_80199704
    lwz r6, 0x0(r31)
    mr r3, r31
    mr r4, r27
    mr r5, r28
    lwz r12, 0x3c(r6)
    mr r6, r29
    mtctr r12
    bctrl
@L_80197A34:
    lwz r31, 0x4(r31)
@L_80197A38:
    cmplwi r31, 0x0
    bne @L_801979F4
    li r3, 0x0
    bl fn_80199704
    li r3, 0x0
    bl fn_8019F024
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

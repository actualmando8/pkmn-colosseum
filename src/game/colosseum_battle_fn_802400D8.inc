    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r28, r3
    mr r29, r4
    mr r30, r5
    mr r31, r6
    bl fn_8025CB3C
    mr r5, r3
    rlwinm r0, r30, 0, 16, 31
    rlwinm r3, r3, 0, 16, 31
    cmplw r3, r0
    beq @L_8024012C
    cmplwi r3, 0x0
    beq @L_8024012C
    mr r3, r28
    mr r4, r29
    mr r6, r31
    bl fn_8023CA9C
    b @L_80240130
@L_8024012C:
    li r3, 0x0
@L_80240130:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

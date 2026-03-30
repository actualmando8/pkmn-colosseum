    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    mr r31, r7
    rlwinm r3, r30, 0, 16, 31
    cmplwi r3, 0x0
    beq @L_8010414C
    bl fn_8005D858
    mr r6, r3
    mr r3, r27
    lha r5, 0xc(r6)
    mr r4, r28
    lha r6, 0xe(r6)
    mr r8, r29
    mr r9, r30
    mr r10, r31
    li r7, -0x1
    bl fn_80104160
@L_8010414C:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

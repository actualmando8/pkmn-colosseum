    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    neg r6, r3
    stw r0, 0x24(r1)
    or r7, r6, r3
    neg r6, r5
    stw r31, 0x1c(r1)
    rlwinm r31, r7, 1, 31, 31
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r4
    lbz r0, -21736(r13)
    rlwinm r3, r0, 0, 24, 31
    rlwinm r0, r7, 1, 31, 31
    cmplw r3, r0
    or r0, r6, r5
    rlwinm r30, r0, 1, 31, 31
    bne @L_801B2840
    lwz r0, -21740(r13)
    cmpw r0, r29
    bne @L_801B2840
    lbz r3, -21744(r13)
    rlwinm r0, r30, 0, 24, 31
    cmplw r3, r0
    beq @L_801B285C
@L_801B2840:
    mr r3, r31
    mr r4, r29
    mr r5, r30
    bl fn_800BCE88
    stb r31, -21736(r13)
    stw r29, -21740(r13)
    stb r30, -21744(r13)
@L_801B285C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

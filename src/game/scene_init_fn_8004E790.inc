    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    lwz r26, 0x60(r3)
    bl fn_80105624
    lwz r4, 0x8(r26)
    mr r30, r3
    lwz r28, 0x4(r26)
    lwz r25, 0x0(r4)
    mr r3, r28
    bl fn_801D16F0
    bl fn_8017B2CC
    cmpwi r3, 0x1
    bne @L_8004E7D4
    li r0, 0x0
    b @L_8004E7D8
    @L_8004E7D4:
    li r0, 0x1
    @L_8004E7D8:
    cmpwi r0, 0x0
    bne @L_8004E7E8
    li r28, -0x1
    b @L_8004E830
    @L_8004E7E8:
    mr r3, r28
    bl fn_801D16F0
    mr r27, r3
    bl fn_8017B448
    li r28, 0x0
    mr r31, r3
    mr r29, r28
    b @L_8004E828
    @L_8004E808:
    mr r3, r27
    mr r4, r29
    bl fn_8017B4BC
    bl fn_8017B5A4
    cmplwi r3, 0x9
    bne @L_8004E824
    addi r28, r28, 0x1
    @L_8004E824:
    addi r29, r29, 0x1
    @L_8004E828:
    cmpw r29, r31
    blt @L_8004E808
    @L_8004E830:
    cmpwi r28, 0x2
    bge @L_8004E840
    li r3, 0x0
    b @L_8004E888
    @L_8004E840:
    lhz r3, 0x6(r30)
    rlwinm r0, r3, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_8004E860
    addi r25, r25, 0x1
    cmpw r25, r28
    blt @L_8004E860
    li r25, 0x0
    @L_8004E860:
    rlwinm r0, r3, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_8004E87C
    subi r25, r25, 0x1
    cmpwi r25, 0x0
    bge @L_8004E87C
    subi r25, r28, 0x1
    @L_8004E87C:
    lwz r4, 0x8(r26)
    li r3, 0x0
    stw r25, 0x0(r4)
    @L_8004E888:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r25, r3
    li r28, 0x0
    lhz r26, 0x18(r3)
    lbz r3, 0x15(r3)
    bl fn_80168C64
    mr r27, r3
    b @L_801695E0
@L_8016954C:
    lhz r0, 0x1e(r27)
    lwz r29, 0x0(r27)
    cmplw r0, r26
    bne @L_801695D8
    lwz r4, 0x8c(r27)
    cmplwi r4, 0x0
    beq @L_801695D8
    cmplw r4, r25
    bne @L_801695D8
    cmplwi r4, 0x0
    beq @L_80169584
    lwz r3, 0x4c(r4)
    subi r0, r3, 0x1
    stw r0, 0x4c(r4)
@L_80169584:
    lwz r0, 0x90(r27)
    cmplwi r0, 0x0
    beq @L_80169598
    mr r3, r27
    bl fn_8016A6FC
@L_80169598:
    lwz r4, 0x4(r27)
    rlwinm. r0, r4, 0, 16, 16
    beq @L_801695C8
    lis r3, -32699
    rlwinm r30, r4, 22, 27, 29
    addi r31, r3, 11720
    lwzx r3, r31, r30
    cmplwi r3, 0x0
    beq @L_801695C8
    bl fn_801A05EC
    li r0, 0x0
    stwx r0, r31, r30
@L_801695C8:
    mr r3, r27
    mr r4, r28
    bl fn_80168CD0
    b @L_801695DC
@L_801695D8:
    mr r28, r27
@L_801695DC:
    mr r27, r29
@L_801695E0:
    cmplwi r27, 0x0
    bne @L_8016954C
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

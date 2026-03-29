    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    lwz r5, 0x60(r3)
    lbz r0, 0x1c(r5)
    cmplwi r0, 0x0
    beq @L_8002AF20
    cmplwi r0, 0x1
    bne @L_8002B024
    @L_8002AF20:
    lbz r4, 0x95(r3)
    lbz r0, 0x94(r3)
    extsb r3, r4
    extsb r0, r0
    add r4, r3, r0
    cmpwi r4, 0x0
    blt @L_8002AF48
    lwz r0, 0x8(r5)
    cmpw r4, r0
    blt @L_8002AF50
    @L_8002AF48:
    li r30, 0x0
    b @L_8002AF5C
    @L_8002AF50:
    lwz r3, 0x4(r5)
    slwi r0, r4, 1
    lhzx r30, r3, r0
    @L_8002AF5C:
    clrlwi r0, r30, 16
    cmplwi r0, 0x0
    beq @L_8002B024
    li r29, 0x0
    bne @L_8002AF74
    b @L_8002AFF8
    @L_8002AF74:
    mr r3, r30
    bl fn_801440A0
    bl fn_80144014
    mr r4, r3
    addi r5, r1, 0x8
    li r3, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    li r28, 0x0
    mr r27, r3
    clrlwi r31, r30, 16
    b @L_8002AFEC
    @L_8002AFAC:
    mr r3, r27
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi r0, r3, 16
    cmplw r0, r31
    bne @L_8002AFE4
    mr r3, r27
    li r4, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_80142CF4
    add r29, r29, r3
    @L_8002AFE4:
    addi r28, r28, 0x1
    addi r27, r27, 0x4
    @L_8002AFEC:
    lhz r0, 0x8(r1)
    cmpw r28, r0
    blt @L_8002AFAC
    @L_8002AFF8:
    clrlwi r4, r30, 16
    li r3, 0x2d
    bl fn_80132A38
    mr r4, r29
    li r3, 0x34
    bl fn_80132A38
    li r3, 0x0
    li r4, 0x0
    li r5, -0x1
    li r6, 0x2b2f
    bl fn_800FB680
    @L_8002B024:
    li r3, 0x0
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

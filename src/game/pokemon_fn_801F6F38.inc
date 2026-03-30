    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r25, r3
    mr r26, r5
    mr r27, r6
    clrlwi r31, r4, 16
    li r28, 0x0
    li r29, 0x0
    b @L_801F6FB0
    @L_801F6F64:
    cmplwi r25, 0x0
    bne @L_801F6F74
    li r30, 0x0
    b @L_801F6F84
    @L_801F6F74:
    mr r3, r25
    mr r4, r29
    bl fn_801F78D4
    mr r30, r3
    @L_801F6F84:
    mr r3, r30
    bl fn_801FA634
    clrlwi. r0, r3, 24
    beq @L_801F6FAC
    mr r3, r30
    mr r4, r26
    mr r5, r27
    bl fn_801F81F8
    add r0, r28, r3
    clrlwi r28, r0, 16
    @L_801F6FAC:
    addi r29, r29, 0x1
    @L_801F6FB0:
    clrlwi r0, r29, 16
    cmplw r0, r31
    blt @L_801F6F64
    mr r3, r28
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

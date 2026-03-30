    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r22, 0x18(r1)
    mr r31, r4
    mr r30, r3
    stw r5, 0x8(r1)
    lwz r0, 0xc(r4)
    clrrwi. r0, r0, 31
    beq @L_8017B694
    lwz r29, 0xfc(r30)
    li r3, 0x5
    li r5, 0x0
    li r0, 0x65
    stw r3, 0x0(r29)
    li r4, 0x20
    stw r0, 0x48(r30)
    lwz r3, 0x14(r31)
    stw r5, 0xc(r1)
    addi r0, r3, 0x1f
    clrrwi r24, r0, 5
    mr r22, r24
    addi r0, r22, 0x1f
    clrrwi r23, r0, 5
    mr r3, r23
    bl fn_800E2B00
    clrlwi. r0, r3, 16
    mr r26, r3
    beq @L_8017B644
    mr r3, r26
    bl fn_800E27B0
    mr r27, r3
    b @L_8017B648
    @L_8017B644:
    li r27, 0x0
    @L_8017B648:
    stw r27, 0x4(r29)
    lwz r3, 0xf8(r30)
    lwz r4, 0x20(r31)
    lwz r5, 0x0(r31)
    bl fn_8017F794
    mr r28, r3
    lwz r3, 0xf8(r30)
    lwz r4, 0x20(r31)
    lwz r5, 0x0(r31)
    bl fn_8017F728
    mr r25, r3
    lis r4, fn_8017A814@ha
    addi r6, r4, fn_8017A814@l
    lwz r3, 0x4(r29)
    mr r4, r28
    mr r5, r25
    mr r7, r30
    bl fn_80180584
    b @L_8017B6A4
    @L_8017B694:
    lwz r5, 0x8(r1)
    mr r3, r30
    mr r4, r31
    bl fn_8017BD34
    @L_8017B6A4:
    lmw r22, 0x18(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr

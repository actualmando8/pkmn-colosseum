    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    li r0, 0x0
    stmw r17, 0x24(r1)
    mr r25, r3
    mr r26, r4
    mr r23, r5
    stw r0, 0xc(r1)
    li r28, 0x0
    li r29, 0x0
    bl fn_800F9318
    mr r30, r3
    lwz r0, 0x1c(r30)
    mr r31, r30
    cmplwi r0, 0x3
    blt @L_8017F64C
    lwz r0, 0x20(r31)
    cmplwi r0, 0x0
    beq @L_8017F58C
    addi r0, r23, 0x1f
    lwz r3, 0x20(r31)
    clrrwi r27, r0, 5
    li r4, 0x20
    addi r0, r27, 0x1f
    addi r5, r3, 0x1f
    clrrwi r17, r0, 5
    mr r3, r17
    clrrwi r18, r5, 5
    bl fn_800E2B00
    clrlwi. r0, r3, 16
    mr r21, r3
    beq @L_8017F518
    mr r3, r21
    bl fn_800E27B0
    mr r22, r3
    b @L_8017F51C
    @L_8017F518:
    li r22, 0x0
    @L_8017F51C:
    mr r29, r22
    mr r4, r30
    mr r3, r29
    mr r5, r23
    bl memcpy
    stw r29, 0x10(r1)
    mr r3, r25
    mr r4, r26
    bl fn_800F9210
    lis r3, fn_8017F6B4@ha
    mr r5, r25
    addi r7, r3, fn_8017F6B4@l
    mr r6, r26
    add r3, r27, r18
    li r4, 0x20
    bl fn_800F9418
    mr r4, r29
    mr r30, r3
    mr r5, r27
    bl memcpy
    mr r31, r30
    add r28, r31, r27
    addi r0, r28, 0x1f
    mr r3, r31
    clrrwi r28, r0, 5
    mr r4, r28
    bl fn_8009ED4C
    b @L_8017F658
    @L_8017F58C:
    addi r0, r23, 0x1f
    li r4, 0x20
    clrrwi r27, r0, 5
    addi r0, r27, 0x1f
    clrrwi r0, r0, 5
    stw r0, 0x8(r1)
    lwz r3, 0x8(r1)
    bl fn_800E2B00
    clrlwi. r0, r3, 16
    mr r19, r3
    beq @L_8017F5C8
    mr r3, r19
    bl fn_800E27B0
    mr r20, r3
    b @L_8017F5CC
    @L_8017F5C8:
    li r20, 0x0
    @L_8017F5CC:
    mr r29, r20
    mr r4, r30
    mr r3, r29
    mr r5, r23
    bl memcpy
    mr r3, r25
    mr r4, r26
    bl fn_800F9210
    lis r4, fn_8017F6B4@ha
    mr r3, r27
    addi r7, r4, fn_8017F6B4@l
    mr r5, r25
    mr r6, r26
    li r4, 0x20
    bl fn_800F9418
    mr r30, r3
    mr r3, r29
    mr r4, r27
    bl DCFlushRange
    mr r3, r30
    mr r4, r29
    mr r5, r27
    bl memcpy
    lwz r0, 0x48(r30)
    mr r31, r30
    mr r3, r31
    add r28, r31, r0
    addi r0, r28, 0x1f
    clrrwi r28, r0, 5
    mr r4, r28
    bl fn_8009ED4C
    b @L_8017F658
    @L_8017F64C:
    mr r3, r31
    mr r4, r28
    bl fn_8009ED4C
    @L_8017F658:
    lwz r0, 0x34(r31)
    cmplwi r0, 0x0
    beq @L_8017F670
    lwz r12, 0x34(r31)
    mtctr r12
    bctrl
    @L_8017F670:
    cmplwi r29, 0x0
    beq @L_8017F69C
    mr r3, r29
    bl fn_800E202C
    clrlwi. r0, r3, 16
    mr r24, r3
    beq @L_8017F69C
    mr r3, r24
    bl fn_800E24B0
    mr r3, r24
    bl fn_800E209C
    @L_8017F69C:
    mr r3, r30
    lmw r17, 0x24(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr

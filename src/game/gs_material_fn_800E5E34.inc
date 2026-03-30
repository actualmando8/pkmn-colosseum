    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r20, 0x10(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    mr r31, r7
    lhz r0, 0x152(r3)
    cmplwi r0, 0x0
    bne @L_800E5F40
    li r23, 0x0
    bl fn_800EE0E8
    mr r25, r3
    li r26, 0x0
    b @L_800E5E9C
    @L_800E5E78:
    mr r3, r27
    mr r4, r26
    bl fn_800EE150
    mr r24, r3
    bl fn_800EE758
    add r23, r23, r3
    mr r3, r24
    bl fn_800EE828
    addi r26, r26, 0x1
    @L_800E5E9C:
    cmpw r26, r25
    blt @L_800E5E78
    cmpwi r23, 0x0
    bne @L_800E5EB4
    li r23, 0x0
    b @L_800E5F38
    @L_800E5EB4:
    slwi r3, r23, 2
    bl fn_800E3534
    clrlwi. r0, r3, 16
    mr r26, r3
    beq @L_800E5F38
    bl fn_800E27B0
    stw r3, 0x14c(r27)
    mr r22, r3
    li r21, 0x0
    sth r26, 0x154(r27)
    sth r23, 0x150(r27)
    b @L_800E5F30
    @L_800E5EE4:
    mr r3, r27
    mr r4, r21
    bl fn_800EE150
    mr r20, r3
    bl fn_800EE758
    mr r26, r3
    li r24, 0x0
    b @L_800E5F1C
    @L_800E5F04:
    mr r3, r20
    mr r4, r24
    bl fn_800EE6B4
    stw r3, 0x0(r22)
    addi r24, r24, 0x1
    addi r22, r22, 0x4
    @L_800E5F1C:
    cmpw r24, r26
    blt @L_800E5F04
    mr r3, r20
    bl fn_800EE828
    addi r21, r21, 0x1
    @L_800E5F30:
    cmpw r21, r25
    blt @L_800E5EE4
    @L_800E5F38:
    mr r21, r23
    b @L_800E5F44
    @L_800E5F40:
    lhz r21, 0x150(r27)
    @L_800E5F44:
    lhz r3, 0x152(r27)
    li r20, 0x0
    addi r0, r3, 0x1
    sth r0, 0x152(r27)
    lwz r22, 0x14c(r27)
    b @L_800E5F90
    @L_800E5F5C:
    lwz r3, 0x0(r22)
    cmplwi r3, 0x0
    beq @L_800E5F88
    mr r4, r28
    mr r5, r29
    mr r6, r30
    mr r7, r31
    bl fn_800DF208
    lwz r3, 0x0(r22)
    li r4, 0x2
    bl fn_800DF384
    @L_800E5F88:
    addi r20, r20, 0x1
    addi r22, r22, 0x4
    @L_800E5F90:
    cmpw r20, r21
    blt @L_800E5F5C
    lmw r20, 0x10(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr

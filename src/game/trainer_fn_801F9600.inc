    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r26, r4
    mr r27, r5
    mr r28, r6
    mr r25, r3
    li r29, -0x1
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r31, r3, 16
    li r3, 0x0
    mr r4, r31
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r30, r3, 16
    bl fn_80008174
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F9680
    mr r3, r25
    mr r5, r26
    mr r6, r27
    li r4, 0x0
    li r7, 0x0
    bl fn_80263DE4
    mr r29, r3
    b @L_801F9778
    @L_801F9680:
    cmplwi r30, 0x0
    beq @L_801F96A4
    mr r3, r25
    mr r4, r26
    mr r5, r27
    mr r6, r28
    bl fn_8024E690
    mr r29, r3
    b @L_801F9778
    @L_801F96A4:
    mr r4, r31
    li r3, 0x0
    li r5, 0x4
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_801F96CC
    li r0, 0x0
    b @L_801F96E8
    @L_801F96CC:
    cmplwi r0, 0x2
    beq @L_801F96DC
    cmplwi r0, 0x3
    bne @L_801F96E4
    @L_801F96DC:
    li r0, 0x1
    b @L_801F96E8
    @L_801F96E4:
    li r0, 0x2
    @L_801F96E8:
    clrlwi. r0, r0, 24
    bne @L_801F9710
    mr r3, r25
    mr r5, r26
    mr r6, r27
    li r4, 0x0
    li r7, 0x0
    bl fn_80263DE4
    mr r29, r3
    b @L_801F9778
    @L_801F9710:
    mr r4, r31
    li r3, 0x0
    li r5, 0x4
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_801F9738
    li r0, 0x0
    b @L_801F9754
    @L_801F9738:
    cmplwi r0, 0x2
    beq @L_801F9748
    cmplwi r0, 0x3
    bne @L_801F9750
    @L_801F9748:
    li r0, 0x1
    b @L_801F9754
    @L_801F9750:
    li r0, 0x2
    @L_801F9754:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_801F9778
    mr r3, r25
    mr r4, r26
    mr r5, r27
    mr r6, r28
    bl fn_80262D3C
    mr r29, r3
    @L_801F9778:
    mr r3, r29
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

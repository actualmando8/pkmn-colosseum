    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r21, 0x14(r1)
    mr r23, r4
    mr r24, r5
    mr r21, r6
    mr r22, r3
    li r25, 0x1
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r28, r3, 16
    mr r3, r22
    li r4, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r27, r3, 16
    mr r3, r22
    li r4, 0x0
    li r5, 0x18
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r0, r21, 24
    clrlwi r26, r3, 16
    cmplwi r0, 0x1
    bne @L_801F3890
    li r21, 0x0
    b @L_801F3880
    @L_801F382C:
    mr r3, r22
    mr r6, r21
    li r4, 0x0
    li r5, 0x59
    bl fn_801F54A4
    mr. r26, r3
    beq @L_801F387C
    bl fn_80206780
    clrlwi. r0, r3, 24
    beq @L_801F387C
    mr r12, r23
    mr r3, r26
    mr r4, r28
    mr r5, r24
    mtctr r12
    bctrl
    clrlwi. r0, r3, 24
    bne @L_801F387C
    li r25, 0x0
    b @L_801F396C
    @L_801F387C:
    addi r21, r21, 0x1
    @L_801F3880:
    clrlwi r0, r21, 16
    cmplwi r0, 0x8
    blt @L_801F382C
    b @L_801F396C
    @L_801F3890:
    li r30, 0x0
    b @L_801F3960
    @L_801F3898:
    li r31, 0x0
    b @L_801F3950
    @L_801F38A0:
    li r29, 0x0
    b @L_801F3940
    @L_801F38A8:
    mr r3, r22
    mr r6, r29
    li r4, 0x0
    li r5, 0x35
    bl fn_801F54A4
    mr r21, r3
    bl fn_801F7404
    clrlwi. r0, r3, 24
    bne @L_801F38D0
    li r21, 0x0
    @L_801F38D0:
    cmplwi r21, 0x0
    bne @L_801F38E0
    li r3, 0x0
    b @L_801F3910
    @L_801F38E0:
    mr r3, r21
    mr r4, r31
    bl fn_801F7258
    cmplwi r3, 0x0
    bne @L_801F38FC
    li r3, 0x0
    b @L_801F3910
    @L_801F38FC:
    mr r4, r30
    bl fn_801F981C
    cmplwi r3, 0x0
    bne @L_801F3910
    li r3, 0x0
    @L_801F3910:
    cmplwi r3, 0x0
    beq @L_801F393C
    mr r12, r23
    mr r4, r28
    mr r5, r24
    mtctr r12
    bctrl
    clrlwi. r0, r3, 24
    bne @L_801F393C
    li r25, 0x0
    b @L_801F396C
    @L_801F393C:
    addi r29, r29, 0x1
    @L_801F3940:
    clrlwi r0, r29, 16
    cmplwi r0, 0x2
    blt @L_801F38A8
    addi r31, r31, 0x1
    @L_801F3950:
    clrlwi r0, r31, 16
    cmplw r0, r27
    blt @L_801F38A0
    addi r30, r30, 0x1
    @L_801F3960:
    clrlwi r0, r30, 16
    cmplw r0, r26
    blt @L_801F3898
    @L_801F396C:
    mr r3, r25
    lmw r21, 0x14(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr

    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    li r7, 0x1
    stw r0, 0x94(r1)
    stmw r22, 0x68(r1)
    mr r22, r3
    mr r23, r5
    mr r24, r6
    addi r5, r1, 0x8
    mr r4, r22
    li r28, 0x0
    li r3, 0x0
    li r6, 0x1
    bl fn_801F1A6C
    addi r29, r1, 0x8
    clrlwi r31, r3, 16
    li r27, 0x0
    b @L_80251774
@L_8025170C:
    clrlslwi r30, r27, 16, 2
    mr r3, r22
    lwzx r4, r29, r30
    bl fn_80238980
    lwzx r4, r29, r30
    mr r26, r3
    mr r3, r22
    bl fn_8023892C
    mr r30, r3
    mr r3, r22
    mr r4, r24
    bl fn_802377E8
    mr r0, r3
    mr r3, r22
    mr r25, r0
    mr r4, r24
    bl fn_80237774
    mr r0, r3
    mr r3, r23
    mr r4, r26
    mr r6, r25
    clrlwi r5, r30, 24
    clrlwi r7, r0, 24
    bl fn_80216CF8
    add r28, r28, r3
    addi r27, r27, 0x1
@L_80251774:
    clrlwi r0, r27, 16
    cmplw r0, r31
    blt @L_8025170C
    mr r3, r28
    lmw r22, 0x68(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr

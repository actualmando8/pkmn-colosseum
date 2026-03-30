    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r22, 0x8(r1)
    mr r29, r3
    mr r30, r5
    clrlwi r28, r4, 16
    li r31, 0x0
    li r23, 0x0
    b @L_801F7150
    @L_801F70B8:
    cmplwi r29, 0x0
    bne @L_801F70C8
    li r3, 0x0
    b @L_801F70D4
    @L_801F70C8:
    mr r3, r29
    mr r4, r23
    bl fn_801F78D4
    @L_801F70D4:
    mr r26, r3
    bl fn_801FA634
    clrlwi. r0, r3, 24
    beq @L_801F714C
    clrlwi r27, r30, 16
    li r22, 0x0
    b @L_801F7140
    @L_801F70F0:
    mr r3, r26
    mr r4, r22
    bl fn_801F986C
    mr. r25, r3
    beq @L_801F713C
    bl fn_80205BE8
    mr. r24, r3
    beq @L_801F713C
    mr r3, r25
    bl fn_80206608
    clrlwi. r0, r3, 24
    beq @L_801F713C
    mr r3, r24
    li r4, 0x0
    li r5, 0x87
    li r6, 0x0
    bl fn_8012640C
    clrlwi r0, r3, 16
    add r31, r31, r0
    @L_801F713C:
    addi r22, r22, 0x1
    @L_801F7140:
    clrlwi r0, r22, 16
    cmplw r0, r27
    blt @L_801F70F0
    @L_801F714C:
    addi r23, r23, 0x1
    @L_801F7150:
    clrlwi r0, r23, 16
    cmplw r0, r28
    blt @L_801F70B8
    mr r3, r31
    lmw r22, 0x8(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

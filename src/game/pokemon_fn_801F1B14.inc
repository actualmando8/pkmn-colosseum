    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r31, r5
    mr r26, r4
    mr r29, r3
    lwz r30, 0x0(r5)
    lwz r28, 0xc(r5)
    bl fn_801FA634
    clrlwi. r0, r3, 24
    bne @L_801F1B4C
    li r3, 0x1
    b @L_801F1C04
    @L_801F1B4C:
    lwz r0, 0x4(r31)
    cmpwi r0, 0x0
    bne @L_801F1B7C
    mr r3, r29
    mr r4, r30
    mr r5, r26
    bl fn_801F8424
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F1B9C
    li r3, 0x1
    b @L_801F1C04
    @L_801F1B7C:
    mr r3, r29
    mr r4, r30
    mr r5, r26
    bl fn_801F8424
    clrlwi. r0, r3, 24
    bne @L_801F1B9C
    li r3, 0x1
    b @L_801F1C04
    @L_801F1B9C:
    lwz r0, 0x10(r31)
    li r27, 0x0
    clrlwi r26, r0, 16
    b @L_801F1BF4
    @L_801F1BAC:
    mr r3, r29
    mr r4, r27
    bl fn_801F986C
    mr. r30, r3
    beq @L_801F1BF0
    lwz r0, 0x14(r31)
    cmpwi r0, 0x1
    bne @L_801F1BD8
    bl fn_80206608
    clrlwi. r0, r3, 24
    beq @L_801F1BF0
    @L_801F1BD8:
    lwz r0, 0x8(r31)
    slwi r0, r0, 2
    stwx r30, r28, r0
    lwz r3, 0x8(r31)
    addi r0, r3, 0x1
    stw r0, 0x8(r31)
    @L_801F1BF0:
    addi r27, r27, 0x1
    @L_801F1BF4:
    clrlwi r0, r27, 16
    cmplw r0, r26
    blt @L_801F1BAC
    li r3, 0x1
    @L_801F1C04:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

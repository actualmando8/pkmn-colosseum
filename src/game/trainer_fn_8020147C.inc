    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r29, r4
    mr r30, r5
    mr r31, r6
    mr r28, r3
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_802014BC
    li r3, 0x0
    b @L_80201558
    @L_802014BC:
    bne @L_802014C8
    li r3, 0x0
    b @L_802014D8
    @L_802014C8:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    @L_802014D8:
    cmplwi r3, 0x0
    bne @L_802014E8
    li r3, 0x0
    b @L_802014F4
    @L_802014E8:
    mr r4, r29
    mr r5, r30
    bl fn_80123110
    @L_802014F4:
    clrlwi r0, r31, 24
    mr r31, r3
    cmplwi r0, 0x1
    bne @L_80201554
    clrlwi. r0, r30, 24
    bne @L_80201528
    clrlwi. r7, r3, 16
    beq @L_80201528
    mr r3, r28
    li r4, 0x0
    li r5, 0xfa
    li r6, 0x0
    bl fn_801254B4
    @L_80201528:
    clrlwi r0, r30, 24
    cmplwi r0, 0x1
    bne @L_80201554
    clrlwi. r0, r29, 16
    beq @L_80201554
    mr r3, r28
    li r4, 0x0
    li r5, 0xfa
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    @L_80201554:
    mr r3, r31
    @L_80201558:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

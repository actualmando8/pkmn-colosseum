    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r28, r5
    mr r31, r3
    lwz r30, 0xc(r5)
    bl fn_802062FC
    clrlwi. r0, r3, 24
    bne @L_801F351C
    li r3, 0x1
    b @L_801F3610
    @L_801F351C:
    cmplwi r30, 0x0
    bne @L_801F352C
    li r29, 0x0
    b @L_801F356C
    @L_801F352C:
    lwz r0, 0x8(r28)
    cmpwi r0, 0x1
    bne @L_801F354C
    mr r4, r30
    li r3, 0x2
    bl fn_801F025C
    mr r29, r3
    b @L_801F356C
    @L_801F354C:
    cmpwi r0, 0x2
    bne @L_801F3568
    mr r4, r30
    li r3, 0x3
    bl fn_801F025C
    mr r29, r3
    b @L_801F356C
    @L_801F3568:
    li r29, 0x0
    @L_801F356C:
    mr r4, r31
    li r3, 0x2
    bl fn_801F025C
    lwz r0, 0x8(r28)
    cmpwi r0, 0x1
    beq @L_801F358C
    cmpwi r0, 0x2
    bne @L_801F359C
    @L_801F358C:
    cmplwi r29, 0x0
    bne @L_801F359C
    li r3, 0x1
    b @L_801F3610
    @L_801F359C:
    cmpwi r0, 0x0
    bne @L_801F35BC
    cmplwi r30, 0x0
    beq @L_801F35E4
    cmplw r30, r31
    bne @L_801F35E4
    li r3, 0x1
    b @L_801F3610
    @L_801F35BC:
    cmpwi r0, 0x1
    beq @L_801F35CC
    cmpwi r0, 0x2
    bne @L_801F35DC
    @L_801F35CC:
    cmplw r29, r3
    beq @L_801F35E4
    li r3, 0x1
    b @L_801F3610
    @L_801F35DC:
    li r3, 0x1
    b @L_801F3610
    @L_801F35E4:
    mr r3, r31
    bl fn_80207BF4
    lwz r0, 0x0(r28)
    clrlwi r3, r3, 16
    clrlwi r0, r0, 16
    cmplw r0, r3
    bne @L_801F360C
    stw r31, 0x4(r28)
    li r3, 0x0
    b @L_801F3610
    @L_801F360C:
    li r3, 0x1
    @L_801F3610:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

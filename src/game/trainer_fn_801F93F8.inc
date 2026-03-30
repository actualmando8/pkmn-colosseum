    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr. r30, r3
    li r3, 0x0
    sth r3, 0x8(r1)
    bne @L_801F941C
    b @L_801F95C8
    @L_801F941C:
    addic. r0, r1, 0x8
    bne @L_801F95A4
    b @L_801F95C8
    b @L_801F95A4
    @L_801F942C:
    mr r3, r30
    li r4, 0x0
    li r5, 0x45
    bl fn_801FB1C0
    mr r31, r3
    bl fn_80206608
    clrlwi. r0, r3, 24
    bne @L_801F9454
    li r0, 0x1
    b @L_801F9590
    @L_801F9454:
    cmplwi r30, 0x0
    bne @L_801F9464
    li r28, 0x0
    b @L_801F94DC
    @L_801F9464:
    cmplwi r31, 0x0
    bne @L_801F9474
    li r28, 0x0
    b @L_801F94DC
    @L_801F9474:
    li r29, 0x0
    b @L_801F94CC
    @L_801F947C:
    mr r3, r30
    mr r6, r29
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    mr r28, r3
    bl fn_80206780
    clrlwi. r0, r3, 24
    bne @L_801F94A4
    li r28, 0x0
    @L_801F94A4:
    cmplwi r28, 0x0
    beq @L_801F94C8
    mr r3, r31
    mr r4, r28
    bl fn_80204928
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F94C8
    b @L_801F94DC
    @L_801F94C8:
    addi r29, r29, 0x1
    @L_801F94CC:
    clrlwi r0, r29, 16
    cmplwi r0, 0x2
    blt @L_801F947C
    li r28, 0x0
    @L_801F94DC:
    cmplwi r28, 0x0
    beq @L_801F94EC
    li r0, 0x2
    b @L_801F9590
    @L_801F94EC:
    cmplwi r30, 0x0
    bne @L_801F94FC
    li r0, 0x0
    b @L_801F9578
    @L_801F94FC:
    cmplwi r31, 0x0
    bne @L_801F950C
    li r0, 0x0
    b @L_801F9578
    @L_801F950C:
    li r29, 0x0
    b @L_801F9568
    @L_801F9514:
    mr r3, r30
    mr r6, r29
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    mr r28, r3
    bl fn_80206780
    clrlwi. r0, r3, 24
    bne @L_801F953C
    li r28, 0x0
    @L_801F953C:
    cmplwi r28, 0x0
    beq @L_801F9564
    mr r3, r28
    mr r4, r31
    bl fn_80204854
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F9564
    li r0, 0x1
    b @L_801F9578
    @L_801F9564:
    addi r29, r29, 0x1
    @L_801F9568:
    clrlwi r0, r29, 16
    cmplwi r0, 0x2
    blt @L_801F9514
    li r0, 0x0
    @L_801F9578:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_801F958C
    li r0, 0x3
    b @L_801F9590
    @L_801F958C:
    li r0, 0x0
    @L_801F9590:
    clrlwi. r0, r0, 24
    beq @L_801F95B0
    lhz r3, 0x8(r1)
    addi r0, r3, 0x1
    sth r0, 0x8(r1)
    @L_801F95A4:
    lhz r6, 0x8(r1)
    cmplwi r6, 0x6
    blt @L_801F942C
    @L_801F95B0:
    lhz r0, 0x8(r1)
    cmplwi r0, 0x6
    blt @L_801F95C4
    li r3, 0x0
    b @L_801F95C8
    @L_801F95C4:
    mr r3, r31
    @L_801F95C8:
    cmplwi r3, 0x0
    beq @L_801F95E8
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    extsh r3, r3
    b @L_801F95EC
    @L_801F95E8:
    li r3, -0x1
    @L_801F95EC:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

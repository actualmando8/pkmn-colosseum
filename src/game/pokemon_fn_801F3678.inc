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
    bne @L_801F36A8
    li r3, 0x1
    b @L_801F379C
    @L_801F36A8:
    cmplwi r30, 0x0
    bne @L_801F36B8
    li r29, 0x0
    b @L_801F36F8
    @L_801F36B8:
    lwz r0, 0x8(r28)
    cmpwi r0, 0x1
    bne @L_801F36D8
    mr r4, r30
    li r3, 0x2
    bl fn_801F025C
    mr r29, r3
    b @L_801F36F8
    @L_801F36D8:
    cmpwi r0, 0x2
    bne @L_801F36F4
    mr r4, r30
    li r3, 0x3
    bl fn_801F025C
    mr r29, r3
    b @L_801F36F8
    @L_801F36F4:
    li r29, 0x0
    @L_801F36F8:
    mr r4, r31
    li r3, 0x2
    bl fn_801F025C
    lwz r0, 0x8(r28)
    cmpwi r0, 0x1
    beq @L_801F3718
    cmpwi r0, 0x2
    bne @L_801F3728
    @L_801F3718:
    cmplwi r29, 0x0
    bne @L_801F3728
    li r3, 0x1
    b @L_801F379C
    @L_801F3728:
    cmpwi r0, 0x0
    bne @L_801F3748
    cmplwi r30, 0x0
    beq @L_801F3770
    cmplw r30, r31
    bne @L_801F3770
    li r3, 0x1
    b @L_801F379C
    @L_801F3748:
    cmpwi r0, 0x1
    beq @L_801F3758
    cmpwi r0, 0x2
    bne @L_801F3768
    @L_801F3758:
    cmplw r29, r3
    beq @L_801F3770
    li r3, 0x1
    b @L_801F379C
    @L_801F3768:
    li r3, 0x1
    b @L_801F379C
    @L_801F3770:
    mr r3, r31
    bl fn_80207BF4
    lwz r0, 0x0(r28)
    clrlwi r3, r3, 16
    clrlwi r0, r0, 16
    cmplw r0, r3
    bne @L_801F3798
    lwz r3, 0x4(r28)
    addi r0, r3, 0x1
    stw r0, 0x4(r28)
    @L_801F3798:
    li r3, 0x1
    @L_801F379C:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r28, r5
    mr r27, r4
    mr r31, r3
    lwz r0, 0x10(r5)
    lwz r30, 0x0(r5)
    cmpwi r0, 0x1
    lwz r29, 0x4(r5)
    bne @L_801F1CDC
    bl fn_802062FC
    clrlwi. r0, r3, 24
    bne @L_801F1CDC
    li r3, 0x1
    b @L_801F1D48
    @L_801F1CDC:
    lwz r0, 0xc(r28)
    cmpwi r0, 0x0
    bne @L_801F1D0C
    mr r3, r30
    mr r4, r31
    mr r5, r27
    bl fn_801F8424
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F1D2C
    li r3, 0x1
    b @L_801F1D48
    @L_801F1D0C:
    mr r3, r30
    mr r4, r31
    mr r5, r27
    bl fn_801F8424
    clrlwi. r0, r3, 24
    bne @L_801F1D2C
    li r3, 0x1
    b @L_801F1D48
    @L_801F1D2C:
    lwz r0, 0x8(r28)
    li r3, 0x1
    slwi r0, r0, 2
    stwx r31, r29, r0
    lwz r4, 0x8(r28)
    addi r0, r4, 0x1
    stw r0, 0x8(r28)
    @L_801F1D48:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

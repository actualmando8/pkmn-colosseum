    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r28, r5
    mr r31, r3
    lwz r0, 0xc(r5)
    lwz r30, 0x0(r5)
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_801F26E8
    bl fn_802062FC
    clrlwi. r0, r3, 24
    bne @L_801F26E8
    li r3, 0x1
    b @L_801F27C0
    @L_801F26E8:
    cmplwi r30, 0x0
    bne @L_801F26F8
    li r29, 0x0
    b @L_801F2738
    @L_801F26F8:
    lwz r0, 0x8(r28)
    cmpwi r0, 0x1
    bne @L_801F2718
    mr r4, r30
    li r3, 0x2
    bl fn_801F025C
    mr r29, r3
    b @L_801F2738
    @L_801F2718:
    cmpwi r0, 0x2
    bne @L_801F2734
    mr r4, r30
    li r3, 0x3
    bl fn_801F025C
    mr r29, r3
    b @L_801F2738
    @L_801F2734:
    li r29, 0x0
    @L_801F2738:
    mr r4, r31
    li r3, 0x2
    bl fn_801F025C
    lwz r0, 0x8(r28)
    cmpwi r0, 0x1
    beq @L_801F2758
    cmpwi r0, 0x2
    bne @L_801F2768
    @L_801F2758:
    cmplwi r29, 0x0
    bne @L_801F2768
    li r3, 0x1
    b @L_801F27C0
    @L_801F2768:
    cmpwi r0, 0x0
    bne @L_801F2788
    cmplwi r30, 0x0
    beq @L_801F27B0
    cmplw r30, r31
    bne @L_801F27B0
    li r3, 0x1
    b @L_801F27C0
    @L_801F2788:
    cmpwi r0, 0x1
    beq @L_801F2798
    cmpwi r0, 0x2
    bne @L_801F27A8
    @L_801F2798:
    cmplw r29, r3
    beq @L_801F27B0
    li r3, 0x1
    b @L_801F27C0
    @L_801F27A8:
    li r3, 0x1
    b @L_801F27C0
    @L_801F27B0:
    lwz r4, 0x4(r28)
    li r3, 0x1
    addi r0, r4, 0x1
    stw r0, 0x4(r28)
    @L_801F27C0:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

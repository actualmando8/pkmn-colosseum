    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r6, lbl_8036CB30@ha
    stw r0, 0x24(r1)
    addi r6, r6, lbl_8036CB30@l
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    stw r28, 0x10(r1)
    mr r28, r5
    lwz r12, 0x44(r6)
    mtctr r12
    bctrl
    lwz r29, 0x20(r29)
    mr r30, r3
    cmplwi r29, 0x0
    bne @L_800DF97C
    b @L_800DFA9C
    @L_800DF97C:
    lhz r0, 0x2(r29)
    rlwinm. r0, r0, 0, 30, 30
    beq @L_800DF9C4
    mr r31, r30
    b @L_800DF9BC
    @L_800DF990:
    mr r3, r31
    bl fn_801B7C60
    cmpwi r3, 0x1
    bne @L_800DF9B8
    lwz r4, 0x10(r29)
    mr r3, r31
    lwz r5, 0x14(r29)
    lwz r6, 0x18(r29)
    lwz r7, 0x1c(r29)
    bl fn_801B6DC0
    @L_800DF9B8:
    lwz r31, 0x4(r31)
    @L_800DF9BC:
    cmplwi r31, 0x0
    bne @L_800DF990
    @L_800DF9C4:
    lhz r0, 0x2(r29)
    clrlwi. r0, r0, 31
    beq @L_800DFA98
    mr r6, r28
    addi r3, r29, 0xc
    li r4, 0x1
    li r5, 0x0
    bl fn_801B6F5C
    mr r31, r3
    mr r6, r28
    addi r3, r29, 0xf
    li r4, 0x6
    li r5, 0x0
    bl fn_801B6F5C
    mr r0, r3
    mr r3, r28
    mr r28, r0
    bl fn_801B707C
    li r4, 0x0
    mr r29, r3
    li r5, 0x0
    li r6, 0x1
    li r7, 0x1
    bl fn_801B6E74
    li r0, 0x0
    mr r3, r29
    stw r0, 0x8(r1)
    mr r7, r31
    mr r9, r30
    li r4, 0x0
    li r5, 0x0
    li r6, 0x1
    li r8, 0x1
    li r10, 0x0
    bl fn_801B64EC
    mr r3, r29
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801B6CD8
    li r0, 0x0
    mr r3, r29
    stw r0, 0x8(r1)
    mr r7, r28
    mr r9, r30
    li r4, 0x0
    li r5, 0x0
    li r6, 0x5
    li r8, 0x5
    li r10, 0x0
    bl fn_801B5F08
    mr r30, r29
    @L_800DFA98:
    mr r3, r30
    @L_800DFA9C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

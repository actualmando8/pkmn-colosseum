    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r5, lbl_80401490@ha
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r3
    li r3, 0x0
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    addi r29, r5, lbl_80401490@l
    stw r28, 0x10(r1)
    mr r28, r4
    lwz r0, 0x0(r29)
    cmplw r0, r31
    bne @L_800E918C
    li r3, 0x1
    b @L_800E91F0
    @L_800E918C:
    addi r4, r29, 0x58
    lwz r0, 0x58(r29)
    cmplw r0, r31
    bne @L_800E91A4
    li r3, 0x1
    b @L_800E91F0
    @L_800E91A4:
    lwzu r0, 0x58(r4)
    cmplw r0, r31
    bne @L_800E91B8
    li r3, 0x1
    b @L_800E91F0
    @L_800E91B8:
    lwzu r0, 0x58(r4)
    cmplw r0, r31
    bne @L_800E91CC
    li r3, 0x1
    b @L_800E91F0
    @L_800E91CC:
    lwzu r0, 0x58(r4)
    cmplw r0, r31
    bne @L_800E91E0
    li r3, 0x1
    b @L_800E91F0
    @L_800E91E0:
    lwz r0, 0x58(r4)
    cmplw r0, r31
    bne @L_800E91F0
    li r3, 0x1
    @L_800E91F0:
    clrlwi. r0, r3, 24
    beq @L_800E9268
    mr r3, r31
    bl fn_800E5188
    clrlwi. r0, r28, 24
    bne @L_800E921C
    lis r4, fn_800E9358@ha
    li r5, 0x0
    addi r4, r4, fn_800E9358@l
    bl fn_801A3918
    b @L_800E922C
    @L_800E921C:
    lis r4, fn_800E9358@ha
    li r5, 0x1
    addi r4, r4, fn_800E9358@l
    bl fn_801A3918
    @L_800E922C:
    clrlwi r30, r28, 24
    li r28, 0x0
    @L_800E9234:
    lbz r0, 0x50(r29)
    cmplwi r0, 0x0
    beq @L_800E9258
    lwz r0, 0x0(r29)
    cmplw r0, r31
    bne @L_800E9258
    lwz r3, 0x54(r29)
    mr r4, r30
    bl fn_801B0880
    @L_800E9258:
    addi r28, r28, 0x1
    addi r29, r29, 0x58
    cmplwi r28, 0x6
    blt @L_800E9234
    @L_800E9268:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

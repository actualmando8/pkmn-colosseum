    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    li r9, 0x0
    stw r0, 0x64(r1)
    stmw r27, 0x4c(r1)
    mr r30, r7
    mr r31, r8
    mr r8, r9
    addi r7, r1, 0x1c
    b @L_801F19C4
    @L_801F19B8:
    clrlslwi r0, r9, 16, 2
    addi r9, r9, 0x1
    stwx r8, r7, r0
    @L_801F19C4:
    clrlwi r0, r9, 16
    cmplwi r0, 0x8
    blt @L_801F19B8
    clrlwi r7, r5, 24
    clrlwi r0, r6, 24
    addi r6, r1, 0x1c
    li r8, 0x0
    stw r6, 0xc(r1)
    lis r5, fn_801F1C98@ha
    li r6, 0x0
    stw r4, 0x8(r1)
    addi r4, r5, fn_801F1C98@l
    addi r5, r1, 0x8
    stw r8, 0x10(r1)
    stw r7, 0x14(r1)
    stw r0, 0x18(r1)
    bl fn_801F37B0
    lwz r0, 0x10(r1)
    addi r29, r1, 0x1c
    li r27, 0x0
    clrlwi r28, r0, 16
    b @L_801F1A48
    @L_801F1A1C:
    clrlslwi r0, r27, 16, 2
    mr r4, r30
    lwzx r3, r29, r0
    mr r5, r31
    bl fn_80204DE4
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F1A44
    li r3, 0x1
    b @L_801F1A58
    @L_801F1A44:
    addi r27, r27, 0x1
    @L_801F1A48:
    clrlwi r0, r27, 16
    cmplw r0, r28
    blt @L_801F1A1C
    li r3, 0x0
    @L_801F1A58:
    lmw r27, 0x4c(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr

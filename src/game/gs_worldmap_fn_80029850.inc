    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r26, r3
    clrlwi r30, r6, 16
    clrlwi r29, r4, 16
    li r28, 0x0
    li r27, 0x0
    clrlwi r31, r5, 16
    b @L_800298BC
    @L_8002987C:
    mr r3, r26
    bl fn_80143C68
    clrlwi r0, r3, 16
    cmplw r0, r31
    bne @L_800298A8
    mr r3, r26
    bl fn_80143C50
    subf r0, r3, r30
    clrlwi r0, r0, 16
    add r28, r28, r0
    b @L_800298B4
    @L_800298A8:
    cmplwi r0, 0x0
    bne @L_800298B4
    add r28, r28, r30
    @L_800298B4:
    addi r27, r27, 0x1
    addi r26, r26, 0x4
    @L_800298BC:
    cmpw r27, r29
    blt @L_8002987C
    mr r3, r28
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

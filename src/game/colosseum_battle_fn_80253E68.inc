    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r30, r6
    mr r28, r4
    mr r27, r3
    mr r29, r5
    mr r4, r30
    bl fn_80235AA0
    mr r31, r3
    mr r3, r27
    mr r4, r30
    bl fn_80235A3C
    clrlwi. r0, r31, 24
    bne @L_80253EB8
    clrlwi. r0, r3, 24
    bne @L_80253EB8
    li r3, 0x0
    b @L_80253F14
@L_80253EB8:
    mr r3, r27
    mr r4, r28
    mr r5, r30
    mr r6, r29
    li r7, 0x90
    li r8, 0x1
    li r9, 0x1
    bl fn_8025C808
    clrlwi. r0, r3, 24
    bne @L_80253F10
    mr r3, r27
    mr r4, r28
    mr r5, r30
    mr r6, r29
    li r7, 0x90
    li r8, 0x2
    li r9, 0x1
    bl fn_8025C808
    clrlwi. r0, r3, 24
    bne @L_80253F10
    li r3, 0x0
    b @L_80253F14
@L_80253F10:
    li r3, 0x1
@L_80253F14:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

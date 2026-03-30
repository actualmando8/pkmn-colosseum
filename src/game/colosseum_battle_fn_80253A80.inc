    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    bl fn_802359D8
    mr r31, r3
    mr r3, r27
    mr r4, r28
    bl fn_80235974
    clrlwi r0, r31, 24
    cmplwi r0, 0xc
    blt @L_80253AD4
    clrlwi r0, r3, 24
    cmplwi r0, 0xc
    blt @L_80253AD4
    li r3, 0x0
    b @L_80253B30
@L_80253AD4:
    mr r3, r27
    mr r4, r28
    mr r5, r30
    mr r6, r29
    li r7, 0x10
    li r8, 0x4
    li r9, 0x41
    bl fn_8025C808
    clrlwi. r0, r3, 24
    bne @L_80253B2C
    mr r3, r27
    mr r4, r28
    mr r5, r30
    mr r6, r29
    li r7, 0x10
    li r8, 0x5
    li r9, 0x41
    bl fn_8025C808
    clrlwi. r0, r3, 24
    bne @L_80253B2C
    li r3, 0x0
    b @L_80253B30
@L_80253B2C:
    li r3, 0x1
@L_80253B30:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

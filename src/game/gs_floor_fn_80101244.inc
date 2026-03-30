    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r7, -32729
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r6
    stw r30, 0x18(r1)
    mr r30, r5
    stw r29, 0x14(r1)
    mr r29, r4
    stw r28, 0x10(r1)
    mr r28, r3
    addi r3, r7, 6596
    crxor 6, 6, 6
    bl fn_800DD970
    addi r0, r29, 0x1f
    mr r5, r30
    mr r6, r31
    li r4, 0x20
    rlwinm r3, r0, 0, 0, 26
    li r7, 0x0
    bl fn_800F9418
    cmplwi r3, 0x0
    bne @L_801012BC
    lis r3, -32729
    mr r4, r29
    addi r3, r3, 6624
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_801012C8
@L_801012BC:
    mr r4, r28
    mr r5, r29
    bl memcpy
@L_801012C8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

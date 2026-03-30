    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    addi r5, r5, 0x1f
    rlwinm r0, r4, 0, 1, 15
    rlwinm r30, r5, 0, 0, 26
    mr r5, r3
    mr r3, r30
    ori r6, r0, 0x400
    li r4, 0x20
    li r7, 0x0
    bl fn_800F9418
    mr r31, r3
    cmplwi r31, 0x0
    bne @L_801144B4
    lis r3, -32729
    mr r4, r30
    addi r3, r3, 8816
    crxor 6, 6, 6
    bl fn_800DD970
@L_801144B4:
    mr r3, r31
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

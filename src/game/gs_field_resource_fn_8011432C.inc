    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r5
    lis r5, -32751
    addi r0, r30, 0x1f
    addi r7, r5, 20628
    mr r6, r4
    mr r5, r3
    rlwinm r3, r0, 0, 0, 26
    li r4, 0x20
    bl fn_800F9418
    mr r31, r3
    cmplwi r31, 0x0
    bne @L_80114384
    lis r3, -32729
    mr r4, r30
    addi r3, r3, 8704
    crxor 6, 6, 6
    bl fn_800DD970
@L_80114384:
    mr r3, r31
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

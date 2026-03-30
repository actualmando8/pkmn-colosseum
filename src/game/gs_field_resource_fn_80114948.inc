    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    addi r5, r5, 0x1f
    rlwinm r0, r4, 0, 1, 15
    rlwinm r4, r5, 0, 0, 26
    mr r5, r3
    addi r31, r4, 0x60
    ori r6, r0, 0x400
    mr r3, r31
    li r4, 0x20
    li r7, 0x0
    bl fn_800F9418
    cmplwi r3, 0x0
    bne @L_801149A4
    lis r3, -32729
    mr r4, r31
    addi r3, r3, 9256
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_801149A8
@L_801149A4:
    addi r3, r3, 0x60
@L_801149A8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

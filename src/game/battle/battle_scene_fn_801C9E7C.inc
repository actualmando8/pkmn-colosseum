    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    mr r31, r4
    bl fn_800F92D4
    cmplwi r3, 0x0
    bne @L_801C9EBC
    lis r3, -32729
    mr r4, r30
    addi r3, r3, 22700
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_801C9EC4
@L_801C9EBC:
    rlwinm r4, r31, 0, 24, 31
    bl fn_800DCC34
@L_801C9EC4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    li r3, 0x0
    li r4, 0xa
    bl fn_80129280
    lwz r0, -31592(r13)
    rlwinm r4, r31, 0, 24, 31
    cmpw r4, r0
    bge @L_801D16B0
    stb r31, 0x443(r3)
@L_801D16B0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

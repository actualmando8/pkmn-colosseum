    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    neg r4, r3
    stw r0, 0x14(r1)
    or r4, r4, r3
    stw r31, 0xc(r1)
    rlwinm r31, r4, 1, 31, 31
    lbz r0, -21763(r13)
    rlwinm r3, r0, 0, 24, 31
    rlwinm r0, r4, 1, 31, 31
    cmplw r3, r0
    beq @L_801B27C8
    mr r3, r31
    bl fn_800BCE30
    stb r31, -21763(r13)
@L_801B27C8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

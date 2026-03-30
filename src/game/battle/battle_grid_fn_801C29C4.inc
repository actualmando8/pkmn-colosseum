    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    beq @L_801C29F4
    stfs f1, 0x4(r3)
    lwz r0, 0x0(r3)
    rlwinm r0, r0, 0, 2, 0
    oris r0, r0, 0x800
    stw r0, 0x0(r3)
    lwz r3, 0x14(r3)
    bl fn_8019B528
    @L_801C29F4:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

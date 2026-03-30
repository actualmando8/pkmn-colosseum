    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r4, 0x0(r3)
    lwz r3, 0x8(r3)
    rlwinm. r0, r4, 0, 28, 28
    beq @L_800EC2F4
    rlwinm. r0, r4, 0, 14, 14
    beq @L_800EC2D4
    lwz r3, 0x10(r3)
    @L_800EC2D4:
    stfs f1, 0xb0(r31)
    li r4, 0x634
    stfs f1, 0xb4(r31)
    lfs f1, 0xb4(r31)
    bl fn_801A32A0
    lwz r0, 0x0(r31)
    rlwinm r0, r0, 0, 17, 15
    stw r0, 0x0(r31)
    @L_800EC2F4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

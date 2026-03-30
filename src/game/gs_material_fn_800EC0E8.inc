    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r3, 0x0(r3)
    rlwinm. r0, r3, 0, 15, 15
    beq @L_800EC110
    rlwinm. r0, r3, 0, 19, 19
    bne @L_800EC120
    @L_800EC110:
    mr r3, r31
    bl fn_800ED1CC
    mr r3, r31
    bl fn_800ED4D4
    @L_800EC120:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

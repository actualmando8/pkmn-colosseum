    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r4, 0x20
    bl fn_800E2C04
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x0
    beq @L_801C7658
    bl fn_800E27B0
    b @L_801C765C
@L_801C7658:
    li r3, 0x0
@L_801C765C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

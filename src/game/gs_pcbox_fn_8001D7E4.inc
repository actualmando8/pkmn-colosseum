    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    b @L_8001D7FC
    @L_8001D7F8:
    bl fn_800F0308
    @L_8001D7FC:
    li r3, 0x1
    bl fn_800F7AF0
    mr r31, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r31
    rlwinm r0, r0, 0, 22, 23
    cmplwi r0, 0x0
    beq @L_8001D7F8
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

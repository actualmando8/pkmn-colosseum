    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    bl fn_801EF634
    rlwinm. r0, r3, 0, 16, 31
    beq @L_8020BBCC
    li r3, 0x1
    b @L_8020BBE8
@L_8020BBCC:
    mr r3, r31
    bl fn_802119D4
    li r3, 0x5
    bl fn_801F000C
    li r3, 0x0
    bl fn_801F4AC0
    li r3, 0x1
@L_8020BBE8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    bl fn_801EE468
    rlwinm r31, r3, 0, 16, 31
    li r30, 0x0
    b @L_801EF0B4
@L_801EF0A0:
    mr r3, r30
    bl fn_801EE440
    li r4, 0x1
    bl fn_801EEB34
    addi r30, r30, 0x1
@L_801EF0B4:
    rlwinm r0, r30, 0, 16, 31
    cmplw r0, r31
    blt @L_801EF0A0
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

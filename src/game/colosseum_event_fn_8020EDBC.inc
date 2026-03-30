    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    bl fn_801FBDC4
    mr r3, r31
    li r4, 0x0
    bl fn_801FBDB4
    mr r3, r31
    li r4, 0x0
    bl fn_801FBDA4
    mr r3, r31
    li r4, 0x0
    bl fn_801FBD94
    mr r3, r31
    li r4, 0x0
    bl fn_801FBD84
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    bl fn_801FD0AC
    mr r3, r31
    li r4, 0x0
    bl fn_801FD09C
    mr r3, r31
    li r4, 0x0
    bl fn_801FD08C
    mr r3, r31
    li r4, 0x0
    bl fn_801FD07C
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_801E4A6C
    li r0, 0x1
    stb r0, -21472(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

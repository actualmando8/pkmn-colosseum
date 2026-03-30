    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_801170A4
    bl fn_80116EC8
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

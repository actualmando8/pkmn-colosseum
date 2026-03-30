    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    srawi r4, r3, 31
    stw r0, 0x14(r1)
    lwz r0, -32728(r13)
    add r0, r0, r3
    andc r3, r0, r4
    bl fn_80104704
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

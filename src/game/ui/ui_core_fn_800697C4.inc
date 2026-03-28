    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lis r4, fn_800697F4@ha
    li r3, 0x0
    addi r4, r4, fn_800697F4@l
    li r5, 0x0
    bl fn_8010B01C
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

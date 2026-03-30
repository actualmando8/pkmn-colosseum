    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, fn_801F1554@ha
    li r5, 0x0
    stw r0, 0x14(r1)
    addi r4, r4, fn_801F1554@l
    li r6, 0x0
    stw r31, 0xc(r1)
    mr r31, r3
    bl fn_801F37B0
    mr r3, r31
    li r4, 0x1
    bl fn_801F3984
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

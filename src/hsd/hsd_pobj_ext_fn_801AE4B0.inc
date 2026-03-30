    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, -32698
    stw r0, 0x14(r1)
    addi r3, r3, 22196
    stw r31, 0xc(r1)
    bl fn_801AA4CC
    mr. r31, r3
    bne @L_801AE4E4
    addi r3, r2, -22864
    li r4, 0x3c5
    addi r5, r2, -22816
    bl fn_80196E10
@L_801AE4E4:
    mr r3, r31
    li r4, 0x0
    li r5, 0x1c
    bl memset
    lwz r0, 0x14(r1)
    mr r3, r31
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

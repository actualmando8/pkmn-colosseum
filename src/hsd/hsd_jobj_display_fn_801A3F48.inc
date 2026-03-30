    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, 0x8046
    stw r0, 0x14(r1)
    addi r3, r3, 21940
    stw r31, 0xc(r1)
    bl fn_801AA4CC
    mr. r31, r3
    bne @L_801A3F7C
    addi r3, r2, -23296
    li r4, 0x4c
    addi r5, r2, -23280
    bl fn_80196E10
@L_801A3F7C:
    mr r3, r31
    li r4, 0x0
    li r5, 0x8
    bl memset
    lwz r0, 0x14(r1)
    mr r3, r31
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

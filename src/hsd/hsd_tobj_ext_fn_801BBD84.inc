    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x18
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl fn_80193B10
    mr. r31, r3
    bne @L_801BBDB4
    addi r3, r2, -22512
    li r4, 0x8f7
    addi r5, r2, -22504
    bl fn_80196E10
@L_801BBDB4:
    mr r3, r31
    li r4, 0x0
    li r5, 0x18
    bl memset
    lwz r0, 0x14(r1)
    mr r3, r31
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr


    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r5, 0x8046
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r4
    stw r29, 0x14(r1)
    mr r29, r3
    addi r3, r5, 21940
    bl fn_801AA4CC
    mr. r31, r3
    bne @L_801A3EF8
    addi r3, r2, -23296
    li r4, 0x4c
    addi r5, r2, -23280
    bl fn_80196E10
@L_801A3EF8:
    mr r3, r31
    li r4, 0x0
    li r5, 0x8
    bl memset
    cmplwi r31, 0x0
    stw r30, 0x4(r31)
    bne @L_801A3F24
    addi r3, r2, -23296
    li r4, 0xca
    addi r5, r2, -23288
    bl fn_80196E10
@L_801A3F24:
    stw r29, 0x0(r31)
    mr r3, r31
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

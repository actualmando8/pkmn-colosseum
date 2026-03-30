    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r31, r3
    mr r30, r4
    bl fn_80113F48
    mr r4, r31
    bl fn_800F9318
    mr r31, r3
    bl fn_80113F48
    mr r4, r30
    bl fn_800F9318
    mr r0, r3
    mr r3, r31
    mr r4, r0
    bl fn_800E3C08
    mr r3, r31
    li r4, 0x4
    bl fn_800E3C00
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

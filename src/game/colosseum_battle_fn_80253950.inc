    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r5, 0xf
    li r6, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r3
    mr r31, r4
    li r3, 0x0
    li r4, 0x0
    bl fn_801F54A4
    clrlwi r3, r3, 16
    bl fn_80136428
    mr r0, r3
    mr r3, r30
    mr r4, r31
    clrlwi r5, r0, 24
    bl fn_80237DBC
    clrlwi r0, r3, 24
    lmw r30, 0x8(r1)
    subfic r3, r0, 0x1
    subic r0, r3, 0x1
    subfe r3, r0, r3
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

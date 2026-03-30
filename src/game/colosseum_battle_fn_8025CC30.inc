    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r3
    lwz r31, 0x0(r5)
    bl fn_802062FC
    clrlwi. r0, r3, 24
    bne @L_8025CC5C
    li r3, 0x1
    b @L_8025CC7C
@L_8025CC5C:
    mr r3, r31
    mr r4, r30
    li r5, 0x6
    bl fn_80237F74
    clrlwi r0, r3, 24
    subfic r3, r0, 0x1
    subic r0, r3, 0x1
    subfe r3, r0, r3
@L_8025CC7C:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

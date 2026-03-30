    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r29, r3
    lwz r31, 0x0(r5)
    lwz r30, 0x4(r5)
    bl fn_802062FC
    clrlwi. r0, r3, 24
    bne @L_8025CD10
    li r3, 0x1
    b @L_8025CD50
@L_8025CD10:
    mr r3, r31
    mr r4, r29
    li r5, 0xb
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025CD4C
    mr r3, r31
    mr r4, r30
    li r5, 0x2b
    bl fn_80237F74
    clrlwi. r0, r3, 24
    bne @L_8025CD4C
    li r3, 0x0
    b @L_8025CD50
@L_8025CD4C:
    li r3, 0x1
@L_8025CD50:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

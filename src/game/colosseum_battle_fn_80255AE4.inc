    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r5, 0x2d
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r3
    mr r31, r4
    bl fn_80236BFC
    clrlwi. r0, r3, 24
    bne @L_80255B14
    li r3, 0x0
    b @L_80255B38
@L_80255B14:
    lfs f1, -20568(r2)
    mr r3, r30
    mr r4, r31
    li r5, 0x0
    bl fn_802373B0
    clrlwi r0, r3, 24
    subfic r3, r0, 0x1
    subic r0, r3, 0x1
    subfe r3, r0, r3
@L_80255B38:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r5, 0x2d
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r29, r3
    mr r30, r4
    li r31, 0x1
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025152C
    mr r3, r30
    li r4, 0x2d
    bl fn_80202360
    mr r31, r3
@L_8025152C:
    li r3, 0x2d
    bl fn_80119DD0
    clrlwi r0, r3, 24
    subf r0, r31, r0
    extsh. r0, r0
    bge @L_80251548
    li r0, 0x0
@L_80251548:
    extsh r0, r0
    li r3, 0x1
    slw r0, r3, r0
    mr r3, r29
    mr r4, r30
    extsh r31, r0
    bl fn_80237664
    clrlwi r0, r3, 16
    divw r0, r0, r31
    neg r3, r0
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

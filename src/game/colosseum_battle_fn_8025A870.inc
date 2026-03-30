    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r5, 0x8
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r3
    mr r31, r4
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025A8A4
    li r3, 0x0
    b @L_8025A8E8
@L_8025A8A4:
    mr r3, r30
    mr r4, r31
    bl fn_8025CC90
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025A8C4
    li r3, 0x0
    b @L_8025A8E8
@L_8025A8C4:
    lfs f1, -20568(r2)
    mr r3, r30
    mr r4, r31
    li r5, 0x0
    bl fn_802373B0
    clrlwi r0, r3, 24
    subfic r3, r0, 0x1
    subic r0, r3, 0x1
    subfe r3, r0, r3
@L_8025A8E8:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

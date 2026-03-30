    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r5, 0x14
    li r6, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r31, r3
    mr r29, r4
    li r3, 0x0
    li r4, 0x0
    bl fn_801F54A4
    cmplwi r31, 0x0
    rlwinm r30, r3, 0, 16, 31
    bne @L_80202BC8
    li r3, 0x0
    b @L_80202C08
@L_80202BC8:
    cmplwi r29, 0x0
    bne @L_80202BD8
    li r3, 0x0
    b @L_80202C08
@L_80202BD8:
    mr r4, r31
    mr r5, r30
    li r3, 0x2
    bl fn_801F02AC
    mr r31, r3
    mr r4, r29
    mr r5, r30
    li r3, 0x2
    bl fn_801F02AC
    subf r0, r31, r3
    cntlzw r0, r0
    rlwinm r3, r0, 27, 24, 31
@L_80202C08:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

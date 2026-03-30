    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r29, r5
    mr r30, r3
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80218204
    li r3, 0x1
    b @L_8021825C
@L_80218204:
    mr r3, r30
    bl fn_80207BF4
    mr r31, r3
    mr r3, r30
    li r4, 0x1e
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80218234
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x2b
    bne @L_8021823C
@L_80218234:
    li r3, 0x1
    b @L_8021825C
@L_8021823C:
    mr r3, r30
    li r4, 0x1e
    li r5, 0x0
    bl fn_8020248C
    lwz r4, 0x0(r29)
    li r3, 0x1
    addi r0, r4, 0x1
    stw r0, 0x0(r29)
@L_8021825C:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

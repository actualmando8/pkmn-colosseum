    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r3
    mr r31, r4
    bl fn_801EF634
    rlwinm. r0, r3, 0, 16, 31
    beq @L_8020BF6C
    li r3, 0x1
    b @L_8020BF8C
@L_8020BF6C:
    mr r3, r30
    mr r4, r31
    bl fn_801F923C
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8020BF88
    li r3, 0x0
    bl fn_801F150C
@L_8020BF88:
    li r3, 0x1
@L_8020BF8C:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr. r30, r3
    mr r31, r4
    beq @L_8020E6C0
    cmplwi r31, 0x0
    beq @L_8020E6C0
    li r4, 0x0
    bl fn_801FD0AC
    mr r3, r30
    li r4, 0x0
    bl fn_801FD09C
    mr r3, r30
    li r4, 0x0
    bl fn_801FD08C
    mr r3, r30
    li r4, 0x0
    bl fn_801FD07C
    mr r3, r30
    mr r4, r31
    bl fn_801FD0AC
    mr r3, r31
    bl fn_80205B8C
    li r4, 0x0
    li r5, 0x83
    li r6, 0x0
    bl fn_8012640C
    rlwinm r4, r3, 0, 16, 31
    mr r3, r30
    bl fn_801FD08C
@L_8020E6C0:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

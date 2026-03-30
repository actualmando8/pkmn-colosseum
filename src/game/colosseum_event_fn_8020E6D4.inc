    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr. r28, r3
    beq @L_8020E744
    rlwinm r31, r4, 0, 16, 31
    li r29, 0x0
    b @L_8020E738
@L_8020E6F8:
    rlwinm r0, r29, 0, 16, 31
    li r4, 0x0
    mulli r0, r0, 0xc
    add r30, r28, r0
    mr r3, r30
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
    addi r29, r29, 0x1
@L_8020E738:
    rlwinm r0, r29, 0, 16, 31
    cmplw r0, r31
    blt @L_8020E6F8
@L_8020E744:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

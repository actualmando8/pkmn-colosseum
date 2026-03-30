    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    bl fn_801F025C
    mr r0, r3
    li r3, 0x2
    mr r31, r0
    mr r4, r31
    bl fn_801F025C
    li r4, 0x4d
    mr r30, r3
    bl fn_801F6E44
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80216630
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r4, r3, 0, 16, 31
    mr r3, r31
    bl fn_801F0134
    mr r0, r3
    mr r3, r30
    mr r5, r0
    li r4, 0x4d
    bl fn_801F6DF0
@L_80216630:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

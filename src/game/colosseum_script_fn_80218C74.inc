    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    bl fn_801F025C
    mr r31, r3
    bl fn_80205184
    mr r0, r3
    li r3, 0x12
    mr r30, r0
    li r4, 0x0
    bl fn_801F025C
    mr r0, r3
    mr r3, r30
    mr r5, r0
    mr r4, r31
    bl fn_80229934
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80218CDC
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_80218D10
@L_80218CDC:
    mr r3, r31
    li r4, 0x15
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80218D04
    mr r3, r31
    li r4, 0x15
    li r5, 0x0
    bl fn_8020248C
@L_80218D04:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
@L_80218D10:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r30, 0x18(r1)
    lwz r3, lbl_8047B610(r13)
    lbz r3, 0x1(r3)
    bl fn_801F025C
    mr r0, r3
    li r3, 0x0
    mr r31, r0
    mr r4, r31
    bl fn_801F4354
    lwz r4, lbl_8047B610(r13)
    mr r30, r3
    lbz r0, 0x6(r4)
    cmplwi r0, 0x1
    bne @L_8021F410
    mr r4, r31
    li r3, 0x0
    li r5, 0x0
    bl fn_801F2020
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021F410
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x2(r3)
    stw r0, lbl_8047B610(r13)
    b @L_8021F444
@L_8021F410:
    li r0, 0x0
    mr r3, r30
    sth r0, 0x8(r1)
    addi r4, r1, 0x8
    bl fn_801F8A18
    cmplwi r3, 0x0
    bne @L_8021F438
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x2(r3)
    b @L_8021F440
@L_8021F438:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x7
@L_8021F440:
    stw r0, lbl_8047B610(r13)
@L_8021F444:
    lmw r30, 0x18(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

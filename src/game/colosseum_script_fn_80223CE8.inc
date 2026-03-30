    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    lwz r3, lbl_8047B610(r13)
    lbz r3, 0x1(r3)
    bl fn_801F025C
    mr r31, r3
    bl fn_80207BF4
    lwz r4, lbl_8047B610(r13)
    rlwinm r3, r3, 0, 16, 31
    lbz r0, 0x2(r4)
    lwz r30, 0x3(r4)
    cmpw r3, r0
    bne @L_80223D48
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x48
    li r6, 0x0
    bl fn_801F4C14
    stw r30, lbl_8047B610(r13)
    b @L_80223D50
@L_80223D48:
    addi r0, r4, 0x7
    stw r0, lbl_8047B610(r13)
@L_80223D50:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    lwz r3, lbl_8047B610(r13)
    lbz r3, 0x1(r3)
    bl fn_801F025C
    lwz r4, lbl_8047B610(r13)
    mr r30, r3
    lwz r0, 0x2(r4)
    lwz r31, 0x6(r4)
    rlwinm r4, r0, 0, 16, 31
    cmplwi r4, 0x1
    bne @L_80223DC4
    bl fn_80203CCC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80223DB4
    stw r31, lbl_8047B610(r13)
    b @L_80223E2C
@L_80223DB4:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0xa
    stw r0, lbl_8047B610(r13)
    b @L_80223E2C
@L_80223DC4:
    cmplwi r4, 0x2
    bne @L_80223DF4
    bl fn_80203C5C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80223DE4
    stw r31, lbl_8047B610(r13)
    b @L_80223E2C
@L_80223DE4:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0xa
    stw r0, lbl_8047B610(r13)
    b @L_80223E2C
@L_80223DF4:
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80223E20
    mr r3, r30
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80223E20
    stw r31, lbl_8047B610(r13)
    b @L_80223E2C
@L_80223E20:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0xa
    stw r0, lbl_8047B610(r13)
@L_80223E2C:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

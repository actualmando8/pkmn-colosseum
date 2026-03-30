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
    mr r31, r3
    lwz r0, 0x2(r4)
    lwz r30, 0x6(r4)
    rlwinm r4, r0, 0, 16, 31
    cmplwi r4, 0x1
    bne @L_80223EA0
    bl fn_80203CCC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80223E90
    stw r30, lbl_8047B610(r13)
    b @L_80223F08
@L_80223E90:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0xa
    stw r0, lbl_8047B610(r13)
    b @L_80223F08
@L_80223EA0:
    cmplwi r4, 0x2
    bne @L_80223ED0
    bl fn_80203C5C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80223EC0
    stw r30, lbl_8047B610(r13)
    b @L_80223F08
@L_80223EC0:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0xa
    stw r0, lbl_8047B610(r13)
    b @L_80223F08
@L_80223ED0:
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80223EFC
    mr r3, r31
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80223EFC
    stw r30, lbl_8047B610(r13)
    b @L_80223F08
@L_80223EFC:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0xa
    stw r0, lbl_8047B610(r13)
@L_80223F08:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_80262308
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021EACC
    lbz r4, lbl_8047B642(r13)
    lwz r3, lbl_8047B610(r13)
    addi r4, r4, 0x1
    addi r0, r3, 0x5
    stb r4, lbl_8047B642(r13)
    stw r0, lbl_8047B610(r13)
    b @L_8021EAD8
@L_8021EACC:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
@L_8021EAD8:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

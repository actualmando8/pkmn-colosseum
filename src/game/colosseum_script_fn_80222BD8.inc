    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x14(r1)
    bl fn_801F025C
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    bl fn_80209F18
    lwz r0, lbl_8047B618(r13)
    li r6, 0x0
    la r5, lbl_80478D78(r13)
    lwz r3, lbl_8047B610(r13)
    rlwinm r4, r0, 0, 26, 24
    stb r6, 0x3(r5)
    addi r0, r3, 0x1
    stw r4, lbl_8047B618(r13)
    rlwinm r3, r4, 0, 18, 16
    stb r6, 0x6(r5)
    stw r3, lbl_8047B618(r13)
    stw r0, lbl_8047B610(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

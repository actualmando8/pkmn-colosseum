    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x14(r1)
    li r5, 0xf
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_80136468
    lwz r4, lbl_8047B610(r13)
    la r5, lbl_80478D78(r13)
    stb r3, 0x3(r5)
    addi r0, r4, 0x1
    stw r0, lbl_8047B610(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x0
    stw r0, 0x14(r1)
    bl fn_801F22D8
    lwz r3, lbl_8047B618(r13)
    rlwinm. r0, r3, 0, 19, 19
    ori r4, r3, 0x1000
    beq @L_8021E724
    rlwinm r4, r3, 0, 20, 18
@L_8021E724:
    lwz r3, lbl_8047B610(r13)
    stw r4, lbl_8047B618(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

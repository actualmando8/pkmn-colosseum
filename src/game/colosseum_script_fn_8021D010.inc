    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl fn_801F025C
    mr r31, r3
    bl fn_80205B8C
    li r4, 0x0
    li r5, 0x83
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021D070
    mr r3, r31
    li r4, 0x83
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_8021D070:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lwz r31, 0xc(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x12
    li r4, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl fn_801F025C
    li r4, 0x19
    mr r31, r3
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_802182B4
    mr r3, r31
    li r4, 0x19
    li r5, 0x0
    bl fn_8020248C
@L_802182B4:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lwz r31, 0xc(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

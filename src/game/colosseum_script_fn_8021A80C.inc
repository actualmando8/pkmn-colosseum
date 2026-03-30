    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x14(r1)
    li r5, 0x14
    li r6, 0x0
    stw r31, 0xc(r1)
    bl fn_801F54A4
    lwz r5, lbl_8047B610(r13)
    rlwinm r31, r3, 0, 16, 31
    li r4, 0x0
    lbz r3, 0x1(r5)
    bl fn_801F025C
    cmplwi r3, 0x0
    beq @L_8021A858
    mr r4, r31
    li r5, 0x1
    bl fn_802653FC
@L_8021A858:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x2
    stw r0, lbl_8047B610(r13)
    lwz r31, 0xc(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

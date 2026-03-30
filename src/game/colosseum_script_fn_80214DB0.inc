    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl fn_801F025C
    li r4, 0x0
    mr r31, r3
    li r5, 0x118
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    li r3, 0x0
    bl fn_801F221C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80214E08
@L_80214DF8:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_80214E3C
@L_80214E08:
    mr r3, r31
    li r4, 0x33
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80214DF8
    mr r3, r31
    li r4, 0x33
    li r5, 0x0
    bl fn_8020248C
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
@L_80214E3C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

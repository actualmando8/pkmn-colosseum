    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r3, lbl_8047B610(r13)
    lwz r4, 0x1(r3)
    cmplwi r4, 0x0
    beq @L_80226164
    la r3, lbl_80478D78(r13)
    lbz r0, 0x5(r3)
    rlwinm r0, r0, 2, 0, 29
    lwzx r3, r4, r0
    b @L_80226178
@L_80226164:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x50
    li r6, 0x0
    bl fn_801F54A4
@L_80226178:
    bl fn_802624CC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80226194
    li r0, 0x1
    la r3, lbl_80478D78(r13)
    stb r0, 0x7(r3)
@L_80226194:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

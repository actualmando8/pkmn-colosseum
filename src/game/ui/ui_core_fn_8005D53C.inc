    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lbz r0, lbl_8047A5A8(r13)
    mr r3, r4
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_8005D564
    li r0, 0x1
    b @L_8005D568
@L_8005D564:
    li r0, 0x0
@L_8005D568:
    clrlwi r4, r0, 24
    bl fn_80109220
    li r3, 0x0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

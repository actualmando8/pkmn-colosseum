    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x14(r1)
    bl fn_801F025C
    li r4, 0x0
    li r5, 0xed
    li r6, 0x0
    bl fn_8012640C
    rlwinm. r0, r3, 0, 16, 31
    beq @L_8021C740
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    b @L_8021C748
@L_8021C740:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
@L_8021C748:
    stw r0, lbl_8047B610(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

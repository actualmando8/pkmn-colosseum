    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lwz r3, lbl_8047B610(r13)
    lbz r3, 0x1(r3)
    bl fn_801F025C
    mr r0, r3
    li r3, 0x11
    mr r31, r0
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    bl fn_802096E8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021EA74
    lwz r3, lbl_8047B618(r13)
    rlwinm. r0, r3, 0, 23, 23
    beq @L_8021EA64
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x2
    stw r0, lbl_8047B610(r13)
    b @L_8021EA80
@L_8021EA64:
    rlwinm. r0, r3, 0, 24, 24
    beq @L_8021EA74
    mr r3, r31
    bl fn_80209380
@L_8021EA74:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x2
    stw r0, lbl_8047B610(r13)
@L_8021EA80:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

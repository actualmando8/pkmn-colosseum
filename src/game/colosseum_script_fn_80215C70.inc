    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl fn_801F025C
    mr r31, r3
    bl fn_80207BF4
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    bl fn_80207BF4
    rlwinm. r0, r3, 0, 16, 31
    mr r4, r3
    beq @L_80215CD0
    cmplwi r0, 0x19
    beq @L_80215CD0
    mr r3, r31
    bl fn_80207BC0
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
    b @L_80215CDC
@L_80215CD0:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
@L_80215CDC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

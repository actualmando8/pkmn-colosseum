    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x53
    stw r0, 0x14(r1)
    bl fn_801F2988
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    beq @L_80217570
    li r3, 0x0
    li r4, 0x0
    li r5, 0x3b
    li r6, 0x0
    li r7, 0x40
    bl fn_801F4C14
    li r0, 0x2
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    b @L_8021758C
@L_80217570:
    li r3, 0x0
    li r4, 0x53
    li r5, 0x0
    bl fn_801F2934
    li r0, 0x4
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
@L_8021758C:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

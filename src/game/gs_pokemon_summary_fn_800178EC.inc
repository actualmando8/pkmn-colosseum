    nofralloc
    lwz r0, lbl_8047A2E0(r13)
    cmpwi r0, 0x3
    beq @L_80017904
    li r0, 0x0
    stb r0, 0x67(r4)
    b @L_8001790C
    @L_80017904:
    li r0, 0xcc
    stb r0, 0x67(r4)
    @L_8001790C:
    li r3, 0x0
    blr

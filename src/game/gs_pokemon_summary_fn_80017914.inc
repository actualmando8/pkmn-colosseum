    nofralloc
    lwz r0, lbl_8047A2E0(r13)
    cmpwi r0, 0x3
    beq @L_8001792C
    li r0, 0x0
    stb r0, 0x67(r4)
    b @L_80017934
    @L_8001792C:
    li r0, 0xff
    stb r0, 0x67(r4)
    @L_80017934:
    li r3, 0x0
    blr

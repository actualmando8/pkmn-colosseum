    nofralloc
    lwz r3, 0x60(r3)
    lbz r0, 0x1c(r3)
    cmplwi r0, 0x0
    beq @L_8002AE80
    cmplwi r0, 0x1
    bne @L_8002AE8C
    @L_8002AE80:
    li r0, 0xcc
    stb r0, 0x67(r4)
    b @L_8002AE94
    @L_8002AE8C:
    li r0, 0x0
    stb r0, 0x67(r4)
    @L_8002AE94:
    li r3, 0x0
    blr

    nofralloc
    lwz r3, 0x60(r3)
    lbz r0, 0x1c(r3)
    cmplwi r0, 0x0
    beq @L_8002AEB4
    cmplwi r0, 0x1
    bne @L_8002AEE8
    @L_8002AEB4:
    mulli r6, r0, 0x3
    lis r3, lbl_80266E70@ha
    li r0, 0xff
    addi r5, r3, lbl_80266E70@l
    lbzx r3, r5, r6
    add r6, r5, r6
    lbz r5, 0x1(r6)
    stb r3, 0x64(r4)
    lbz r3, 0x2(r6)
    stb r5, 0x65(r4)
    stb r3, 0x66(r4)
    stb r0, 0x67(r4)
    b @L_8002AEF0
    @L_8002AEE8:
    li r0, 0x0
    stb r0, 0x67(r4)
    @L_8002AEF0:
    li r3, 0x0
    blr

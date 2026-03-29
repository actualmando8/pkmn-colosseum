    nofralloc
    lwz r3, 0x60(r3)
    lwz r3, 0x24(r3)
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    blt @L_80026438
    cmpwi r0, 0x2
    blt @L_80026448
    @L_80026438:
    li r0, 0xff
    mr r5, r0
    mr r6, r0
    b @L_80026464
    @L_80026448:
    mulli r0, r0, 0x18
    lis r3, lbl_80266E18@ha
    addi r5, r3, lbl_80266E18@l
    add r3, r5, r0
    lbzx r0, r5, r0
    lbz r5, 0x1(r3)
    lbz r6, 0x2(r3)
    @L_80026464:
    stb r0, 0x64(r4)
    li r3, 0x0
    stb r5, 0x65(r4)
    stb r6, 0x66(r4)
    blr

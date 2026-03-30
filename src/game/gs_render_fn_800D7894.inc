    nofralloc
    lwz r0, lbl_8047AAB0(r13)
    lwz r3, lbl_8047AAAC(r13)
    mtctr r0
    cmplwi r0, 0x0
    ble @L_800D7938
    @L_800D78A8:
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    bne @L_800D7930
    li r0, 0x1
    lis r4, lbl_803144D0@ha
    stb r0, 0x0(r3)
    addi r4, r4, lbl_803144D0@l
    lwz r0, lbl_8047AAB4(r13)
    slwi r0, r0, 2
    lwzx r0, r4, r0
    stw r0, 0x4(r3)
    lwz r4, lbl_8047AAB4(r13)
    addi r0, r4, 0x1
    cmplwi r0, 0x8
    stw r0, lbl_8047AAB4(r13)
    blt @L_800D78F0
    li r0, 0x0
    stw r0, lbl_8047AAB4(r13)
    @L_800D78F0:
    li r0, 0x0
    stb r0, 0x8(r3)
    stb r0, 0x24(r3)
    stb r0, 0x40(r3)
    stb r0, 0x5c(r3)
    stb r0, 0x78(r3)
    stb r0, 0x94(r3)
    stb r0, 0xb0(r3)
    stb r0, 0xcc(r3)
    stb r0, 0xe8(r3)
    stb r0, 0x104(r3)
    stb r0, 0x120(r3)
    stb r0, 0x13c(r3)
    stb r0, 0x158(r3)
    stb r0, 0x174(r3)
    blr
    @L_800D7930:
    addi r3, r3, 0x190
    bdnz @L_800D78A8
    @L_800D7938:
    li r3, 0x0
    blr

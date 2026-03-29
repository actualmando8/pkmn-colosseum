    nofralloc
    lwz r3, 0x60(r3)
    lwz r3, 0x24(r3)
    lwz r3, 0x0(r3)
    addi r3, r3, 0x1
    cmpwi r3, 0x2
    blt @L_800263CC
    subi r3, r3, 0x2
    @L_800263CC:
    cmpwi r3, 0x0
    blt @L_800263DC
    cmpwi r3, 0x2
    blt @L_800263EC
    @L_800263DC:
    li r0, 0xff
    mr r5, r0
    mr r6, r0
    b @L_80026408
    @L_800263EC:
    mulli r0, r3, 0x18
    lis r3, lbl_80266E18@ha
    addi r5, r3, lbl_80266E18@l
    add r3, r5, r0
    lbzx r0, r5, r0
    lbz r5, 0x1(r3)
    lbz r6, 0x2(r3)
    @L_80026408:
    stb r0, 0x64(r4)
    li r3, 0x0
    stb r5, 0x65(r4)
    stb r6, 0x66(r4)
    blr

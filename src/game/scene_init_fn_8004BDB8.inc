    nofralloc
    extsb r0, r3
    cmpwi r0, 0x0
    blt @L_8004BDD0
    lis r5, lbl_803A6A60@ha
    addi r5, r5, lbl_803A6A60@l
    stb r3, 0x1(r5)
    @L_8004BDD0:
    extsb r0, r4
    cmpwi r0, 0x0
    bltlr
    lis r3, lbl_803A6A60@ha
    addi r3, r3, lbl_803A6A60@l
    stb r4, 0x1(r3)
    blr

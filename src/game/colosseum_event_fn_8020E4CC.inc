    nofralloc
    cmpwi r3, 0x0
    bge @L_8020E4D8
    li r3, 0x0
@L_8020E4D8:
    cmpwi r3, 0xc
    blelr
    li r3, 0xc
    blr

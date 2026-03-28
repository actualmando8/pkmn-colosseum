    nofralloc
    stw r4, 0x0(r3)
    li r0, 0x2
    stw r0, 0x4(r3)
    lwz r4, 0x0(r3)
    lbz r0, 0x0(r4)
    extrwi r0, r0, 3, 25
    stb r0, 0x8(r3)
    lwz r4, 0x0(r3)
    lbz r0, 0x0(r4)
    clrlwi r0, r0, 28
    stb r0, 0x9(r3)
    lwz r4, 0x0(r3)
    addi r0, r4, 0x1
    stw r0, 0x0(r3)
    blr

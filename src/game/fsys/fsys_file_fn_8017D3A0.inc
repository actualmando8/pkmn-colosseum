    nofralloc
    lwz r0, 0x54(r3)
    cmpwi r0, 0xb
    beq @L_8017D3C4
    bge @L_8017D3CC
    cmpwi r0, 0x6
    bge @L_8017D3CC
    cmpwi r0, 0x4
    bge @L_8017D3C4
    b @L_8017D3CC
    @L_8017D3C4:
    li r3, 0x1
    blr
    @L_8017D3CC:
    li r3, 0x0
    blr

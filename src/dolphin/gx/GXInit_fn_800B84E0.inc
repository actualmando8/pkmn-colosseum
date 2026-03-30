    nofralloc
    cmpwi r3, 0x19
    bne @L_800B84EC
    li r3, 0xa
@L_800B84EC:
    li r0, 0x8
    subi r6, r3, 0x9
    lis r3, 0xcc01
    stb r0, -0x8000(r3)
    ori r0, r6, 0xa0
    rlwinm r4, r4, 0, 2, 31
    stb r0, -0x8000(r3)
    subic. r0, r6, 0xc
    stw r4, -0x8000(r3)
    blt @L_800B852C
    cmpwi r0, 0x4
    bge @L_800B852C
    lwz r3, -29632(r13)
    rlwinm r0, r0, 2, 0, 29
    add r3, r3, r0
    stw r4, 0x88(r3)
@L_800B852C:
    li r0, 0x8
    lis r3, 0xcc01
    stb r0, -0x8000(r3)
    ori r0, r6, 0xb0
    rlwinm r4, r5, 0, 24, 31
    stb r0, -0x8000(r3)
    subic. r0, r6, 0xc
    stw r4, -0x8000(r3)
    bltlr
    cmpwi r0, 0x4
    bgelr
    lwz r3, -29632(r13)
    rlwinm r0, r0, 2, 0, 29
    add r3, r3, r0
    stw r4, 0x98(r3)
    blr

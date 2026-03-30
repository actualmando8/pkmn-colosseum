    nofralloc
    cmpwi r4, 0x13
    rlwinm r0, r5, 0, 8, 31
    oris r7, r0, 0xf400
    beq @L_800BC6A0
    bge @L_800BC68C
    cmpwi r4, 0x11
    beq @L_800BC698
    b @L_800BC6B0
@L_800BC68C:
    cmpwi r4, 0x16
    beq @L_800BC6A8
    b @L_800BC6B0
@L_800BC698:
    li r6, 0x0
    b @L_800BC6B4
@L_800BC6A0:
    li r6, 0x1
    b @L_800BC6B4
@L_800BC6A8:
    li r6, 0x2
    b @L_800BC6B4
@L_800BC6B0:
    li r6, 0x2
@L_800BC6B4:
    li r0, 0x61
    lwz r4, -29632(r13)
    lis r5, 0xcc01
    stb r0, -0x8000(r5)
    rlwinm r6, r6, 0, 30, 27
    rlwinm r3, r3, 2, 0, 29
    stw r7, -0x8000(r5)
    or r3, r6, r3
    rlwinm r3, r3, 0, 8, 31
    stb r0, -0x8000(r5)
    oris r3, r3, 0xf500
    li r0, 0x0
    stw r3, -0x8000(r5)
    sth r0, 0x2(r4)
    blr

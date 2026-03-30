    nofralloc
    cmpwi r3, 0x3
    beq @L_800BA648
    bge @L_800BA5E0
    cmpwi r3, 0x1
    beq @L_800BA610
    bge @L_800BA630
    cmpwi r3, 0x0
    bge @L_800BA5F0
    blr
@L_800BA5E0:
    cmpwi r3, 0x5
    beq @L_800BA66C
    bgelr
    b @L_800BA660
@L_800BA5F0:
    lwz r3, -29632(r13)
    li r5, 0x0
    lwz r0, 0x0(r4)
    lwz r3, 0xb0(r3)
    rlwinm r0, r0, 0, 0, 23
    mr r7, r0
    rlwimi r7, r3, 0, 24, 31
    b @L_800BA67C
@L_800BA610:
    lwz r3, -29632(r13)
    li r5, 0x1
    lwz r0, 0x0(r4)
    lwz r3, 0xb4(r3)
    rlwinm r0, r0, 0, 0, 23
    mr r7, r0
    rlwimi r7, r3, 0, 24, 31
    b @L_800BA67C
@L_800BA630:
    lwz r3, -29632(r13)
    li r5, 0x0
    lbz r7, 0x3(r4)
    lwz r3, 0xb0(r3)
    rlwimi r7, r3, 0, 0, 23
    b @L_800BA67C
@L_800BA648:
    lwz r3, -29632(r13)
    li r5, 0x1
    lbz r7, 0x3(r4)
    lwz r3, 0xb4(r3)
    rlwimi r7, r3, 0, 0, 23
    b @L_800BA67C
@L_800BA660:
    lwz r7, 0x0(r4)
    li r5, 0x0
    b @L_800BA67C
@L_800BA66C:
    lwz r7, 0x0(r4)
    li r5, 0x1
    b @L_800BA67C
    blr
@L_800BA67C:
    li r0, 0x10
    lwz r4, -29632(r13)
    lis r6, 0xcc01
    stb r0, -0x8000(r6)
    addi r3, r5, 0x100c
    rlwinm r0, r5, 2, 0, 29
    stw r3, -0x8000(r6)
    li r5, 0x1
    add r3, r4, r0
    stw r7, -0x8000(r6)
    sth r5, 0x2(r4)
    stw r7, 0xb0(r3)
    blr

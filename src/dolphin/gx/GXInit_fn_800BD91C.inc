    nofralloc
    lwz r5, -29632(r13)
    lwz r0, 0x4e4(r5)
    cmpwi r0, 0x22
    beq @L_800BD954
    bge @L_800BD9A0
    cmpwi r0, 0xb
    bge @L_800BD944
    cmpwi r0, 0x0
    bge @L_800BD954
    b @L_800BD9A0
@L_800BD944:
    cmpwi r0, 0x1b
    bge @L_800BD98C
    b @L_800BD974
    b @L_800BD9A0
@L_800BD954:
    li r0, 0x10
    lis r6, 0xcc01
    stb r0, -0x8000(r6)
    li r5, 0x1006
    li r0, 0x0
    stw r5, -0x8000(r6)
    stw r0, -0x8000(r6)
    b @L_800BD9A0
@L_800BD974:
    li r0, 0x61
    lis r5, 0xcc01
    stb r0, -0x8000(r5)
    lis r0, 0x2300
    stw r0, -0x8000(r5)
    b @L_800BD9A0
@L_800BD98C:
    li r0, 0x61
    lis r5, 0xcc01
    stb r0, -0x8000(r5)
    lis r0, 0x2400
    stw r0, -0x8000(r5)
@L_800BD9A0:
    lwz r7, -29632(r13)
    lwz r0, 0x4e8(r7)
    cmpwi r0, 0x15
    beq @L_800BD9D8
    bge @L_800BDA28
    cmpwi r0, 0x9
    bge @L_800BD9C8
    cmpwi r0, 0x0
    bge @L_800BD9D8
    b @L_800BDA28
@L_800BD9C8:
    cmpwi r0, 0x11
    bge @L_800BDA1C
    b @L_800BD9F0
    b @L_800BDA28
@L_800BD9D8:
    li r0, 0x61
    lis r5, 0xcc01
    stb r0, -0x8000(r5)
    lis r0, 0x6700
    stw r0, -0x8000(r5)
    b @L_800BDA28
@L_800BD9F0:
    lwz r0, 0x4ec(r7)
    li r6, 0x8
    lis r5, 0xcc01
    rlwinm r0, r0, 0, 28, 23
    stw r0, 0x4ec(r7)
    li r0, 0x20
    stb r6, -0x8000(r5)
    stb r0, -0x8000(r5)
    lwz r0, 0x4ec(r7)
    stw r0, -0x8000(r5)
    b @L_800BDA28
@L_800BDA1C:
    lwz r5, -24228(r13)
    li r0, 0x0
    sth r0, 0x6(r5)
@L_800BDA28:
    lwz r5, -29632(r13)
    stw r3, 0x4e4(r5)
    lwz r0, 0x4e4(r5)
    cmplwi r0, 0x23
    bgt @L_800BDE54
    lis r3, -32719
    addi r3, r3, 13956
    rlwinm r0, r0, 2, 0, 29
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r0, 0x10
    lis r5, 0xcc01
    stb r0, -0x8000(r5)
    li r3, 0x1006
    li r0, 0x273
    stw r3, -0x8000(r5)
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x10
    lis r5, 0xcc01
    stb r0, -0x8000(r5)
    li r3, 0x1006
    li r0, 0x14a
    stw r3, -0x8000(r5)
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x10
    lis r5, 0xcc01
    stb r0, -0x8000(r5)
    li r3, 0x1006
    li r0, 0x16b
    stw r3, -0x8000(r5)
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x10
    lis r5, 0xcc01
    stb r0, -0x8000(r5)
    li r3, 0x1006
    li r0, 0x84
    stw r3, -0x8000(r5)
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x10
    lis r5, 0xcc01
    stb r0, -0x8000(r5)
    li r3, 0x1006
    li r0, 0xc6
    stw r3, -0x8000(r5)
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x10
    lis r5, 0xcc01
    stb r0, -0x8000(r5)
    li r3, 0x1006
    li r0, 0x210
    stw r3, -0x8000(r5)
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x10
    lis r5, 0xcc01
    stb r0, -0x8000(r5)
    li r3, 0x1006
    li r0, 0x252
    stw r3, -0x8000(r5)
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x10
    lis r5, 0xcc01
    stb r0, -0x8000(r5)
    li r3, 0x1006
    li r0, 0x231
    stw r3, -0x8000(r5)
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x10
    lis r5, 0xcc01
    stb r0, -0x8000(r5)
    li r3, 0x1006
    li r0, 0x1ad
    stw r3, -0x8000(r5)
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x10
    lis r5, 0xcc01
    stb r0, -0x8000(r5)
    li r3, 0x1006
    li r0, 0x1ce
    stw r3, -0x8000(r5)
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x10
    lis r5, 0xcc01
    stb r0, -0x8000(r5)
    li r3, 0x1006
    li r0, 0x21
    stw r3, -0x8000(r5)
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x10
    lis r5, 0xcc01
    stb r0, -0x8000(r5)
    li r3, 0x1006
    li r0, 0x153
    stw r3, -0x8000(r5)
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x61
    lis r5, 0xcc01
    lis r3, 0x2301
    stb r0, -0x8000(r5)
    subi r0, r3, 0x5181
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x61
    lis r5, 0xcc01
    lis r3, 0x2301
    stb r0, -0x8000(r5)
    subi r0, r3, 0x7181
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x61
    lis r5, 0xcc01
    lis r3, 0x2301
    stb r0, -0x8000(r5)
    subi r0, r3, 0x6181
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x61
    lis r5, 0xcc01
    lis r3, 0x2300
    stb r0, -0x8000(r5)
    addi r0, r3, 0x1e7f
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x61
    lis r5, 0xcc01
    lis r3, 0x2301
    stb r0, -0x8000(r5)
    subi r0, r3, 0x53c1
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x61
    lis r5, 0xcc01
    lis r3, 0x2301
    stb r0, -0x8000(r5)
    subi r0, r3, 0x5381
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x61
    lis r5, 0xcc01
    lis r3, 0x2301
    stb r0, -0x8000(r5)
    subi r0, r3, 0x5341
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x61
    lis r5, 0xcc01
    lis r3, 0x2301
    stb r0, -0x8000(r5)
    subi r0, r3, 0x5301
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x61
    lis r5, 0xcc01
    lis r3, 0x2301
    stb r0, -0x8000(r5)
    subi r0, r3, 0x52c1
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x61
    lis r5, 0xcc01
    lis r3, 0x2301
    stb r0, -0x8000(r5)
    subi r0, r3, 0x5281
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x61
    lis r5, 0xcc01
    lis r3, 0x2301
    stb r0, -0x8000(r5)
    subi r0, r3, 0x5241
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x61
    lis r5, 0xcc01
    lis r3, 0x2301
    stb r0, -0x8000(r5)
    subi r0, r3, 0x5201
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x61
    lis r5, 0xcc01
    lis r3, 0x2301
    stb r0, -0x8000(r5)
    subi r0, r3, 0x51c1
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x61
    lis r5, 0xcc01
    lis r3, 0x2301
    stb r0, -0x8000(r5)
    subi r0, r3, 0x5d81
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x61
    lis r5, 0xcc01
    lis r3, 0x2301
    stb r0, -0x8000(r5)
    subi r0, r3, 0x5981
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x61
    lis r5, 0xcc01
    lis r3, 0x2301
    stb r0, -0x8000(r5)
    subi r0, r3, 0x5581
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x61
    lis r5, 0xcc01
    lis r3, 0x2403
    stb r0, -0x8000(r5)
    subi r0, r3, 0x3f3a
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x61
    lis r5, 0xcc01
    lis r3, 0x2403
    stb r0, -0x8000(r5)
    subi r0, r3, 0x3e95
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x61
    lis r5, 0xcc01
    lis r3, 0x2403
    stb r0, -0x8000(r5)
    subi r0, r3, 0x3f19
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x61
    lis r5, 0xcc01
    lis r3, 0x2403
    stb r0, -0x8000(r5)
    subi r0, r3, 0x3ef8
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x61
    lis r5, 0xcc01
    lis r3, 0x2403
    stb r0, -0x8000(r5)
    subi r0, r3, 0x3ed7
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x61
    lis r5, 0xcc01
    lis r3, 0x2403
    stb r0, -0x8000(r5)
    subi r0, r3, 0x3eb6
    stw r0, -0x8000(r5)
    b @L_800BDE54
    li r0, 0x61
    lis r5, 0xcc01
    lis r3, 0x2403
    stb r0, -0x8000(r5)
    subi r0, r3, 0x3e53
    stw r0, -0x8000(r5)
@L_800BDE54:
    lwz r3, -29632(r13)
    stw r4, 0x4e8(r3)
    lwz r0, 0x4e8(r3)
    cmplwi r0, 0x16
    bgt @L_800BE154
    lis r4, -32719
    addi r4, r4, 13864
    rlwinm r0, r0, 2, 0, 29
    lwzx r0, r4, r0
    mtctr r0
    bctr
    li r0, 0x61
    lis r4, 0xcc01
    lis r3, 0x6700
    stb r0, -0x8000(r4)
    addi r0, r3, 0x42
    stw r0, -0x8000(r4)
    b @L_800BE154
    li r0, 0x61
    lis r4, 0xcc01
    lis r3, 0x6700
    stb r0, -0x8000(r4)
    addi r0, r3, 0x84
    stw r0, -0x8000(r4)
    b @L_800BE154
    li r0, 0x61
    lis r4, 0xcc01
    lis r3, 0x6700
    stb r0, -0x8000(r4)
    addi r0, r3, 0x63
    stw r0, -0x8000(r4)
    b @L_800BE154
    li r0, 0x61
    lis r4, 0xcc01
    lis r3, 0x6700
    stb r0, -0x8000(r4)
    addi r0, r3, 0x129
    stw r0, -0x8000(r4)
    b @L_800BE154
    li r0, 0x61
    lis r4, 0xcc01
    lis r3, 0x6700
    stb r0, -0x8000(r4)
    addi r0, r3, 0x252
    stw r0, -0x8000(r4)
    b @L_800BE154
    li r0, 0x61
    lis r4, 0xcc01
    lis r3, 0x6700
    stb r0, -0x8000(r4)
    addi r0, r3, 0x21
    stw r0, -0x8000(r4)
    b @L_800BE154
    li r0, 0x61
    lis r4, 0xcc01
    lis r3, 0x6700
    stb r0, -0x8000(r4)
    addi r0, r3, 0x14b
    stw r0, -0x8000(r4)
    b @L_800BE154
    li r0, 0x61
    lis r4, 0xcc01
    lis r3, 0x6700
    stb r0, -0x8000(r4)
    addi r0, r3, 0x18d
    stw r0, -0x8000(r4)
    b @L_800BE154
    li r0, 0x61
    lis r4, 0xcc01
    lis r3, 0x6700
    stb r0, -0x8000(r4)
    addi r0, r3, 0x1cf
    stw r0, -0x8000(r4)
    b @L_800BE154
    li r0, 0x61
    lis r4, 0xcc01
    lis r3, 0x6700
    stb r0, -0x8000(r4)
    addi r0, r3, 0x211
    stw r0, -0x8000(r4)
    b @L_800BE154
    lwz r0, 0x4ec(r3)
    li r5, 0x8
    lis r4, 0xcc01
    rlwinm r0, r0, 0, 28, 23
    ori r0, r0, 0x20
    stw r0, 0x4ec(r3)
    li r0, 0x20
    stb r5, -0x8000(r4)
    stb r0, -0x8000(r4)
    lwz r0, 0x4ec(r3)
    stw r0, -0x8000(r4)
    b @L_800BE154
    lwz r0, 0x4ec(r3)
    li r5, 0x8
    lis r4, 0xcc01
    rlwinm r0, r0, 0, 28, 23
    ori r0, r0, 0x30
    stw r0, 0x4ec(r3)
    li r0, 0x20
    stb r5, -0x8000(r4)
    stb r0, -0x8000(r4)
    lwz r0, 0x4ec(r3)
    stw r0, -0x8000(r4)
    b @L_800BE154
    lwz r0, 0x4ec(r3)
    li r5, 0x8
    lis r4, 0xcc01
    rlwinm r0, r0, 0, 28, 23
    ori r0, r0, 0x40
    stw r0, 0x4ec(r3)
    li r0, 0x20
    stb r5, -0x8000(r4)
    stb r0, -0x8000(r4)
    lwz r0, 0x4ec(r3)
    stw r0, -0x8000(r4)
    b @L_800BE154
    lwz r0, 0x4ec(r3)
    li r5, 0x8
    lis r4, 0xcc01
    rlwinm r0, r0, 0, 28, 23
    ori r0, r0, 0x50
    stw r0, 0x4ec(r3)
    li r0, 0x20
    stb r5, -0x8000(r4)
    stb r0, -0x8000(r4)
    lwz r0, 0x4ec(r3)
    stw r0, -0x8000(r4)
    b @L_800BE154
    lwz r0, 0x4ec(r3)
    li r5, 0x8
    lis r4, 0xcc01
    rlwinm r0, r0, 0, 28, 23
    ori r0, r0, 0x60
    stw r0, 0x4ec(r3)
    li r0, 0x20
    stb r5, -0x8000(r4)
    stb r0, -0x8000(r4)
    lwz r0, 0x4ec(r3)
    stw r0, -0x8000(r4)
    b @L_800BE154
    lwz r0, 0x4ec(r3)
    li r5, 0x8
    lis r4, 0xcc01
    rlwinm r0, r0, 0, 28, 23
    ori r0, r0, 0x70
    stw r0, 0x4ec(r3)
    li r0, 0x20
    stb r5, -0x8000(r4)
    stb r0, -0x8000(r4)
    lwz r0, 0x4ec(r3)
    stw r0, -0x8000(r4)
    b @L_800BE154
    lwz r0, 0x4ec(r3)
    li r5, 0x8
    lis r4, 0xcc01
    rlwinm r0, r0, 0, 28, 23
    ori r0, r0, 0x90
    stw r0, 0x4ec(r3)
    li r0, 0x20
    stb r5, -0x8000(r4)
    stb r0, -0x8000(r4)
    lwz r0, 0x4ec(r3)
    stw r0, -0x8000(r4)
    b @L_800BE154
    lwz r0, 0x4ec(r3)
    li r5, 0x8
    lis r4, 0xcc01
    rlwinm r0, r0, 0, 28, 23
    ori r0, r0, 0x80
    stw r0, 0x4ec(r3)
    li r0, 0x20
    stb r5, -0x8000(r4)
    stb r0, -0x8000(r4)
    lwz r0, 0x4ec(r3)
    stw r0, -0x8000(r4)
    b @L_800BE154
    lwz r3, -24228(r13)
    li r0, 0x2
    sth r0, 0x6(r3)
    b @L_800BE154
    lwz r3, -24228(r13)
    li r0, 0x3
    sth r0, 0x6(r3)
    b @L_800BE154
    lwz r3, -24228(r13)
    li r0, 0x4
    sth r0, 0x6(r3)
    b @L_800BE154
    lwz r3, -24228(r13)
    li r0, 0x5
    sth r0, 0x6(r3)
@L_800BE154:
    lwz r3, -29632(r13)
    li r0, 0x0
    sth r0, 0x2(r3)
    blr

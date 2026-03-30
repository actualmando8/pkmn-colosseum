    nofralloc
    mflr r0
    cmplwi r5, 0x14
    stw r0, 0x4(r1)
    li r11, 0x0
    li r12, 0x0
    stwu r1, -0x8(r1)
    li r10, 0x5
    bgt @L_800B8630
    lis r9, -32719
    addi r9, r9, 11244
    rlwinm r0, r5, 2, 0, 29
    lwzx r0, r9, r0
    mtctr r0
    bctr
    li r10, 0x0
    li r12, 0x1
    b @L_800B8630
    li r10, 0x1
    li r12, 0x1
    b @L_800B8630
    li r10, 0x3
    li r12, 0x1
    b @L_800B8630
    li r10, 0x4
    li r12, 0x1
    b @L_800B8630
    li r10, 0x2
    b @L_800B8630
    li r10, 0x2
    b @L_800B8630
    li r10, 0x5
    b @L_800B8630
    li r10, 0x6
    b @L_800B8630
    li r10, 0x7
    b @L_800B8630
    li r10, 0x8
    b @L_800B8630
    li r10, 0x9
    b @L_800B8630
    li r10, 0xa
    b @L_800B8630
    li r10, 0xb
    b @L_800B8630
    li r10, 0xc
@L_800B8630:
    cmpwi r4, 0x1
    beq @L_800B8658
    bge @L_800B8648
    cmpwi r4, 0x0
    bge @L_800B866C
    b @L_800B86E8
@L_800B8648:
    cmpwi r4, 0xa
    beq @L_800B86C0
    bge @L_800B86E8
    b @L_800B8684
@L_800B8658:
    rlwinm r0, r12, 2, 0, 29
    rlwinm r4, r0, 0, 28, 19
    rlwinm r0, r10, 7, 0, 24
    or r11, r4, r0
    b @L_800B86E8
@L_800B866C:
    rlwinm r0, r12, 2, 0, 29
    ori r0, r0, 0x2
    rlwinm r4, r0, 0, 28, 19
    rlwinm r0, r10, 7, 0, 24
    or r11, r4, r0
    b @L_800B86E8
@L_800B8684:
    rlwinm r0, r12, 2, 0, 29
    rlwinm r0, r0, 0, 28, 24
    ori r0, r0, 0x10
    rlwinm r9, r0, 0, 25, 19
    rlwinm r0, r10, 7, 0, 24
    or r9, r9, r0
    subi r5, r5, 0xc
    subi r0, r4, 0x2
    rlwinm r9, r9, 0, 20, 16
    rlwinm r4, r5, 12, 0, 19
    or r4, r9, r4
    rlwinm r4, r4, 0, 17, 13
    rlwinm r0, r0, 15, 0, 16
    or r11, r4, r0
    b @L_800B86E8
@L_800B86C0:
    cmpwi r5, 0x13
    rlwinm r0, r12, 2, 0, 29
    bne @L_800B86D8
    rlwinm r0, r0, 0, 28, 24
    ori r0, r0, 0x20
    b @L_800B86E0
@L_800B86D8:
    rlwinm r0, r0, 0, 28, 24
    ori r0, r0, 0x30
@L_800B86E0:
    rlwinm r0, r0, 0, 25, 19
    ori r11, r0, 0x100
@L_800B86E8:
    li r10, 0x10
    lis r9, 0xcc01
    stb r10, -0x8000(r9)
    addi r0, r3, 0x1040
    subi r4, r8, 0x40
    stw r0, -0x8000(r9)
    rlwinm r5, r4, 0, 24, 22
    clrlslwi r4, r7, 24, 8
    stw r11, -0x8000(r9)
    addi r0, r3, 0x1050
    cmplwi r3, 0x6
    stb r10, -0x8000(r9)
    or r4, r5, r4
    stw r0, -0x8000(r9)
    stw r4, -0x8000(r9)
    bgt @L_800B8818
    lis r4, -32719
    addi r4, r4, 11216
    rlwinm r0, r3, 2, 0, 29
    lwzx r0, r4, r0
    mtctr r0
    bctr
    lwz r4, -29632(r13)
    rlwinm r0, r6, 6, 0, 25
    addi r5, r4, 0x80
    lwz r4, 0x80(r4)
    rlwinm r4, r4, 0, 26, 19
    or r0, r4, r0
    stw r0, 0x0(r5)
    b @L_800B8834
    lwz r4, -29632(r13)
    rlwinm r0, r6, 12, 0, 19
    addi r5, r4, 0x80
    lwz r4, 0x80(r4)
    rlwinm r4, r4, 0, 20, 13
    or r0, r4, r0
    stw r0, 0x0(r5)
    b @L_800B8834
    lwz r4, -29632(r13)
    rlwinm r0, r6, 18, 0, 13
    addi r5, r4, 0x80
    lwz r4, 0x80(r4)
    rlwinm r4, r4, 0, 14, 7
    or r0, r4, r0
    stw r0, 0x0(r5)
    b @L_800B8834
    lwz r4, -29632(r13)
    rlwinm r0, r6, 24, 0, 7
    addi r5, r4, 0x80
    lwz r4, 0x80(r4)
    rlwinm r4, r4, 0, 8, 1
    or r0, r4, r0
    stw r0, 0x0(r5)
    b @L_800B8834
    lwz r4, -29632(r13)
    lwzu r0, 0x84(r4)
    rlwinm r0, r0, 0, 0, 25
    or r0, r0, r6
    stw r0, 0x0(r4)
    b @L_800B8834
    lwz r4, -29632(r13)
    rlwinm r0, r6, 6, 0, 25
    addi r5, r4, 0x84
    lwz r4, 0x84(r4)
    rlwinm r4, r4, 0, 26, 19
    or r0, r4, r0
    stw r0, 0x0(r5)
    b @L_800B8834
    lwz r4, -29632(r13)
    rlwinm r0, r6, 12, 0, 19
    addi r5, r4, 0x84
    lwz r4, 0x84(r4)
    rlwinm r4, r4, 0, 20, 13
    or r0, r4, r0
    stw r0, 0x0(r5)
    b @L_800B8834
@L_800B8818:
    lwz r4, -29632(r13)
    rlwinm r0, r6, 18, 0, 13
    addi r5, r4, 0x84
    lwz r4, 0x84(r4)
    rlwinm r4, r4, 0, 14, 7
    or r0, r4, r0
    stw r0, 0x0(r5)
@L_800B8834:
    addi r3, r3, 0x1
    bl fn_800BD898
    lwz r0, 0xc(r1)
    addi r1, r1, 0x8
    mtlr r0
    blr

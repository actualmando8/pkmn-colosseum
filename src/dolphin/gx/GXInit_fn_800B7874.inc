    nofralloc
    cmplwi r3, 0x19
    bgt @L_800B7B68
    lis r5, -32719
    addi r5, r5, 10976
    rlwinm r0, r3, 2, 0, 29
    lwzx r0, r5, r0
    mtctr r0
    bctr
    lwz r3, -29632(r13)
    lwzu r0, 0x14(r3)
    rlwinm r0, r0, 0, 0, 30
    or r0, r0, r4
    stw r0, 0x0(r3)
    b @L_800B7B68
    lwz r3, -29632(r13)
    rlwinm r0, r4, 1, 0, 30
    addi r4, r3, 0x14
    lwz r3, 0x14(r3)
    rlwinm r3, r3, 0, 31, 29
    or r0, r3, r0
    stw r0, 0x0(r4)
    b @L_800B7B68
    lwz r3, -29632(r13)
    rlwinm r0, r4, 2, 0, 29
    addi r4, r3, 0x14
    lwz r3, 0x14(r3)
    rlwinm r3, r3, 0, 30, 28
    or r0, r3, r0
    stw r0, 0x0(r4)
    b @L_800B7B68
    lwz r3, -29632(r13)
    rlwinm r0, r4, 3, 0, 28
    addi r4, r3, 0x14
    lwz r3, 0x14(r3)
    rlwinm r3, r3, 0, 29, 27
    or r0, r3, r0
    stw r0, 0x0(r4)
    b @L_800B7B68
    lwz r3, -29632(r13)
    rlwinm r0, r4, 4, 0, 27
    addi r4, r3, 0x14
    lwz r3, 0x14(r3)
    rlwinm r3, r3, 0, 28, 26
    or r0, r3, r0
    stw r0, 0x0(r4)
    b @L_800B7B68
    lwz r3, -29632(r13)
    rlwinm r0, r4, 5, 0, 26
    addi r4, r3, 0x14
    lwz r3, 0x14(r3)
    rlwinm r3, r3, 0, 27, 25
    or r0, r3, r0
    stw r0, 0x0(r4)
    b @L_800B7B68
    lwz r3, -29632(r13)
    rlwinm r0, r4, 6, 0, 25
    addi r4, r3, 0x14
    lwz r3, 0x14(r3)
    rlwinm r3, r3, 0, 26, 24
    or r0, r3, r0
    stw r0, 0x0(r4)
    b @L_800B7B68
    lwz r3, -29632(r13)
    rlwinm r0, r4, 7, 0, 24
    addi r4, r3, 0x14
    lwz r3, 0x14(r3)
    rlwinm r3, r3, 0, 25, 23
    or r0, r3, r0
    stw r0, 0x0(r4)
    b @L_800B7B68
    lwz r3, -29632(r13)
    rlwinm r0, r4, 8, 0, 23
    addi r4, r3, 0x14
    lwz r3, 0x14(r3)
    rlwinm r3, r3, 0, 24, 22
    or r0, r3, r0
    stw r0, 0x0(r4)
    b @L_800B7B68
    lwz r3, -29632(r13)
    rlwinm r0, r4, 9, 0, 22
    addi r4, r3, 0x14
    lwz r3, 0x14(r3)
    rlwinm r3, r3, 0, 23, 20
    or r0, r3, r0
    stw r0, 0x0(r4)
    b @L_800B7B68
    cmpwi r4, 0x0
    beq @L_800B79F0
    lwz r3, -29632(r13)
    li r5, 0x1
    li r0, 0x0
    stb r5, 0x41c(r3)
    stb r0, 0x41d(r3)
    stw r4, 0x418(r3)
    b @L_800B7B68
@L_800B79F0:
    lwz r3, -29632(r13)
    li r0, 0x0
    stb r0, 0x41c(r3)
    b @L_800B7B68
    cmpwi r4, 0x0
    beq @L_800B7A24
    lwz r3, -29632(r13)
    li r5, 0x1
    li r0, 0x0
    stb r5, 0x41d(r3)
    stb r0, 0x41c(r3)
    stw r4, 0x418(r3)
    b @L_800B7B68
@L_800B7A24:
    lwz r3, -29632(r13)
    li r0, 0x0
    stb r0, 0x41d(r3)
    b @L_800B7B68
    lwz r3, -29632(r13)
    rlwinm r0, r4, 13, 0, 18
    addi r4, r3, 0x14
    lwz r3, 0x14(r3)
    rlwinm r3, r3, 0, 19, 16
    or r0, r3, r0
    stw r0, 0x0(r4)
    b @L_800B7B68
    lwz r3, -29632(r13)
    rlwinm r0, r4, 15, 0, 16
    addi r4, r3, 0x14
    lwz r3, 0x14(r3)
    rlwinm r3, r3, 0, 17, 14
    or r0, r3, r0
    stw r0, 0x0(r4)
    b @L_800B7B68
    lwz r3, -29632(r13)
    lwzu r0, 0x18(r3)
    rlwinm r0, r0, 0, 0, 29
    or r0, r0, r4
    stw r0, 0x0(r3)
    b @L_800B7B68
    lwz r3, -29632(r13)
    rlwinm r0, r4, 2, 0, 29
    addi r4, r3, 0x18
    lwz r3, 0x18(r3)
    rlwinm r3, r3, 0, 30, 27
    or r0, r3, r0
    stw r0, 0x0(r4)
    b @L_800B7B68
    lwz r3, -29632(r13)
    rlwinm r0, r4, 4, 0, 27
    addi r4, r3, 0x18
    lwz r3, 0x18(r3)
    rlwinm r3, r3, 0, 28, 25
    or r0, r3, r0
    stw r0, 0x0(r4)
    b @L_800B7B68
    lwz r3, -29632(r13)
    rlwinm r0, r4, 6, 0, 25
    addi r4, r3, 0x18
    lwz r3, 0x18(r3)
    rlwinm r3, r3, 0, 26, 23
    or r0, r3, r0
    stw r0, 0x0(r4)
    b @L_800B7B68
    lwz r3, -29632(r13)
    rlwinm r0, r4, 8, 0, 23
    addi r4, r3, 0x18
    lwz r3, 0x18(r3)
    rlwinm r3, r3, 0, 24, 21
    or r0, r3, r0
    stw r0, 0x0(r4)
    b @L_800B7B68
    lwz r3, -29632(r13)
    rlwinm r0, r4, 10, 0, 21
    addi r4, r3, 0x18
    lwz r3, 0x18(r3)
    rlwinm r3, r3, 0, 22, 19
    or r0, r3, r0
    stw r0, 0x0(r4)
    b @L_800B7B68
    lwz r3, -29632(r13)
    rlwinm r0, r4, 12, 0, 19
    addi r4, r3, 0x18
    lwz r3, 0x18(r3)
    rlwinm r3, r3, 0, 20, 17
    or r0, r3, r0
    stw r0, 0x0(r4)
    b @L_800B7B68
    lwz r3, -29632(r13)
    rlwinm r0, r4, 14, 0, 17
    addi r4, r3, 0x18
    lwz r3, 0x18(r3)
    rlwinm r3, r3, 0, 18, 15
    or r0, r3, r0
    stw r0, 0x0(r4)
@L_800B7B68:
    lwz r3, -29632(r13)
    lbz r0, 0x41c(r3)
    cmplwi r0, 0x0
    bne @L_800B7B84
    lbz r0, 0x41d(r3)
    cmplwi r0, 0x0
    beq @L_800B7BA4
@L_800B7B84:
    addi r4, r3, 0x14
    lwz r0, 0x418(r3)
    lwz r3, 0x14(r3)
    rlwinm r0, r0, 11, 0, 20
    rlwinm r3, r3, 0, 21, 18
    or r0, r3, r0
    stw r0, 0x0(r4)
    b @L_800B7BB0
@L_800B7BA4:
    lwzu r0, 0x14(r3)
    rlwinm r0, r0, 0, 21, 18
    stw r0, 0x0(r3)
@L_800B7BB0:
    lwz r3, -29632(r13)
    lwz r0, 0x4f4(r3)
    ori r0, r0, 0x8
    stw r0, 0x4f4(r3)
    blr

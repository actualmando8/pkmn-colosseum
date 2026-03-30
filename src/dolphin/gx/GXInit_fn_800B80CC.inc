    nofralloc
    lwz r6, -29632(r13)
    rlwinm r0, r3, 2, 0, 29
    lis r5, -32719
    add r6, r6, r0
    addi r8, r6, 0x1c
    addi r9, r6, 0x3c
    addi r10, r6, 0x5c
    addi r5, r5, 11148
    b @L_800B8408
@L_800B80F0:
    lwz r6, 0x0(r4)
    lbz r7, 0xc(r4)
    subi r11, r6, 0x9
    lwz r6, 0x8(r4)
    cmplwi r11, 0x10
    lwz r0, 0x4(r4)
    bgt @L_800B8404
    rlwinm r11, r11, 2, 0, 29
    lwzx r11, r5, r11
    mtctr r11
    bctr
    lwz r12, 0x0(r8)
    rlwinm r11, r6, 1, 0, 30
    rlwinm r6, r7, 4, 0, 27
    rlwinm r7, r12, 0, 0, 30
    or r0, r7, r0
    stw r0, 0x0(r8)
    lwz r0, 0x0(r8)
    rlwinm r0, r0, 0, 31, 27
    or r0, r0, r11
    stw r0, 0x0(r8)
    lwz r0, 0x0(r8)
    rlwinm r0, r0, 0, 28, 22
    or r0, r0, r6
    stw r0, 0x0(r8)
    b @L_800B8404
    lwz r7, 0x0(r8)
    rlwinm r6, r6, 10, 0, 21
    cmpwi r0, 0x2
    rlwinm r7, r7, 0, 22, 18
    or r6, r7, r6
    stw r6, 0x0(r8)
    bne @L_800B8198
    lwz r0, 0x0(r8)
    rlwinm r0, r0, 0, 23, 21
    ori r0, r0, 0x200
    stw r0, 0x0(r8)
    lwz r0, 0x0(r8)
    rlwinm r0, r0, 0, 1, 31
    oris r0, r0, 0x8000
    stw r0, 0x0(r8)
    b @L_800B8404
@L_800B8198:
    lwz r6, 0x0(r8)
    rlwinm r0, r0, 9, 0, 22
    rlwinm r6, r6, 0, 23, 21
    or r0, r6, r0
    stw r0, 0x0(r8)
    lwz r0, 0x0(r8)
    rlwinm r0, r0, 0, 1, 31
    stw r0, 0x0(r8)
    b @L_800B8404
    lwz r11, 0x0(r8)
    rlwinm r7, r0, 13, 0, 18
    rlwinm r0, r6, 14, 0, 17
    rlwinm r6, r11, 0, 19, 17
    or r6, r6, r7
    stw r6, 0x0(r8)
    lwz r6, 0x0(r8)
    rlwinm r6, r6, 0, 18, 14
    or r0, r6, r0
    stw r0, 0x0(r8)
    b @L_800B8404
    lwz r11, 0x0(r8)
    rlwinm r7, r0, 17, 0, 14
    rlwinm r0, r6, 18, 0, 13
    rlwinm r6, r11, 0, 15, 13
    or r6, r6, r7
    stw r6, 0x0(r8)
    lwz r6, 0x0(r8)
    rlwinm r6, r6, 0, 14, 10
    or r0, r6, r0
    stw r0, 0x0(r8)
    b @L_800B8404
    lwz r11, 0x0(r8)
    rlwinm r0, r0, 21, 0, 10
    rlwinm r6, r6, 22, 0, 9
    rlwinm r11, r11, 0, 11, 9
    or r0, r11, r0
    stw r0, 0x0(r8)
    rlwinm r0, r7, 25, 0, 6
    lwz r7, 0x0(r8)
    rlwinm r7, r7, 0, 10, 6
    or r6, r7, r6
    stw r6, 0x0(r8)
    lwz r6, 0x0(r8)
    rlwinm r6, r6, 0, 7, 1
    or r0, r6, r0
    stw r0, 0x0(r8)
    b @L_800B8404
    lwz r12, 0x0(r9)
    rlwinm r11, r6, 1, 0, 30
    rlwinm r6, r7, 4, 0, 27
    rlwinm r7, r12, 0, 0, 30
    or r0, r7, r0
    stw r0, 0x0(r9)
    lwz r0, 0x0(r9)
    rlwinm r0, r0, 0, 31, 27
    or r0, r0, r11
    stw r0, 0x0(r9)
    lwz r0, 0x0(r9)
    rlwinm r0, r0, 0, 28, 22
    or r0, r0, r6
    stw r0, 0x0(r9)
    b @L_800B8404
    lwz r11, 0x0(r9)
    rlwinm r0, r0, 9, 0, 22
    rlwinm r6, r6, 10, 0, 21
    rlwinm r11, r11, 0, 23, 21
    or r0, r11, r0
    stw r0, 0x0(r9)
    rlwinm r0, r7, 13, 0, 18
    lwz r7, 0x0(r9)
    rlwinm r7, r7, 0, 22, 18
    or r6, r7, r6
    stw r6, 0x0(r9)
    lwz r6, 0x0(r9)
    rlwinm r6, r6, 0, 19, 13
    or r0, r6, r0
    stw r0, 0x0(r9)
    b @L_800B8404
    lwz r11, 0x0(r9)
    rlwinm r0, r0, 18, 0, 13
    rlwinm r6, r6, 19, 0, 12
    rlwinm r11, r11, 0, 14, 12
    or r0, r11, r0
    stw r0, 0x0(r9)
    rlwinm r0, r7, 22, 0, 9
    lwz r7, 0x0(r9)
    rlwinm r7, r7, 0, 13, 9
    or r6, r7, r6
    stw r6, 0x0(r9)
    lwz r6, 0x0(r9)
    rlwinm r6, r6, 0, 10, 4
    or r0, r6, r0
    stw r0, 0x0(r9)
    b @L_800B8404
    lwz r12, 0x0(r9)
    rlwinm r11, r0, 27, 0, 4
    rlwinm r0, r6, 28, 0, 3
    rlwinm r6, r12, 0, 5, 3
    or r6, r6, r11
    stw r6, 0x0(r9)
    lwz r6, 0x0(r9)
    rlwinm r6, r6, 0, 4, 0
    or r0, r6, r0
    stw r0, 0x0(r9)
    lwz r0, 0x0(r10)
    rlwinm r0, r0, 0, 0, 26
    or r0, r0, r7
    stw r0, 0x0(r10)
    b @L_800B8404
    lwz r11, 0x0(r10)
    rlwinm r0, r0, 5, 0, 26
    rlwinm r6, r6, 6, 0, 25
    rlwinm r11, r11, 0, 27, 25
    or r0, r11, r0
    stw r0, 0x0(r10)
    rlwinm r0, r7, 9, 0, 22
    lwz r7, 0x0(r10)
    rlwinm r7, r7, 0, 26, 22
    or r6, r7, r6
    stw r6, 0x0(r10)
    lwz r6, 0x0(r10)
    rlwinm r6, r6, 0, 23, 17
    or r0, r6, r0
    stw r0, 0x0(r10)
    b @L_800B8404
    lwz r11, 0x0(r10)
    rlwinm r0, r0, 14, 0, 17
    rlwinm r6, r6, 15, 0, 16
    rlwinm r11, r11, 0, 18, 16
    or r0, r11, r0
    stw r0, 0x0(r10)
    rlwinm r0, r7, 18, 0, 13
    lwz r7, 0x0(r10)
    rlwinm r7, r7, 0, 17, 13
    or r6, r7, r6
    stw r6, 0x0(r10)
    lwz r6, 0x0(r10)
    rlwinm r6, r6, 0, 14, 8
    or r0, r6, r0
    stw r0, 0x0(r10)
    b @L_800B8404
    lwz r12, 0x0(r10)
    rlwinm r11, r0, 23, 0, 8
    rlwinm r0, r6, 24, 0, 7
    rlwinm r6, r12, 0, 9, 7
    or r6, r6, r11
    stw r6, 0x0(r10)
    lwz r6, 0x0(r10)
    rlwinm r6, r6, 0, 8, 4
    or r0, r6, r0
    stw r0, 0x0(r10)
    lwz r0, 0x0(r10)
    rlwinm r0, r0, 0, 5, 31
    rlwimi r0, r7, 27, 0, 4
    stw r0, 0x0(r10)
@L_800B8404:
    addi r4, r4, 0x10
@L_800B8408:
    lwz r0, 0x0(r4)
    cmpwi r0, 0xff
    bne @L_800B80F0
    lwz r5, -29632(r13)
    rlwinm r0, r3, 0, 24, 31
    li r3, 0x1
    lwz r4, 0x4f4(r5)
    slw r0, r3, r0
    rlwinm r0, r0, 0, 24, 31
    ori r3, r4, 0x10
    stw r3, 0x4f4(r5)
    lbz r3, 0x4f3(r5)
    or r0, r3, r0
    stb r0, 0x4f3(r5)
    blr

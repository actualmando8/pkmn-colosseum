    nofralloc
    subi r0, r4, 0x9
    lwz r8, -29632(r13)
    rlwinm r4, r3, 2, 0, 29
    add r9, r8, r4
    cmplwi r0, 0x10
    addi r4, r9, 0x1c
    addi r8, r9, 0x3c
    addi r9, r9, 0x5c
    bgt @L_800B809C
    lis r10, -32719
    addi r10, r10, 11080
    rlwinm r0, r0, 2, 0, 29
    lwzx r0, r10, r0
    mtctr r0
    bctr
    lwz r0, 0x0(r4)
    rlwinm r6, r6, 1, 0, 30
    rlwinm r0, r0, 0, 0, 30
    or r0, r0, r5
    stw r0, 0x0(r4)
    clrlslwi r0, r7, 24, 4
    lwz r5, 0x0(r4)
    rlwinm r5, r5, 0, 31, 27
    or r5, r5, r6
    stw r5, 0x0(r4)
    lwz r5, 0x0(r4)
    rlwinm r5, r5, 0, 28, 22
    or r0, r5, r0
    stw r0, 0x0(r4)
    b @L_800B809C
    lwz r7, 0x0(r4)
    rlwinm r0, r6, 10, 0, 21
    cmpwi r5, 0x2
    rlwinm r6, r7, 0, 22, 18
    or r0, r6, r0
    stw r0, 0x0(r4)
    bne @L_800B7E2C
    lwz r0, 0x0(r4)
    rlwinm r0, r0, 0, 23, 21
    ori r0, r0, 0x200
    stw r0, 0x0(r4)
    lwz r0, 0x0(r4)
    rlwinm r0, r0, 0, 1, 31
    oris r0, r0, 0x8000
    stw r0, 0x0(r4)
    b @L_800B809C
@L_800B7E2C:
    lwz r6, 0x0(r4)
    rlwinm r0, r5, 9, 0, 22
    rlwinm r5, r6, 0, 23, 21
    or r0, r5, r0
    stw r0, 0x0(r4)
    lwz r0, 0x0(r4)
    rlwinm r0, r0, 0, 1, 31
    stw r0, 0x0(r4)
    b @L_800B809C
    lwz r7, 0x0(r4)
    rlwinm r5, r5, 13, 0, 18
    rlwinm r0, r6, 14, 0, 17
    rlwinm r6, r7, 0, 19, 17
    or r5, r6, r5
    stw r5, 0x0(r4)
    lwz r5, 0x0(r4)
    rlwinm r5, r5, 0, 18, 14
    or r0, r5, r0
    stw r0, 0x0(r4)
    b @L_800B809C
    lwz r7, 0x0(r4)
    rlwinm r5, r5, 17, 0, 14
    rlwinm r0, r6, 18, 0, 13
    rlwinm r6, r7, 0, 15, 13
    or r5, r6, r5
    stw r5, 0x0(r4)
    lwz r5, 0x0(r4)
    rlwinm r5, r5, 0, 14, 10
    or r0, r5, r0
    stw r0, 0x0(r4)
    b @L_800B809C
    lwz r8, 0x0(r4)
    rlwinm r0, r5, 21, 0, 10
    rlwinm r5, r8, 0, 11, 9
    or r0, r5, r0
    stw r0, 0x0(r4)
    rlwinm r5, r6, 22, 0, 9
    rlwinm r0, r7, 25, 0, 6
    lwz r6, 0x0(r4)
    rlwinm r6, r6, 0, 10, 6
    or r5, r6, r5
    stw r5, 0x0(r4)
    lwz r5, 0x0(r4)
    rlwinm r5, r5, 0, 7, 1
    or r0, r5, r0
    stw r0, 0x0(r4)
    b @L_800B809C
    lwz r0, 0x0(r8)
    rlwinm r4, r6, 1, 0, 30
    rlwinm r0, r0, 0, 0, 30
    or r0, r0, r5
    stw r0, 0x0(r8)
    clrlslwi r0, r7, 24, 4
    lwz r5, 0x0(r8)
    rlwinm r5, r5, 0, 31, 27
    or r4, r5, r4
    stw r4, 0x0(r8)
    lwz r4, 0x0(r8)
    rlwinm r4, r4, 0, 28, 22
    or r0, r4, r0
    stw r0, 0x0(r8)
    b @L_800B809C
    lwz r4, 0x0(r8)
    rlwinm r0, r5, 9, 0, 22
    rlwinm r4, r4, 0, 23, 21
    or r0, r4, r0
    stw r0, 0x0(r8)
    rlwinm r4, r6, 10, 0, 21
    clrlslwi r0, r7, 24, 13
    lwz r5, 0x0(r8)
    rlwinm r5, r5, 0, 22, 18
    or r4, r5, r4
    stw r4, 0x0(r8)
    lwz r4, 0x0(r8)
    rlwinm r4, r4, 0, 19, 13
    or r0, r4, r0
    stw r0, 0x0(r8)
    b @L_800B809C
    lwz r4, 0x0(r8)
    rlwinm r0, r5, 18, 0, 13
    rlwinm r4, r4, 0, 14, 12
    or r0, r4, r0
    stw r0, 0x0(r8)
    rlwinm r4, r6, 19, 0, 12
    clrlslwi r0, r7, 24, 22
    lwz r5, 0x0(r8)
    rlwinm r5, r5, 0, 13, 9
    or r4, r5, r4
    stw r4, 0x0(r8)
    lwz r4, 0x0(r8)
    rlwinm r4, r4, 0, 10, 4
    or r0, r4, r0
    stw r0, 0x0(r8)
    b @L_800B809C
    lwz r10, 0x0(r8)
    rlwinm r0, r5, 27, 0, 4
    rlwinm r4, r6, 28, 0, 3
    rlwinm r5, r10, 0, 5, 3
    or r0, r5, r0
    stw r0, 0x0(r8)
    rlwinm r0, r7, 0, 24, 31
    lwz r5, 0x0(r8)
    rlwinm r5, r5, 0, 4, 0
    or r4, r5, r4
    stw r4, 0x0(r8)
    lwz r4, 0x0(r9)
    rlwinm r4, r4, 0, 0, 26
    or r0, r4, r0
    stw r0, 0x0(r9)
    b @L_800B809C
    lwz r4, 0x0(r9)
    rlwinm r0, r5, 5, 0, 26
    rlwinm r4, r4, 0, 27, 25
    or r0, r4, r0
    stw r0, 0x0(r9)
    rlwinm r4, r6, 6, 0, 25
    clrlslwi r0, r7, 24, 9
    lwz r5, 0x0(r9)
    rlwinm r5, r5, 0, 26, 22
    or r4, r5, r4
    stw r4, 0x0(r9)
    lwz r4, 0x0(r9)
    rlwinm r4, r4, 0, 23, 17
    or r0, r4, r0
    stw r0, 0x0(r9)
    b @L_800B809C
    lwz r4, 0x0(r9)
    rlwinm r0, r5, 14, 0, 17
    rlwinm r4, r4, 0, 18, 16
    or r0, r4, r0
    stw r0, 0x0(r9)
    rlwinm r4, r6, 15, 0, 16
    clrlslwi r0, r7, 24, 18
    lwz r5, 0x0(r9)
    rlwinm r5, r5, 0, 17, 13
    or r4, r5, r4
    stw r4, 0x0(r9)
    lwz r4, 0x0(r9)
    rlwinm r4, r4, 0, 14, 8
    or r0, r4, r0
    stw r0, 0x0(r9)
    b @L_800B809C
    lwz r4, 0x0(r9)
    rlwinm r0, r5, 23, 0, 8
    rlwinm r4, r4, 0, 9, 7
    or r0, r4, r0
    stw r0, 0x0(r9)
    rlwinm r0, r6, 24, 0, 7
    lwz r4, 0x0(r9)
    rlwinm r4, r4, 0, 8, 4
    or r0, r4, r0
    stw r0, 0x0(r9)
    lwz r0, 0x0(r9)
    rlwinm r0, r0, 0, 5, 31
    rlwimi r0, r7, 27, 0, 4
    stw r0, 0x0(r9)
@L_800B809C:
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

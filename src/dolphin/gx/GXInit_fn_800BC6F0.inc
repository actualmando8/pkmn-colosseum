    nofralloc
    srawi r7, r3, 1
    lwz r9, -29632(r13)
    rlwinm r10, r5, 0, 24, 22
    addze r7, r7
    rlwinm r0, r3, 2, 0, 29
    add r8, r9, r0
    rlwinm r7, r7, 2, 0, 29
    stw r5, 0x49c(r8)
    addi r7, r7, 0x100
    cmplwi r10, 0x8
    addi r0, r10, 0x0
    add r7, r9, r7
    blt @L_800BC72C
    li r9, 0x0
    b @L_800BC730
@L_800BC72C:
    mr r9, r0
@L_800BC730:
    cmpwi r4, 0x8
    blt @L_800BC75C
    lwz r4, -29632(r13)
    li r0, 0x1
    slw r0, r0, r3
    lwz r8, 0x4e0(r4)
    addi r10, r4, 0x4e0
    li r4, 0x0
    andc r0, r8, r0
    stw r0, 0x0(r10)
    b @L_800BC778
@L_800BC75C:
    lwz r8, -29632(r13)
    li r0, 0x1
    slw r0, r0, r3
    addi r10, r8, 0x4e0
    lwz r8, 0x4e0(r8)
    or r0, r8, r0
    stw r0, 0x0(r10)
@L_800BC778:
    rlwinm. r0, r3, 0, 31, 31
    beq @L_800BC810
    lwz r8, 0x0(r7)
    rlwinm r3, r9, 12, 0, 19
    rlwinm r0, r4, 15, 0, 16
    rlwinm r4, r8, 0, 20, 16
    or r3, r4, r3
    stw r3, 0x0(r7)
    cmpwi r6, 0xff
    lwz r3, 0x0(r7)
    rlwinm r3, r3, 0, 17, 13
    or r0, r3, r0
    stw r0, 0x0(r7)
    bne @L_800BC7B8
    li r0, 0x7
    b @L_800BC7CC
@L_800BC7B8:
    lis r3, -32719
    rlwinm r4, r6, 2, 0, 29
    addi r0, r3, 13792
    add r3, r0, r4
    lwz r0, 0x0(r3)
@L_800BC7CC:
    lwz r3, 0x0(r7)
    rlwinm r0, r0, 19, 0, 12
    cmpwi r5, 0xff
    rlwinm r3, r3, 0, 13, 9
    or r0, r3, r0
    stw r0, 0x0(r7)
    li r4, 0x0
    beq @L_800BC7F8
    rlwinm. r0, r5, 0, 23, 23
    bne @L_800BC7F8
    li r4, 0x1
@L_800BC7F8:
    lwz r3, 0x0(r7)
    rlwinm r0, r4, 18, 0, 13
    rlwinm r3, r3, 0, 14, 12
    or r0, r3, r0
    stw r0, 0x0(r7)
    b @L_800BC898
@L_800BC810:
    lwz r3, 0x0(r7)
    rlwinm r0, r4, 3, 0, 28
    cmpwi r6, 0xff
    rlwinm r3, r3, 0, 0, 28
    or r3, r3, r9
    stw r3, 0x0(r7)
    lwz r3, 0x0(r7)
    rlwinm r3, r3, 0, 29, 25
    or r0, r3, r0
    stw r0, 0x0(r7)
    bne @L_800BC844
    li r0, 0x7
    b @L_800BC858
@L_800BC844:
    lis r3, -32719
    rlwinm r4, r6, 2, 0, 29
    addi r0, r3, 13792
    add r3, r0, r4
    lwz r0, 0x0(r3)
@L_800BC858:
    lwz r3, 0x0(r7)
    rlwinm r0, r0, 7, 0, 24
    cmpwi r5, 0xff
    rlwinm r3, r3, 0, 25, 21
    or r0, r3, r0
    stw r0, 0x0(r7)
    li r4, 0x0
    beq @L_800BC884
    rlwinm. r0, r5, 0, 23, 23
    bne @L_800BC884
    li r4, 0x1
@L_800BC884:
    lwz r3, 0x0(r7)
    rlwinm r0, r4, 6, 0, 25
    rlwinm r3, r3, 0, 26, 24
    or r0, r3, r0
    stw r0, 0x0(r7)
@L_800BC898:
    li r0, 0x61
    lwz r3, -29632(r13)
    lis r5, 0xcc01
    stb r0, -0x8000(r5)
    li r0, 0x0
    lwz r4, 0x0(r7)
    stw r4, -0x8000(r5)
    sth r0, 0x2(r3)
    lwz r0, 0x4f4(r3)
    ori r0, r0, 0x1
    stw r0, 0x4f4(r3)
    blr

    nofralloc
    lwz r7, -29632(r13)
    lis r5, -32719
    rlwinm r6, r3, 2, 0, 29
    addi r0, r5, 13832
    lwz r8, 0x1dc(r7)
    add r10, r0, r6
    lwz r5, 0x0(r10)
    rlwinm r6, r8, 0, 0, 28
    rlwinm r0, r4, 3, 0, 28
    or r4, r6, r5
    stw r4, 0x1dc(r7)
    lwz r4, 0x1dc(r7)
    rlwinm r4, r4, 0, 29, 25
    or r0, r4, r0
    stw r0, 0x1dc(r7)
    lwz r5, 0x1dc(r7)
    cmplw r8, r5
    beq @L_800BCF84
    li r0, 0x61
    lis r4, 0xcc01
    stb r0, -0x8000(r4)
    cmpwi r3, 0x2
    stw r5, -0x8000(r4)
    bne @L_800BCF5C
    li r0, 0x1
    b @L_800BCF60
@L_800BCF5C:
    li r0, 0x0
@L_800BCF60:
    lwz r5, -29632(r13)
    clrlslwi r0, r0, 24, 9
    lwz r4, 0x204(r5)
    rlwinm r4, r4, 0, 23, 21
    or r0, r4, r0
    stw r0, 0x204(r5)
    lwz r0, 0x4f4(r5)
    ori r0, r0, 0x4
    stw r0, 0x4f4(r5)
@L_800BCF84:
    lwz r0, 0x0(r10)
    cmplwi r0, 0x4
    bne @L_800BCFCC
    lwz r5, -29632(r13)
    subi r3, r3, 0x4
    li r0, 0x61
    lwz r4, 0x1d4(r5)
    rlwinm r4, r4, 0, 23, 20
    rlwimi r4, r3, 9, 21, 22
    stw r4, 0x1d4(r5)
    lis r3, 0xcc01
    lwz r4, 0x1d4(r5)
    rlwinm r4, r4, 0, 8, 31
    oris r4, r4, 0x4200
    stw r4, 0x1d4(r5)
    stb r0, -0x8000(r3)
    lwz r0, 0x1d4(r5)
    stw r0, -0x8000(r3)
@L_800BCFCC:
    lwz r3, -29632(r13)
    li r0, 0x0
    sth r0, 0x2(r3)
    blr

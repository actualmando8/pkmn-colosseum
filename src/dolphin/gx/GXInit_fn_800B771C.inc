    nofralloc
    lwz r5, -29632(r13)
    lwz r4, 0x14(r5)
    rlwinm. r0, r4, 19, 30, 31
    beq @L_800B7734
    li r3, 0x1
    b @L_800B7738
@L_800B7734:
    li r3, 0x0
@L_800B7738:
    rlwinm. r0, r4, 17, 30, 31
    beq @L_800B7748
    li r4, 0x1
    b @L_800B774C
@L_800B7748:
    li r4, 0x0
@L_800B774C:
    lbz r0, 0x41d(r5)
    add r7, r3, r4
    cmplwi r0, 0x0
    beq @L_800B7764
    li r4, 0x2
    b @L_800B777C
@L_800B7764:
    lbz r0, 0x41c(r5)
    cmplwi r0, 0x0
    beq @L_800B7778
    li r4, 0x1
    b @L_800B777C
@L_800B7778:
    li r4, 0x0
@L_800B777C:
    lwz r6, 0x18(r5)
    rlwinm. r0, r6, 0, 30, 31
    beq @L_800B7790
    li r3, 0x1
    b @L_800B7794
@L_800B7790:
    li r3, 0x0
@L_800B7794:
    rlwinm. r0, r6, 30, 30, 31
    beq @L_800B77A4
    li r5, 0x1
    b @L_800B77A8
@L_800B77A4:
    li r5, 0x0
@L_800B77A8:
    rlwinm. r0, r6, 28, 30, 31
    add r8, r3, r5
    beq @L_800B77BC
    li r3, 0x1
    b @L_800B77C0
@L_800B77BC:
    li r3, 0x0
@L_800B77C0:
    rlwinm. r0, r6, 26, 30, 31
    add r8, r8, r3
    beq @L_800B77D4
    li r3, 0x1
    b @L_800B77D8
@L_800B77D4:
    li r3, 0x0
@L_800B77D8:
    rlwinm. r0, r6, 24, 30, 31
    add r8, r8, r3
    beq @L_800B77EC
    li r3, 0x1
    b @L_800B77F0
@L_800B77EC:
    li r3, 0x0
@L_800B77F0:
    rlwinm. r0, r6, 22, 30, 31
    add r8, r8, r3
    beq @L_800B7804
    li r3, 0x1
    b @L_800B7808
@L_800B7804:
    li r3, 0x0
@L_800B7808:
    rlwinm. r0, r6, 20, 30, 31
    add r8, r8, r3
    beq @L_800B781C
    li r3, 0x1
    b @L_800B7820
@L_800B781C:
    li r3, 0x0
@L_800B7820:
    rlwinm. r0, r6, 18, 30, 31
    add r8, r8, r3
    beq @L_800B7834
    li r6, 0x1
    b @L_800B7838
@L_800B7834:
    li r6, 0x0
@L_800B7838:
    li r0, 0x10
    lwz r3, -29632(r13)
    lis r5, 0xcc01
    add r8, r8, r6
    stb r0, -0x8000(r5)
    rlwinm r0, r4, 2, 0, 29
    li r4, 0x1008
    stw r4, -0x8000(r5)
    rlwinm r4, r8, 4, 0, 27
    or r0, r7, r0
    or r0, r4, r0
    stw r0, -0x8000(r5)
    li r0, 0x1
    sth r0, 0x2(r3)
    blr

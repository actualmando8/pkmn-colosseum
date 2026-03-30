    nofralloc
    lwz r10, -29632(r13)
    li r12, 0x0
    li r11, 0x0
    lis r7, 0xcc01
    b @L_800B84C4
@L_800B8458:
    rlwinm r9, r12, 0, 24, 31
    lbz r3, 0x4f3(r10)
    li r0, 0x1
    slw r0, r0, r9
    and. r0, r3, r0
    beq @L_800B84BC
    li r8, 0x8
    stb r8, -0x8000(r7)
    ori r3, r9, 0x70
    addi r0, r11, 0x1c
    stb r3, -0x8000(r7)
    ori r5, r9, 0x80
    addi r4, r11, 0x3c
    lwzx r6, r10, r0
    ori r3, r9, 0x90
    addi r0, r11, 0x5c
    stw r6, -0x8000(r7)
    stb r8, -0x8000(r7)
    stb r5, -0x8000(r7)
    lwzx r4, r10, r4
    stw r4, -0x8000(r7)
    stb r8, -0x8000(r7)
    stb r3, -0x8000(r7)
    lwzx r0, r10, r0
    stw r0, -0x8000(r7)
@L_800B84BC:
    addi r11, r11, 0x4
    addi r12, r12, 0x1
@L_800B84C4:
    rlwinm r0, r12, 0, 24, 31
    cmplwi r0, 0x8
    blt @L_800B8458
    lwz r3, -29632(r13)
    li r0, 0x0
    stb r0, 0x4f3(r3)
    blr

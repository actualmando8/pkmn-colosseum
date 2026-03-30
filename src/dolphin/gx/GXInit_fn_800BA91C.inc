    nofralloc
    cmplwi r3, 0x3c
    bgt @L_800BA960
    lis r9, -32719
    addi r9, r9, 13404
    rlwinm r0, r3, 2, 0, 29
    lwzx r0, r9, r0
    mtctr r0
    bctr
    li r11, 0x3
    li r12, 0x3
    b @L_800BA968
    li r11, 0x3
    li r12, 0x2
    b @L_800BA968
    li r11, 0x2
    li r12, 0x2
    b @L_800BA968
@L_800BA960:
    li r12, 0x0
    li r11, 0x0
@L_800BA968:
    rlwinm. r0, r4, 0, 16, 31
    bne @L_800BA974
    li r4, 0x1
@L_800BA974:
    rlwinm. r0, r5, 0, 16, 31
    bne @L_800BA980
    li r5, 0x1
@L_800BA980:
    li r10, 0x1
    slw r9, r10, r11
    rlwinm r4, r4, 0, 16, 31
    subi r0, r9, 0x1
    add r0, r4, r0
    sraw r0, r0, r11
    slw r4, r10, r12
    stw r0, 0x0(r6)
    rlwinm r5, r5, 0, 16, 31
    subi r0, r4, 0x1
    add r0, r5, r0
    sraw r0, r0, r12
    cmpwi r3, 0x6
    stw r0, 0x0(r7)
    beq @L_800BA9C8
    cmpwi r3, 0x16
    beq @L_800BA9C8
    li r10, 0x0
@L_800BA9C8:
    cmpwi r10, 0x0
    beq @L_800BA9D8
    li r0, 0x2
    b @L_800BA9DC
@L_800BA9D8:
    li r0, 0x1
@L_800BA9DC:
    stw r0, 0x0(r8)
    blr

    nofralloc
    lis r5, 0x9249
    addi r0, r5, 0x2493
    addi r7, r3, 0x6
    mulhw r6, r0, r7
    lis r5, 0xb38d
    subi r0, r5, 0x64f
    mulhw r0, r0, r3
    add r5, r6, r7
    srawi r5, r5, 2
    rlwinm r6, r5, 1, 31, 31
    add r5, r5, r6
    add r0, r0, r3
    mulli r6, r5, 0x7
    srawi r0, r0, 8
    rlwinm r5, r0, 1, 31, 31
    add r5, r0, r5
    subf r0, r6, r7
    mulli r11, r5, 0x16d
    stw r0, 0x18(r4)
    b @L_800A284C
@L_800A284C:
    lis r6, 0x51ec
    subi r10, r6, 0x7ae1
    b @L_800A2858
@L_800A2858:
    b @L_800A2864
@L_800A285C:
    subi r11, r11, 0x16d
    subi r5, r5, 0x1
@L_800A2864:
    cmpwi r5, 0x1
    bge @L_800A2874
    li r0, 0x0
    b @L_800A28A8
@L_800A2874:
    subi r0, r5, 0x1
    mulhw r0, r10, r0
    srawi r8, r0, 7
    srawi r6, r0, 5
    addi r0, r5, 0x3
    rlwinm r7, r6, 1, 31, 31
    srawi r0, r0, 2
    rlwinm r9, r8, 1, 31, 31
    add r6, r6, r7
    addze r0, r0
    add r7, r8, r9
    subf r0, r6, r0
    add r0, r7, r0
@L_800A28A8:
    add r0, r11, r0
    cmpw r3, r0
    blt @L_800A285C
    srawi r6, r5, 2
    stw r5, 0x14(r4)
    addze r6, r6
    rlwinm r6, r6, 2, 0, 29
    subfc r6, r6, r5
    subf r0, r0, r3
    cmpwi r6, 0x0
    stw r0, 0x1c(r4)
    li r7, 0x1
    li r8, 0x0
    bne @L_800A290C
    lis r3, 0x51ec
    subi r3, r3, 0x7ae1
    mulhw r3, r3, r5
    srawi r3, r3, 5
    rlwinm r6, r3, 1, 31, 31
    add r3, r3, r6
    mulli r3, r3, 0x64
    subf r3, r3, r5
    cmpwi r3, 0x0
    beq @L_800A290C
    mr r8, r7
@L_800A290C:
    cmpwi r8, 0x0
    bne @L_800A2940
    lis r3, 0x51ec
    subi r3, r3, 0x7ae1
    mulhw r3, r3, r5
    srawi r3, r3, 7
    rlwinm r6, r3, 1, 31, 31
    add r3, r3, r6
    mulli r3, r3, 0x190
    subf r3, r3, r5
    cmpwi r3, 0x0
    beq @L_800A2940
    li r7, 0x0
@L_800A2940:
    cmpwi r7, 0x0
    beq @L_800A2954
    lis r3, -32719
    addi r6, r3, 6312
    b @L_800A295C
@L_800A2954:
    lis r3, -32719
    addi r6, r3, 6264
@L_800A295C:
    li r7, 0xc
    li r3, 0x30
    b @L_800A2968
@L_800A2968:
    b @L_800A296C
@L_800A296C:
    subi r3, r3, 0x4
    lwzx r5, r6, r3
    subi r7, r7, 0x1
    cmpw r0, r5
    blt @L_800A296C
    stw r7, 0x10(r4)
    lwzx r3, r6, r3
    subf r3, r3, r0
    addi r0, r3, 0x1
    stw r0, 0xc(r4)
    blr

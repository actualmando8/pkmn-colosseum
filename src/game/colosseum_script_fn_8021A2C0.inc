    nofralloc
    rlwinm. r0, r3, 0, 16, 31
    bne @L_8021A2D0
    li r3, 0x1
    blr
@L_8021A2D0:
    cmplwi r0, 0x165
    bne @L_8021A2E0
    li r3, 0x1
    blr
@L_8021A2E0:
    cmplwi r0, 0x163
    bne @L_8021A2F0
    li r3, 0x1
    blr
@L_8021A2F0:
    lis r4, lbl_80279FA0@ha
    li r3, 0x0
    addi r4, r4, lbl_80279FA0@l
    b @L_8021A30C
@L_8021A300:
    cmplw r0, r5
    beq @L_8021A318
    addi r3, r3, 0x2
@L_8021A30C:
    lhzx r5, r4, r3
    cmplwi r5, 0xfffe
    bne @L_8021A300
@L_8021A318:
    lis r3, 0x1
    subi r0, r3, 0x2
    rlwinm r0, r0, 0, 16, 31
    subf r3, r5, r0
    subic r0, r3, 0x1
    subfe r0, r0, r3
    rlwinm r3, r0, 0, 24, 31
    blr

    nofralloc
    rlwinm. r0, r3, 0, 16, 31
    bne @L_80219FF4
    li r3, 0x0
    blr
@L_80219FF4:
    cmplwi r0, 0x165
    bne @L_8021A004
    li r3, 0x0
    blr
@L_8021A004:
    cmplwi r0, 0x163
    bne @L_8021A014
    li r3, 0x0
    blr
@L_8021A014:
    lis r4, lbl_80279FA0@ha
    li r3, 0x0
    addi r4, r4, lbl_80279FA0@l
@L_8021A020:
    lhzx r5, r4, r3
    cmplwi r5, 0xffff
    beq @L_8021A03C
    cmplw r0, r5
    beq @L_8021A03C
    addi r3, r3, 0x2
    b @L_8021A020
@L_8021A03C:
    cmplwi r5, 0xffff
    beq @L_8021A04C
    li r3, 0x0
    blr
@L_8021A04C:
    li r3, 0x1
    blr

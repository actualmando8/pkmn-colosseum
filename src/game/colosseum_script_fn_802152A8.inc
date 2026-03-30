    nofralloc
    lis r4, lbl_80279F88@ha
    rlwinm r5, r3, 0, 16, 31
    addi r3, r4, lbl_80279F88@l
    li r4, 0x0
    b @L_802152C8
@L_802152BC:
    cmplw r0, r5
    bgt @L_802152D8
    addi r4, r4, 0x2
@L_802152C8:
    clrlslwi r0, r4, 16, 1
    lhzx r0, r3, r0
    cmplwi r0, 0xffff
    bne @L_802152BC
@L_802152D8:
    cmplwi r0, 0xffff
    beq @L_802152F8
    lis r3, lbl_80279F88@ha
    clrlslwi r0, r4, 16, 1
    addi r3, r3, lbl_80279F88@l
    add r3, r3, r0
    lhz r3, 0x2(r3)
    blr
@L_802152F8:
    li r3, 0x78
    blr

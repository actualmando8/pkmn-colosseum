    nofralloc
    rlwinm r5, r3, 0, 16, 31
    rlwinm r0, r4, 0, 16, 31
    mulli r3, r5, 0x30
    divw r0, r3, r0
    rlwinm. r4, r0, 0, 24, 31
    bne @L_80218B90
    cmplwi r5, 0x0
    beq @L_80218B90
    li r4, 0x1
@L_80218B90:
    lis r3, lbl_80279F7C@ha
    li r0, 0x6
    addi r3, r3, lbl_80279F7C@l
    rlwinm r4, r4, 0, 24, 31
    li r5, 0x0
    mtctr r0
@L_80218BA8:
    lbz r0, 0x0(r3)
    cmplw r4, r0
    ble @L_80218BC0
    addi r5, r5, 0x2
    addi r3, r3, 0x2
    bdnz @L_80218BA8
@L_80218BC0:
    lis r3, lbl_80279F7C@ha
    addi r0, r3, lbl_80279F7C@l
    add r3, r0, r5
    lbz r3, 0x1(r3)
    blr

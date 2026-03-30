    nofralloc
    lwz r0, lbl_80478B90(r13)
    cmplw r3, r0
    bgt @L_80135F6C
    cmplwi r4, 0x8
    ble @L_80135F74
    @L_80135F6C:
    li r3, 0x0
    blr
    @L_80135F74:
    lis r5, lbl_80363B78@ha
    slwi r6, r3, 4
    addi r3, r5, lbl_80363B78@l
    slwi r0, r4, 1
    add r3, r3, r6
    lhzx r3, r3, r0
    blr

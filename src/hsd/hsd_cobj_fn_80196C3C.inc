    nofralloc
    cmplwi r3, 0x0
    bne @L_80196C4C
    lis r3, fn_80196C54@ha
    addi r3, r3, fn_80196C54@l
@L_80196C4C:
    stw r3, lbl_80478C58(r13)
    blr

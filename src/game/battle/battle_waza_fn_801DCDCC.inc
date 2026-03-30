    nofralloc
    cmplwi r3, 0x0
    bne @L_801DCDDC
    li r3, 0x0
    blr
@L_801DCDDC:
    lbz r0, 0x77(r3)
    cmplwi r0, 0x0
    bne @L_801DCDF0
    li r3, 0x0
    blr
@L_801DCDF0:
    lbz r0, 0x4e(r3)
    cmplwi r0, 0x0
    beq @L_801DCE04
    lbz r3, 0x4f(r3)
    blr
@L_801DCE04:
    li r3, 0x0
    blr

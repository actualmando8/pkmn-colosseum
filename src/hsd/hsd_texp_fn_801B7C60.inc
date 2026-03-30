    nofralloc
    cmplwi r3, 0x0
    bne @L_801B7C70
    li r3, 0x0
    blr
@L_801B7C70:
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    bne @L_801B7C84
    li r3, 0x2
    blr
@L_801B7C84:
    addis r0, r3, 0x1
    cmplwi r0, 0xfffe
    bne @L_801B7C98
    li r3, 0x3
    blr
@L_801B7C98:
    lwz r3, 0x0(r3)
    blr

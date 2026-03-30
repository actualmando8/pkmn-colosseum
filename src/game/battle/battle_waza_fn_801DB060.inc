    nofralloc
    lwz r3, -21520(r13)
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    bne @L_801DB078
    li r0, 0x0
    stw r0, -21520(r13)
@L_801DB078:
    lwz r3, -21520(r13)
    addi r3, r3, 0x1
    stw r3, -21520(r13)
    blr

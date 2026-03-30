    nofralloc
    cmplwi r3, 0x0
    beqlr
    cmplwi r4, 0x0
    bne @L_80135CA4
    blr
@L_80135CA4:
    lfd f1, 0x0(r4)
    lfd f0, 0x8(r4)
    stfd f1, 0x8(r3)
    stfd f0, 0x10(r3)
    lfd f1, 0x10(r4)
    lfd f0, 0x18(r4)
    stfd f1, 0x18(r3)
    stfd f0, 0x20(r3)
    lfd f0, 0x20(r4)
    stfd f0, 0x28(r3)
    blr

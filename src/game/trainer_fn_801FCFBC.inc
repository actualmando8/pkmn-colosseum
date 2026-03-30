    nofralloc
    cmplwi r3, 0x0
    bne @L_801FCFCC
    li r3, 0x0
    blr
    @L_801FCFCC:
    lhz r3, 0x676(r3)
    blr

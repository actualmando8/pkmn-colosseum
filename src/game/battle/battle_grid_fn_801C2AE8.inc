    nofralloc
    lis r4, lbl_80466DE8@ha
    li r5, 0x0
    addi r0, r4, lbl_80466DE8@l
    mr r4, r0
    b @L_801C2B18
    @L_801C2AFC:
    lwz r0, 0x0(r4)
    cmplw r0, r3
    bne @L_801C2B10
    lhz r3, 0xc(r4)
    blr
    @L_801C2B10:
    addi r5, r5, 0x1
    addi r4, r4, 0x10
    @L_801C2B18:
    clrlwi r0, r5, 16
    cmplwi r0, 0x4
    blt @L_801C2AFC
    li r3, 0x0
    blr

    nofralloc
    lwz r0, lbl_8047AB78(r13)
    cmplw r3, r0
    blt @L_800E3B1C
    li r3, 0x0
    blr
    @L_800E3B1C:
    mulli r0, r3, 0x170
    lwz r3, lbl_8047AB74(r13)
    add r3, r3, r0
    lwz r0, 0x0(r3)
    clrlwi. r0, r0, 31
    bnelr
    li r3, 0x0
    blr

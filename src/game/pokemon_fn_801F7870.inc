    nofralloc
    lwz r4, lbl_80478F38(r13)
    clrlwi r3, r3, 16
    lwz r0, 0x0(r4)
    cmplw r3, r0
    blt @L_801F788C
    li r3, 0x0
    blr
    @L_801F788C:
    mulli r0, r3, 0x14
    lwz r3, lbl_80478F3C(r13)
    add r3, r3, r0
    blr

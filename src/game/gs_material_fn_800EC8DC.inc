    nofralloc
    li r6, 0x0
    li r5, 0x0
    b @L_800EC908
    @L_800EC8E8:
    lwz r4, lbl_8047AB74(r13)
    lwzx r3, r4, r5
    clrlwi. r0, r3, 31
    beq @L_800EC900
    rlwinm r0, r3, 0, 24, 22
    stwx r0, r4, r5
    @L_800EC900:
    addi r5, r5, 0x170
    addi r6, r6, 0x1
    @L_800EC908:
    lwz r0, lbl_8047AB78(r13)
    cmplw r6, r0
    blt @L_800EC8E8
    blr

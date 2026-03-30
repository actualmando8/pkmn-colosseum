    nofralloc
    li r6, 0x0
    li r5, 0x0
    b @L_800EC944
    @L_800EC924:
    lwz r4, lbl_8047AB74(r13)
    lwzx r3, r4, r5
    clrlwi. r0, r3, 31
    beq @L_800EC93C
    ori r0, r3, 0x100
    stwx r0, r4, r5
    @L_800EC93C:
    addi r5, r5, 0x170
    addi r6, r6, 0x1
    @L_800EC944:
    lwz r0, lbl_8047AB78(r13)
    cmplw r6, r0
    blt @L_800EC924
    blr

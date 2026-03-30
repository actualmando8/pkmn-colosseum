    nofralloc
    li r4, 0x0
    li r5, 0x0
    b @L_801BF15C
    @L_801BF144:
    li r0, 0x1
    slw r0, r0, r5
    and. r0, r3, r0
    beq @L_801BF158
    addi r4, r4, 0x1
    @L_801BF158:
    addi r5, r5, 0x1
    @L_801BF15C:
    cmpwi r5, 0x20
    blt @L_801BF144
    mr r3, r4
    blr

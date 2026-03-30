    nofralloc
    cmplwi r3, 0x0
    beqlr
    li r7, 0x0
    b @L_801F6534
    @L_801F651C:
    clrlslwi r6, r7, 16, 2
    addi r7, r7, 0x1
    addis r5, r6, 0x1
    lwzx r0, r4, r6
    subi r5, r5, 0x5b3c
    stwx r0, r3, r5
    @L_801F6534:
    clrlwi r0, r7, 16
    cmplwi r0, 0x8
    blt @L_801F651C
    blr

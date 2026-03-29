    nofralloc
    lis r4, lbl_803A9768@ha
    li r0, 0x27
    addi r6, r4, lbl_803A9768@l
    subi r4, r3, 0x4
    lwz r3, 0x278(r6)
    addi r3, r3, 0x1
    srwi r5, r3, 31
    clrlwi r3, r3, 31
    xor r3, r3, r5
    subf r3, r5, r3
    mulli r3, r3, 0x138
    add r3, r6, r3
    addi r5, r3, 0x4
    mtctr r0
    @L_80057490:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_80057490
    blr

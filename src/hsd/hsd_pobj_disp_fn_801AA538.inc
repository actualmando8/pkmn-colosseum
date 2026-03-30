    nofralloc
    lis r8, lbl_8036CBF0@ha
    lis r6, lbl_8036CBF0@ha
    lis r5, lbl_8036CBF0@ha
    lis r7, lbl_8036CBF0@ha
    addi r8, r8, lbl_8036CBF0@l
    addi r6, r6, lbl_8036CBF0@l
    addi r5, r5, lbl_8036CBF0@l
    stw r4, 0x4(r8)
    stw r4, lbl_8036CBF0@l(r7)
    stw r3, 0xc(r6)
    stw r3, 0x8(r5)
    blr

    nofralloc
    lis r3, lbl_803F7A30@ha
    li r0, 0x1
    addi r3, r3, lbl_803F7A30@l
    stb r0, 0x342(r3)
    blr

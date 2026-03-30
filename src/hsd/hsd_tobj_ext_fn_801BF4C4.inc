    nofralloc
    lis r5, lbl_80466BC0@ha
    lis r4, lbl_80466BC0@ha
    addi r5, r5, lbl_80466BC0@l
    li r0, 0x1
    addi r4, r4, lbl_80466BC0@l
    stw r3, 0x3c(r5)
    stb r0, 0x54(r4)
    blr

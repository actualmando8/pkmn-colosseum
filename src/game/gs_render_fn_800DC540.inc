    nofralloc
    li r0, 0x0
    lis r3, lbl_80400EE0@ha
    stbu r0, lbl_80400EE0@l(r3)
    stb r0, lbl_8047AAE0(r13)
    stb r0, 0x14(r3)
    stb r0, 0x28(r3)
    stb r0, 0x3c(r3)
    blr

    nofralloc
    lis r3, lbl_803A9F08@ha
    li r0, 0x0
    addi r4, r3, lbl_803A9F08@l
    stw r0, 0x2c(r4)
    addis r3, r4, 0x1
    stb r0, -0x327c(r3)
    li r0, 0x2
    mtctr r0
@L_80068FA4:
    addi r9, r4, 0x30
    li r0, 0x0
    stb r0, 0x0(r9)
    addi r8, r9, 0xc
    addi r7, r9, 0x18
    addi r6, r9, 0x24
    stw r0, 0x4(r9)
    addi r5, r9, 0x30
    addi r3, r9, 0x3c
    addi r4, r4, 0x48
    stb r0, 0x0(r8)
    addi r9, r4, 0x30
    addi r4, r4, 0x48
    stw r0, 0x4(r8)
    addi r8, r9, 0xc
    stb r0, 0x0(r7)
    stw r0, 0x4(r7)
    addi r7, r9, 0x18
    stb r0, 0x0(r6)
    stw r0, 0x4(r6)
    addi r6, r9, 0x24
    stb r0, 0x0(r5)
    stw r0, 0x4(r5)
    addi r5, r9, 0x30
    stb r0, 0x0(r3)
    stw r0, 0x4(r3)
    addi r3, r9, 0x3c
    stb r0, 0x0(r9)
    stw r0, 0x4(r9)
    stb r0, 0x0(r8)
    stw r0, 0x4(r8)
    stb r0, 0x0(r7)
    stw r0, 0x4(r7)
    stb r0, 0x0(r6)
    stw r0, 0x4(r6)
    stb r0, 0x0(r5)
    stw r0, 0x4(r5)
    stb r0, 0x0(r3)
    stw r0, 0x4(r3)
    bdnz @L_80068FA4
    blr

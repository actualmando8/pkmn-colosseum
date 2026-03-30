    nofralloc
    lis r6, lbl_80401C10@ha
    stwu r1, -0x20(r1)
    addi r8, r6, lbl_80401C10@l
    li r7, 0x0
    lwz r0, 0x0(r8)
    cmpw r0, r3
    bne @L_800F7D58
    b @L_800F7D8C
    @L_800F7D58:
    lwzu r0, 0x6c(r8)
    cmpw r0, r3
    bne @L_800F7D68
    b @L_800F7D8C
    @L_800F7D68:
    lwzu r0, 0x6c(r8)
    cmpw r0, r3
    bne @L_800F7D78
    b @L_800F7D8C
    @L_800F7D78:
    lwzu r0, 0x6c(r8)
    cmpw r0, r3
    bne @L_800F7D88
    b @L_800F7D8C
    @L_800F7D88:
    mr r8, r7
    @L_800F7D8C:
    cmplwi r8, 0x0
    beq @L_800F7DDC
    extsb r0, r4
    lis r3, 0x4330
    xoris r6, r0, 0x8000
    extsb r0, r5
    stw r6, 0xc(r1)
    xoris r0, r0, 0x8000
    lfd f2, lbl_8047CCC8(r2)
    stw r3, 0x8(r1)
    lfd f0, 0x8(r1)
    stb r4, 0x34(r8)
    fsubs f1, f0, f2
    stb r5, 0x35(r8)
    stw r0, 0x14(r1)
    stw r3, 0x10(r1)
    lfd f0, 0x10(r1)
    stfs f1, 0x48(r8)
    fsubs f0, f0, f2
    stfs f0, 0x4c(r8)
    @L_800F7DDC:
    addi r1, r1, 0x20
    blr

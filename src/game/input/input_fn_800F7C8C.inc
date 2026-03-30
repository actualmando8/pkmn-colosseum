    nofralloc
    lis r6, lbl_80401C10@ha
    stwu r1, -0x20(r1)
    addi r8, r6, lbl_80401C10@l
    li r7, 0x0
    lwz r0, 0x0(r8)
    cmpw r0, r3
    bne @L_800F7CAC
    b @L_800F7CE0
    @L_800F7CAC:
    lwzu r0, 0x6c(r8)
    cmpw r0, r3
    bne @L_800F7CBC
    b @L_800F7CE0
    @L_800F7CBC:
    lwzu r0, 0x6c(r8)
    cmpw r0, r3
    bne @L_800F7CCC
    b @L_800F7CE0
    @L_800F7CCC:
    lwzu r0, 0x6c(r8)
    cmpw r0, r3
    bne @L_800F7CDC
    b @L_800F7CE0
    @L_800F7CDC:
    mr r8, r7
    @L_800F7CE0:
    cmplwi r8, 0x0
    beq @L_800F7D30
    extsb r0, r4
    lis r3, 0x4330
    xoris r6, r0, 0x8000
    extsb r0, r5
    stw r6, 0xc(r1)
    xoris r0, r0, 0x8000
    lfd f2, lbl_8047CCC8(r2)
    stw r3, 0x8(r1)
    lfd f0, 0x8(r1)
    stb r4, 0x36(r8)
    fsubs f1, f0, f2
    stb r5, 0x37(r8)
    stw r0, 0x14(r1)
    stw r3, 0x10(r1)
    lfd f0, 0x10(r1)
    stfs f1, 0x50(r8)
    fsubs f0, f0, f2
    stfs f0, 0x54(r8)
    @L_800F7D30:
    addi r1, r1, 0x20
    blr

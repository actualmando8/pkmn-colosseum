    nofralloc
    stwu r1, -0x10(r1)
    lis r3, lbl_802E61E8@ha
    li r6, 0x0
    addi r5, r3, lbl_802E61E8@l
    li r0, 0x11
    mtctr r0
    @L_80054438:
    lha r3, 0x6(r4)
    lwz r0, 0x0(r5)
    cmpw r3, r0
    beq @L_80054454
    addi r5, r5, 0x8
    addi r6, r6, 0x1
    bdnz @L_80054438
    @L_80054454:
    cmpwi r6, 0x11
    blt @L_80054464
    li r3, 0x0
    b @L_800544A0
    @L_80054464:
    lfs f1, lbl_8047BE80(r2)
    lis r3, lbl_802E61E8@ha
    lfs f0, lbl_8047A558(r13)
    addi r5, r3, lbl_802E61E8@l
    slwi r0, r6, 3
    li r3, 0x0
    fmuls f0, f1, f0
    add r5, r5, r0
    lha r0, 0x4(r5)
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r5, 0xc(r1)
    add r0, r5, r0
    extsh r0, r0
    sth r0, 0x52(r4)
    @L_800544A0:
    addi r1, r1, 0x10
    blr

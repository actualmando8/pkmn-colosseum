    nofralloc
    lwz r0, lbl_8047ABC0(r13)
    lwz r3, lbl_8047ABBC(r13)
    mtctr r0
    cmplwi r0, 0x0
    ble @L_800EE860
    @L_800EE848:
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    bne @L_800EE858
    b @L_800EE864
    @L_800EE858:
    addi r3, r3, 0xc
    bdnz @L_800EE848
    @L_800EE860:
    li r3, 0x0
    @L_800EE864:
    cmplwi r3, 0x0
    bne @L_800EE874
    li r3, 0x0
    blr
    @L_800EE874:
    li r0, 0x1
    stb r0, 0x0(r3)
    blr

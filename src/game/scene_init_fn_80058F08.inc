    nofralloc
    cmpwi r4, 0x0
    blt @L_80058F18
    cmpwi r4, 0x7
    blt @L_80058F20
    @L_80058F18:
    li r3, 0x2
    blr
    @L_80058F20:
    lis r5, lbl_802677D0@ha
    slwi r4, r4, 4
    addi r0, r5, lbl_802677D0@l
    add r4, r0, r4
    lwz r0, 0x8(r4)
    stw r0, 0x0(r3)
    lwz r3, 0x4(r4)
    blr

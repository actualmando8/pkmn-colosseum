    nofralloc
    cmpwi r4, 0x0
    blt @L_800551A4
    cmpwi r4, 0x20
    blt @L_800551AC
    @L_800551A4:
    li r3, 0x3
    blr
    @L_800551AC:
    mulli r5, r4, 0xc
    lis r4, lbl_80267398@ha
    addi r0, r4, lbl_80267398@l
    add r4, r0, r5
    lwz r0, 0x8(r4)
    stw r0, 0x0(r3)
    lwz r3, 0x4(r4)
    blr

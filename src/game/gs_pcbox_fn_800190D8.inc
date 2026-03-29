    nofralloc
    lis r5, lbl_803A1B90@ha
    addi r5, r5, lbl_803A1B90@l
    lwz r0, 0x40(r5)
    cmpwi r0, 0x8
    bgelr
    clrlwi r3, r3, 16
    slwi r0, r0, 3
    sthx r3, r5, r0
    lwz r0, 0x40(r5)
    slwi r0, r0, 3
    add r3, r5, r0
    stw r4, 0x4(r3)
    lwz r3, 0x40(r5)
    addi r0, r3, 0x1
    stw r0, 0x40(r5)
    blr

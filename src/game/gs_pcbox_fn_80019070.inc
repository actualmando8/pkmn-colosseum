    nofralloc
    lis r4, lbl_803A1B90@ha
    li r7, -0x1
    addi r4, r4, lbl_803A1B90@l
    li r6, 0x0
    lwz r0, 0x40(r4)
    mr r5, r4
    clrlwi r3, r3, 16
    mtctr r0
    cmpwi r0, 0x0
    ble @L_800190C8
    @L_80019098:
    lhz r0, 0x0(r5)
    cmplw r3, r0
    bne @L_800190BC
    lis r3, lbl_803A1B90@ha
    slwi r0, r6, 3
    addi r3, r3, lbl_803A1B90@l
    add r3, r3, r0
    lwz r7, 0x4(r3)
    b @L_800190C8
    @L_800190BC:
    addi r5, r5, 0x8
    addi r6, r6, 0x1
    bdnz @L_80019098
    @L_800190C8:
    li r0, 0x0
    mr r3, r7
    stw r0, 0x40(r4)
    blr

    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lis r5, lbl_803A1B90@ha
    addi r5, r5, lbl_803A1B90@l
    lwz r0, 0x40(r5)
    mtctr r0
    cmpwi r0, 0x0
    ble @L_80021AE4
    @L_80021AC0:
    lhz r0, 0x0(r5)
    cmplw r3, r0
    bne @L_80021ADC
    li r0, 0x0
    li r3, 0x2
    stw r0, 0x0(r4)
    b @L_80021B04
    @L_80021ADC:
    addi r5, r5, 0x8
    bdnz @L_80021AC0
    @L_80021AE4:
    li r3, 0x2
    li r4, 0x426a
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r3, 0x1
    @L_80021B04:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

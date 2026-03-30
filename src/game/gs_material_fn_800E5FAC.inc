    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lhz r0, 0x150(r3)
    cmplwi r0, 0x0
    bne @L_800E5FCC
    li r3, 0x0
    b @L_800E5FEC
    @L_800E5FCC:
    lwz r3, 0x14c(r3)
    lwz r3, 0x0(r3)
    cmplwi r3, 0x0
    bne @L_800E5FE4
    li r3, 0x0
    b @L_800E5FEC
    @L_800E5FE4:
    bl fn_800DF240
    extrwi r3, r3, 1, 29
    @L_800E5FEC:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

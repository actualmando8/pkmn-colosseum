    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lhz r0, 0x150(r3)
    cmplwi r0, 0x0
    bne @L_800E5998
    li r3, 0x0
    b @L_800E59B8
    @L_800E5998:
    lwz r3, 0x14c(r3)
    lwz r3, 0x0(r3)
    cmplwi r3, 0x0
    bne @L_800E59B0
    li r3, 0x0
    b @L_800E59B8
    @L_800E59B0:
    bl fn_800DF240
    clrlwi r3, r3, 31
    @L_800E59B8:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

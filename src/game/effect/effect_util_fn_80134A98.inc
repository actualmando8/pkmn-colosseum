    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    bne @L_80134AC0
    li r3, 0x0
    li r4, 0x3
    bl fn_80129280
    @L_80134AC0:
    extsb. r0, r31
    blt @L_80134AD4
    extsb r0, r31
    cmpwi r0, 0x3
    blt @L_80134ADC
    @L_80134AD4:
    li r3, 0x0
    b @L_80134AE4
    @L_80134ADC:
    mulli r0, r0, 0x24a4
    add r3, r3, r0
    @L_80134AE4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

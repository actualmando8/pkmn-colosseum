    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    bne @L_80135388
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    cmplwi r3, 0x0
    bne @L_80135370
    li r3, 0x0
    b @L_80135398
    @L_80135370:
    li r4, 0x1
    bl fn_80129280
    cmplwi r3, 0x0
    bne @L_80135388
    li r3, 0x0
    b @L_80135398
    @L_80135388:
    bl fn_80135B0C
    cmplwi r3, 0x0
    bne @L_80135398
    li r3, 0x0
    @L_80135398:
    cmplwi r3, 0x0
    beq @L_801353AC
    bl fn_80135708
    mr r3, r31
    bl fn_80135CE8
    @L_801353AC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

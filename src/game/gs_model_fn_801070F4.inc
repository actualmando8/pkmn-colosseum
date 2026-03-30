    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_80104704
    cmplwi r3, 0x0
    bne @L_80107114
    li r3, 0x0
    b @L_80107160
    @L_80107114:
    lwz r0, 0x24(r3)
    cmplwi r0, 0x0
    beq @L_80107128
    li r3, 0x1
    b @L_80107160
    @L_80107128:
    lwz r3, 0x1c(r3)
    b @L_80107154
    @L_80107130:
    lwz r0, 0xc(r3)
    cmplwi r0, 0x0
    beq @L_80107150
    lbz r0, 0x46(r3)
    cmplwi r0, 0x0
    bne @L_80107150
    li r3, 0x1
    b @L_80107160
    @L_80107150:
    lwz r3, 0x0(r3)
    @L_80107154:
    cmplwi r3, 0x0
    bne @L_80107130
    li r3, 0x0
    @L_80107160:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

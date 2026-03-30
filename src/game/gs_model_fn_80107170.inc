    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    bl fn_80104704
    mr r4, r31
    bl fn_801046C8
    cmplwi r3, 0x0
    beq @L_801071B8
    lwz r0, 0xc(r3)
    cmplwi r0, 0x0
    beq @L_801071B8
    lbz r0, 0x46(r3)
    cmplwi r0, 0x0
    bne @L_801071B8
    li r3, 0x1
    b @L_801071BC
    @L_801071B8:
    li r3, 0x0
    @L_801071BC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

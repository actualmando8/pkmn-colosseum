    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r4
    bl fn_80104704
    mr r31, r3
    cmplwi r31, 0x0
    beq @L_80107F1C
    rlwinm r3, r30, 0, 16, 31
    bl fn_8005D830
    lwz r0, 0x24(r31)
    cmplw r0, r3
    bne @L_80107F1C
    li r3, 0x1
    b @L_80107F20
@L_80107F1C:
    li r3, 0x0
@L_80107F20:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

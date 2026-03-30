    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    cmplwi r3, 0x0
    bne @L_8007D520
    li r3, 0xa6
    bl fn_80104704
@L_8007D520:
    bl fn_801040A0
    lwz r4, 0x0(r3)
    lhz r0, 0x0(r4)
    cmplwi r0, 0x0
    beq @L_8007D548
    li r0, 0xe4
    li r3, 0x37
    stw r0, 0x4c(r31)
    bl fn_80132A38
    b @L_8007D550
@L_8007D548:
    li r0, 0x0
    stw r0, 0x4c(r31)
@L_8007D550:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    lwz r3, 0x60(r3)
    lwz r3, 0x0(r3)
    lwz r3, 0x0(r3)
    bl fn_8004BE40
    bl fn_801D16F0
    cmplwi r3, 0x0
    beq @L_8004D624
    li r0, 0x1
    b @L_8004D628
    @L_8004D624:
    li r0, 0x0
    @L_8004D628:
    mr r3, r31
    clrlwi r4, r0, 24
    bl fn_80109220
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

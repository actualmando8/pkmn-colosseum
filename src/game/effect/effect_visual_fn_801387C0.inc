    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r4
    beq @L_8013881C
    mr r31, r3
    lhz r3, 0x12(r3)
    cmplwi r3, 0x0
    beq @L_801387FC
    lhz r0, 0x10(r31)
    cmplw r0, r3
    bge @L_8013881C
    @L_801387FC:
    mr r3, r31
    li r4, 0x0
    bl fn_80138838
    lhz r0, 0x10(r31)
    li r3, 0x1
    add r0, r0, r30
    sth r0, 0x10(r31)
    b @L_80138820
    @L_8013881C:
    li r3, 0x0
    @L_80138820:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

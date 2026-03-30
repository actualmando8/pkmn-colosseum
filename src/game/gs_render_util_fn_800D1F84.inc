    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    stw r30, 0x8(r1)
    mr r30, r3
    lbz r0, 0x4(r3)
    cmplwi r0, 0x0
    beq @L_800D1FB8
    lwz r3, 0xc(r30)
    addi r4, r30, 0x70
    bl fn_80195904
@L_800D1FB8:
    mr r3, r31
    addi r4, r30, 0x70
    bl fn_800E01D0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    li r3, 0xa6
    bl fn_80104704
    bl fn_801040A0
    lwz r31, 0x0(r3)
    cmplwi r31, 0x0
    beq @L_8007CB9C
    mr r3, r31
    mr r4, r30
    li r5, 0x50
    bl fn_800F9D24
    li r0, 0x0
    sth r0, 0x9e(r31)
@L_8007CB9C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

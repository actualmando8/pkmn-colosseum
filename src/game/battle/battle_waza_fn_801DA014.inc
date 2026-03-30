    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    beq @L_801DA05C
    lis r3, 0x8046
    addi r3, r3, 31936
    lwz r0, 0x8(r3)
    cmplwi r0, 0x0
    beq @L_801DA05C
    lwz r3, 0x80(r31)
    cmplwi r3, 0x0
    beq @L_801DA05C
    li r4, 0x1
    bl fn_80118A68
    li r0, 0x0
    stw r0, 0x80(r31)
@L_801DA05C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

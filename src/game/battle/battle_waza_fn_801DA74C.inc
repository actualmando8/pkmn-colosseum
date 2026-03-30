    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r6
    stw r30, 0x8(r1)
    mr. r30, r3
    bne @L_801DA774
    li r3, 0x0
    b @L_801DA794
@L_801DA774:
    bl fn_801DD0C8
    mr. r4, r3
    beq @L_801DA790
    mr r3, r30
    mr r5, r31
    bl fn_801DB3F8
    b @L_801DA794
@L_801DA790:
    li r3, 0x0
@L_801DA794:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

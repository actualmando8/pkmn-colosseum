    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lha r0, 0x6(r4)
    cmpwi r0, 0xa9e
    beq @L_800643A0
    bge @L_800643C4
    cmpwi r0, 0xa88
    beq @L_800643C0
    b @L_800643C4
@L_800643A0:
    lbz r5, 0x8b(r3)
    li r0, -0x100
    li r3, 0x0
    li r4, 0x0
    or r5, r5, r0
    li r6, 0x3c1a
    bl fn_800FB680
    b @L_800643C4
@L_800643C0:
    bl fn_80063AD4
@L_800643C4:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

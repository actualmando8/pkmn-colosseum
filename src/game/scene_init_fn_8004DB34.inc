    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r3, 0x60(r3)
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    beq @L_8004DB60
    mr r3, r4
    li r4, 0x0
    bl fn_80109220
    b @L_8004DB6C
    @L_8004DB60:
    mr r3, r4
    li r4, 0x1
    bl fn_80109220
    @L_8004DB6C:
    li r3, 0x0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

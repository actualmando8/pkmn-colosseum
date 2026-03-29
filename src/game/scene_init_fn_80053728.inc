    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    bl fn_80057E40
    cmpwi r3, 0x2
    beq @L_80053750
    li r0, 0x1
    b @L_80053754
    @L_80053750:
    li r0, 0x0
    @L_80053754:
    mr r3, r31
    clrlwi r4, r0, 24
    bl fn_80109220
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

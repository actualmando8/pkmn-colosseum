    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lbz r0, 0x1(r31)
    cmplwi r0, 0x0
    beq @L_80131744
    lwz r4, 0x30(r31)
    lbz r0, 0x0(r4)
    stb r0, 0x4a(r31)
    bl fn_800FA064
    @L_80131744:
    lwz r4, 0x30(r31)
    li r3, 0x0
    addi r0, r4, 0x1
    stw r0, 0x30(r31)
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

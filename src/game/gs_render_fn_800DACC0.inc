    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r4, lbl_8047AA80(r13)
    lwz r0, 0x480(r4)
    cmplw r0, r31
    beq @L_800DACFC
    lhz r3, 0x2(r31)
    bl fn_800E24B0
    lhz r3, 0x2(r31)
    bl fn_800E209C
    li r0, 0x0
    stb r0, 0x0(r31)
    @L_800DACFC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

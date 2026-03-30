    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    la r31, lbl_80478D78(r13)
    lbz r0, 0x7(r31)
    cmplwi r0, 0x0
    beq @L_80226264
    lwz r3, lbl_8047B610(r13)
    lhz r3, 0x1(r3)
    bl fn_801F000C
    bl fn_8026246C
    li r0, 0x0
    stb r0, 0x7(r31)
@L_80226264:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x3
    stw r0, lbl_8047B610(r13)
    lwz r31, 0xc(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

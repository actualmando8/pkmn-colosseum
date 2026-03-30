    nofralloc
    stwu r1, -0x30(r1)
    stmw r24, 0x10(r1)
    lwz r31, 0x40(r3)
    lwz r24, 0xf4(r3)
    cmplwi r31, 0x0
    beq @L_8017C3D4
    lwz r4, 0x18(r31)
    slwi r0, r24, 2
    add r26, r31, r4
    lwz r4, 0x0(r26)
    add r27, r31, r4
    lwzx r0, r27, r0
    add r28, r31, r0
    b @L_8017C3D8
    @L_8017C3D4:
    li r28, 0x0
    @L_8017C3D8:
    lwz r0, 0xc(r28)
    lwz r30, 0xfc(r3)
    clrrwi. r0, r0, 31
    beq @L_8017C3F4
    li r0, 0x4
    stw r0, 0x0(r30)
    b @L_8017C3FC
    @L_8017C3F4:
    li r0, 0x4
    stw r0, 0x0(r30)
    @L_8017C3FC:
    li r0, 0x64
    stw r0, 0x48(r3)
    li r3, 0x0
    lmw r24, 0x10(r1)
    addi r1, r1, 0x30
    blr

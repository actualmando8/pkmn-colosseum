    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    li r30, 0x0
    lwz r31, lbl_8047B1B4(r13)
    b @L_8017D8D0
    @L_8017D820:
    lwz r0, 0x48(r31)
    cmpwi r0, 0x1f4
    bne @L_8017D8C8
    lwz r0, 0x4c(r31)
    cmpwi r0, 0x2
    beq @L_8017D890
    bge @L_8017D84C
    cmpwi r0, 0x0
    beq @L_8017D858
    bge @L_8017D874
    b @L_8017D8CC
    @L_8017D84C:
    cmpwi r0, 0x7
    beq @L_8017D8AC
    b @L_8017D8CC
    @L_8017D858:
    lwz r4, 0xf8(r31)
    mr r3, r31
    lwz r5, 0x134(r31)
    lwz r6, 0x138(r31)
    lwz r7, 0x13c(r31)
    bl fn_8017E09C
    b @L_8017D8E0
    @L_8017D874:
    lwz r4, 0xf8(r31)
    mr r3, r31
    lwz r5, 0x134(r31)
    lwz r6, 0x138(r31)
    lwz r7, 0x13c(r31)
    bl fn_8017DEA4
    b @L_8017D8E0
    @L_8017D890:
    lwz r4, 0xf8(r31)
    mr r3, r31
    lwz r5, 0x134(r31)
    lwz r6, 0x138(r31)
    lwz r7, 0x13c(r31)
    bl fn_8017DFF4
    b @L_8017D8CC
    @L_8017D8AC:
    lwz r4, 0xf8(r31)
    mr r3, r31
    lwz r5, 0x134(r31)
    lwz r6, 0x138(r31)
    lwz r7, 0x13c(r31)
    bl fn_8017DF4C
    b @L_8017D8CC
    @L_8017D8C8:
    addi r31, r31, 0x140
    @L_8017D8CC:
    addi r30, r30, 0x1
    @L_8017D8D0:
    lis r3, lbl_80453FEC@ha
    lwz r0, lbl_80453FEC@l(r3)
    cmplw r30, r0
    blt @L_8017D820
    @L_8017D8E0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    lis r4, lbl_8036C7E8@ha
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    mr r30, r3
    addi r3, r4, lbl_8036C7E8@l
    stw r29, 0x24(r1)
    bl fn_80193828
    mr. r29, r3
    bne @L_8019BBB8
    la r3, lbl_8047DA74(r2)
    li r4, 0xa1
    la r5, lbl_8047DA80(r2)
    bl fn_80196E10
    @L_8019BBB8:
    mr. r31, r29
    bne @L_8019BBD0
    la r3, lbl_8047DA74(r2)
    li r4, 0xae
    la r5, lbl_8047DA80(r2)
    bl fn_80196E10
    @L_8019BBD0:
    cmplwi r31, 0x0
    beq @L_8019BC44
    cmplwi r30, 0x0
    beq @L_8019BC04
    lwz r0, 0x0(r30)
    stw r0, 0x8(r31)
    lfs f0, 0x8(r30)
    stfs f0, 0x10(r31)
    lfs f0, 0xc(r30)
    stfs f0, 0x14(r31)
    lwz r0, 0x10(r30)
    stw r0, 0x18(r31)
    b @L_8019BC44
    @L_8019BC04:
    addi r3, r1, 0x8
    bl fn_800BD768
    li r0, 0x2
    li r5, 0xff
    stw r0, 0x8(r31)
    li r4, 0xff
    li r3, 0xff
    li r0, 0xff
    lfs f0, 0x18(r1)
    stfs f0, 0x10(r31)
    lfs f0, 0x1c(r1)
    stfs f0, 0x14(r31)
    stb r5, 0x18(r31)
    stb r4, 0x19(r31)
    stb r3, 0x1a(r31)
    stb r0, 0x1b(r31)
    @L_8019BC44:
    lwz r0, 0x4(r30)
    cmplwi r0, 0x0
    beq @L_8019BCF8
    lis r3, lbl_8036C828@ha
    lwz r30, 0x4(r30)
    addi r3, r3, lbl_8036C828@l
    bl fn_80193828
    mr. r29, r3
    bne @L_8019BC78
    la r3, lbl_8047DA74(r2)
    li r4, 0xf9
    la r5, lbl_8047DA7C(r2)
    bl fn_80196E10
    @L_8019BC78:
    cmplwi r29, 0x0
    bne @L_8019BC90
    la r3, lbl_8047DA74(r2)
    li r4, 0x109
    la r5, lbl_8047DA7C(r2)
    bl fn_80196E10
    @L_8019BC90:
    cmplwi r29, 0x0
    beq @L_8019BCF4
    cmplwi r30, 0x0
    beq @L_8019BCCC
    lwz r0, 0x0(r30)
    addi r3, r29, 0x10
    addi r4, r30, 0x8
    li r5, 0x40
    stw r0, 0x8(r29)
    lhz r0, 0x6(r30)
    sth r0, 0xe(r29)
    lhz r0, 0x4(r30)
    sth r0, 0xc(r29)
    bl memcpy
    b @L_8019BCF4
    @L_8019BCCC:
    li r0, 0x0
    li r5, 0x0
    stw r0, 0x8(r29)
    li r0, 0x0
    addi r3, r29, 0x10
    li r4, 0x0
    sth r5, 0xe(r29)
    li r5, 0x40
    sth r0, 0xc(r29)
    bl memset
    @L_8019BCF4:
    stw r29, 0xc(r31)
    @L_8019BCF8:
    lwz r0, 0x34(r1)
    mr r3, r31
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

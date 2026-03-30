    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lbz r0, 0x1(r31)
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_8006EF4C
    b @L_8006EFDC
    @L_8006EF4C:
    lbz r0, 0x95(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8006EF98
    bl fn_80105624
    lhz r0, 0x6(r3)
    clrlwi r0, r0, 31
    cmpwi r0, 0x0
    beq @L_8006EFDC
    lbz r3, 0x94(r31)
    subi r0, r3, 0x1
    stb r0, 0x94(r31)
    lbz r0, 0x94(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bge @L_8006EFE4
    li r0, 0x0
    stb r0, 0x94(r31)
    b @L_8006EFE4
    @L_8006EF98:
    cmpwi r0, 0xa
    bne @L_8006EFDC
    bl fn_80105624
    lhz r0, 0x6(r3)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_8006EFDC
    lbz r3, 0x94(r31)
    addi r0, r3, 0x1
    stb r0, 0x94(r31)
    lbz r0, 0x94(r31)
    extsb r0, r0
    cmpwi r0, 0x32
    ble @L_8006EFE4
    li r0, 0x32
    stb r0, 0x94(r31)
    b @L_8006EFE4
    @L_8006EFDC:
    mr r3, r31
    bl fn_80102F38
    @L_8006EFE4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

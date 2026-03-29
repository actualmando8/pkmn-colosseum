    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    lwz r31, 0x60(r30)
    bl fn_80105624
    lwz r4, 0xc(r31)
    lfs f1, lbl_8047B980(r2)
    lfs f0, 0x0(r4)
    fcmpu cr0, f1, f0
    beq @L_8002BE44
    li r3, 0x0
    b @L_8002BFFC
    @L_8002BE44:
    lhz r4, 0x4(r3)
    lhz r0, 0x8(r3)
    lwz r5, 0x8(r31)
    or r0, r4, r0
    rlwinm r0, r0, 0, 30, 30
    addi r7, r5, 0x1
    cmpwi r0, 0x0
    beq @L_8002BEDC
    lbz r4, 0x95(r30)
    addi r0, r4, 0x1
    stb r0, 0x95(r30)
    lbz r6, 0x95(r30)
    lbz r4, 0x94(r30)
    extsb r5, r6
    extsb r0, r4
    add r0, r5, r0
    cmpw r0, r7
    blt @L_8002BE98
    subi r0, r6, 0x1
    stb r0, 0x95(r30)
    b @L_8002BEDC
    @L_8002BE98:
    cmpwi r5, 0xa
    blt @L_8002BEC4
    addi r4, r4, 0x1
    li r0, 0x1
    stb r4, 0x94(r30)
    lbz r4, 0x95(r30)
    subi r4, r4, 0x1
    stb r4, 0x95(r30)
    lwz r4, 0x14(r31)
    stw r0, 0x0(r4)
    b @L_8002BED0
    @L_8002BEC4:
    lwz r4, 0x14(r31)
    li r0, 0x0
    stw r0, 0x0(r4)
    @L_8002BED0:
    lfs f0, lbl_8047B9B8(r2)
    lwz r4, 0xc(r31)
    stfs f0, 0x0(r4)
    @L_8002BEDC:
    lhz r4, 0x4(r3)
    lhz r0, 0x8(r3)
    or r0, r4, r0
    clrlwi r0, r0, 31
    cmpwi r0, 0x0
    beq @L_8002BF68
    lbz r0, 0x95(r30)
    extsb r0, r0
    cmpwi r0, 0x0
    bgt @L_8002BF14
    lbz r0, 0x94(r30)
    extsb r0, r0
    cmpwi r0, 0x0
    ble @L_8002BF68
    @L_8002BF14:
    lbz r3, 0x95(r30)
    subi r3, r3, 0x1
    extsb r0, r3
    stb r3, 0x95(r30)
    cmpwi r0, 0x0
    bge @L_8002BF50
    li r3, 0x0
    li r0, 0x1
    stb r3, 0x95(r30)
    lbz r3, 0x94(r30)
    subi r3, r3, 0x1
    stb r3, 0x94(r30)
    lwz r3, 0x14(r31)
    stw r0, 0x0(r3)
    b @L_8002BF5C
    @L_8002BF50:
    lwz r3, 0x14(r31)
    li r0, 0x0
    stw r0, 0x0(r3)
    @L_8002BF5C:
    lfs f0, lbl_8047B9BC(r2)
    lwz r3, 0xc(r31)
    stfs f0, 0x0(r3)
    @L_8002BF68:
    lbz r3, 0x94(r30)
    lbz r0, 0x95(r30)
    extsb r3, r3
    extsb r0, r0
    add r4, r3, r0
    cmpwi r4, 0x0
    blt @L_8002BF90
    lwz r0, 0x8(r31)
    cmpw r4, r0
    blt @L_8002BF98
    @L_8002BF90:
    li r3, 0x0
    b @L_8002BFA4
    @L_8002BF98:
    lwz r3, 0x4(r31)
    slwi r0, r4, 1
    lhzx r3, r3, r0
    @L_8002BFA4:
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_8002BFBC
    bl fn_801440A0
    bl fn_80143F84
    b @L_8002BFF4
    @L_8002BFBC:
    lbz r0, 0x1c(r31)
    cmplwi r0, 0x0
    beq @L_8002BFD0
    cmplwi r0, 0x1
    bne @L_8002BFD8
    @L_8002BFD0:
    li r3, 0x2b2d
    b @L_8002BFF4
    @L_8002BFD8:
    lbz r0, 0x1d(r31)
    clrlwi r0, r0, 31
    cmpwi r0, 0x0
    beq @L_8002BFF0
    li r3, 0x2b46
    b @L_8002BFF4
    @L_8002BFF0:
    li r3, 0x2b37
    @L_8002BFF4:
    stw r3, lbl_8047A3E4(r13)
    li r3, 0x0
    @L_8002BFFC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

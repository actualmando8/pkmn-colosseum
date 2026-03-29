    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r3
    bl fn_80105624
    mr r31, r3
    li r3, 0xa
    bl fn_80103E68
    lhz r0, 0x6(r31)
    lhz r3, 0x94(r30)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    sth r3, 0xc(r1)
    beq @L_8004D028
    lbz r3, 0xd(r1)
    addi r3, r3, 0x1
    extsb r0, r3
    stb r3, 0xd(r1)
    cmpwi r0, 0xb
    ble @L_8004CFD8
    li r0, 0x0
    stb r0, 0xd(r1)
    @L_8004CFD8:
    bl fn_801D1F7C
    lbz r0, 0xc(r1)
    extsb r0, r0
    mulli r0, r0, 0xa
    subf r3, r0, r3
    cmpwi r3, 0xa
    ble @L_8004CFFC
    li r3, 0xa
    b @L_8004D008
    @L_8004CFFC:
    cmpwi r3, 0x0
    bge @L_8004D008
    li r3, 0x0
    @L_8004D008:
    lbz r0, 0xd(r1)
    extsb r0, r0
    cmpwi r0, 0xa
    bge @L_8004D028
    cmpw r0, r3
    blt @L_8004D028
    li r0, 0xa
    stb r0, 0xd(r1)
    @L_8004D028:
    lhz r0, 0x6(r31)
    clrlwi r0, r0, 31
    cmpwi r0, 0x0
    beq @L_8004D0C0
    lbz r3, 0xd(r1)
    subi r3, r3, 0x1
    extsb r0, r3
    stb r3, 0xd(r1)
    cmpwi r0, 0x0
    bge @L_8004D058
    li r0, 0xb
    stb r0, 0xd(r1)
    @L_8004D058:
    bl fn_801D1F7C
    lbz r0, 0xc(r1)
    extsb r0, r0
    mulli r0, r0, 0xa
    subf r3, r0, r3
    cmpwi r3, 0xa
    ble @L_8004D07C
    li r3, 0xa
    b @L_8004D088
    @L_8004D07C:
    cmpwi r3, 0x0
    bge @L_8004D088
    li r3, 0x0
    @L_8004D088:
    lbz r0, 0xd(r1)
    extsb r0, r0
    cmpwi r0, 0xa
    bge @L_8004D0C0
    cmpw r0, r3
    blt @L_8004D0C0
    cmpwi r3, 0x0
    ble @L_8004D0B8
    subi r0, r3, 0x1
    extsb r0, r0
    stb r0, 0xd(r1)
    b @L_8004D0C0
    @L_8004D0B8:
    li r0, 0xb
    stb r0, 0xd(r1)
    @L_8004D0C0:
    lhz r0, 0x6(r31)
    rlwinm r0, r0, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_8004D178
    lbz r0, 0xd(r1)
    extsb r0, r0
    cmpwi r0, 0xa
    bge @L_8004D170
    bl fn_801D1F7C
    lis r5, 0x6666
    lbz r4, 0xc(r1)
    addi r3, r3, 0x9
    addi r5, r5, 0x6667
    addi r0, r4, 0x1
    mulhw r3, r5, r3
    stb r0, 0xc(r1)
    extsb r0, r0
    srawi r3, r3, 2
    srwi r4, r3, 31
    add r3, r3, r4
    cmpw r0, r3
    blt @L_8004D120
    li r0, 0x0
    stb r0, 0xc(r1)
    @L_8004D120:
    bl fn_801D1F7C
    lbz r0, 0xc(r1)
    extsb r0, r0
    mulli r0, r0, 0xa
    subf r3, r0, r3
    cmpwi r3, 0xa
    ble @L_8004D144
    li r3, 0xa
    b @L_8004D150
    @L_8004D144:
    cmpwi r3, 0x0
    bge @L_8004D150
    li r3, 0x0
    @L_8004D150:
    lbz r0, 0xd(r1)
    extsb r0, r0
    cmpw r0, r3
    blt @L_8004D178
    subi r0, r3, 0x1
    extsb r0, r0
    stb r0, 0xd(r1)
    b @L_8004D178
    @L_8004D170:
    li r0, 0xb
    stb r0, 0xd(r1)
    @L_8004D178:
    lhz r0, 0x6(r31)
    rlwinm r0, r0, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_8004D234
    lbz r0, 0xd(r1)
    extsb r0, r0
    cmpwi r0, 0xa
    bge @L_8004D22C
    lbz r3, 0xc(r1)
    subi r3, r3, 0x1
    extsb r0, r3
    stb r3, 0xc(r1)
    cmpwi r0, 0x0
    bge @L_8004D1DC
    bl fn_801D1F7C
    lis r4, 0x6666
    addi r0, r3, 0x9
    addi r3, r4, 0x6667
    mulhw r0, r3, r0
    srawi r0, r0, 2
    srwi r3, r0, 31
    add r3, r0, r3
    subi r0, r3, 0x1
    extsb r0, r0
    stb r0, 0xc(r1)
    @L_8004D1DC:
    bl fn_801D1F7C
    lbz r0, 0xc(r1)
    extsb r0, r0
    mulli r0, r0, 0xa
    subf r3, r0, r3
    cmpwi r3, 0xa
    ble @L_8004D200
    li r3, 0xa
    b @L_8004D20C
    @L_8004D200:
    cmpwi r3, 0x0
    bge @L_8004D20C
    li r3, 0x0
    @L_8004D20C:
    lbz r0, 0xd(r1)
    extsb r0, r0
    cmpw r0, r3
    blt @L_8004D234
    subi r0, r3, 0x1
    extsb r0, r0
    stb r0, 0xd(r1)
    b @L_8004D234
    @L_8004D22C:
    li r0, 0xa
    stb r0, 0xd(r1)
    @L_8004D234:
    lhz r0, 0xc(r1)
    addi r4, r1, 0x8
    li r3, 0xa
    sth r0, 0x8(r1)
    bl fn_80103EAC
    lhz r0, 0xc(r1)
    li r3, 0x0
    sth r0, 0x94(r30)
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

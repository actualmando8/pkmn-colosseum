    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    lbz r0, 0x1(r30)
    li r31, 0x0
    extsb r0, r0
    cmpwi r0, 0x3
    beq @L_80013704
    bge @L_800136A4
    cmpwi r0, 0x0
    beq @L_800136B0
    b @L_80013728
    @L_800136A4:
    cmpwi r0, 0x5
    beq @L_80013728
    b @L_80013728
    @L_800136B0:
    lbz r0, 0x2(r30)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_800136C4
    li r31, 0x1
    @L_800136C4:
    lwz r3, 0x4(r30)
    li r4, 0x2a
    bl fn_80107ED8
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_800136E0
    li r31, 0x1
    @L_800136E0:
    clrlwi r0, r31, 24
    cmplwi r0, 0x0
    beq @L_80013728
    lwz r3, 0x4(r30)
    li r4, 0x26
    bl fn_801080CC
    li r0, 0x1
    stb r0, 0x2(r30)
    b @L_80013728
    @L_80013704:
    lbz r0, 0x2(r30)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80013728
    lwz r3, 0x4(r30)
    li r4, 0x2a
    bl fn_801080CC
    li r0, 0x1
    stb r0, 0x2(r30)
    @L_80013728:
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

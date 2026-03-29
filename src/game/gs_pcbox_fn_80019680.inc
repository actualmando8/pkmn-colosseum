    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lbz r0, 0x1(r31)
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_800196F0
    bge @L_800196B4
    cmpwi r0, 0x0
    beq @L_800196C0
    b @L_8001973C
    @L_800196B4:
    cmpwi r0, 0x4
    bge @L_8001973C
    b @L_80019718
    @L_800196C0:
    lbz r0, 0x2(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8001973C
    li r3, 0x5f
    li r4, 0x66
    bl fn_801080CC
    lfs f0, lbl_8047B7A8(r2)
    li r0, 0x1
    stfs f0, lbl_8047A300(r13)
    stb r0, 0x2(r31)
    b @L_8001973C
    @L_800196F0:
    lfs f2, lbl_8047A300(r13)
    lfs f1, lbl_8047B7AC(r2)
    lfs f0, lbl_8047B7A4(r2)
    fadds f1, f2, f1
    fcmpo cr0, f1, f0
    stfs f1, lbl_8047A300(r13)
    ble @L_8001973C
    lfs f0, lbl_8047B7A8(r2)
    stfs f0, lbl_8047A300(r13)
    b @L_8001973C
    @L_80019718:
    lbz r0, 0x2(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8001973C
    li r3, 0x5f
    li r4, 0x6a
    bl fn_801080CC
    li r0, 0x1
    stb r0, 0x2(r31)
    @L_8001973C:
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

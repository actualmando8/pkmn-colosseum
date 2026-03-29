    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r31, r3
    bl fn_80105624
    lwz r0, lbl_8047A520(r13)
    mr r30, r3
    cmpwi r0, 0x0
    beq @L_8004DE74
    lbz r3, lbl_8047A524(r13)
    bl fn_801D1650
    cmplwi r3, 0x0
    beq @L_8004DE04
    bl fn_801666BC
    @L_8004DE04:
    lbz r3, lbl_8047A524(r13)
    bl fn_801D1650
    cmplwi r3, 0x0
    beq @L_8004DE18
    bl fn_80166B18
    @L_8004DE18:
    lbz r0, 0x95(r31)
    stb r0, lbl_8047A524(r13)
    clrlwi r3, r0, 24
    bl fn_801D1650
    mr r29, r3
    cmplwi r29, 0x0
    beq @L_8004DE6C
    lwz r4, lbl_8047A52C(r13)
    lis r5, 0x1
    bl fn_801654E0
    mr r3, r29
    li r4, 0x0
    li r5, 0x408
    bl fn_80166B3C
    mr r3, r29
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
    li r0, 0x0
    stw r0, lbl_8047A528(r13)
    @L_8004DE6C:
    li r0, 0x0
    stw r0, lbl_8047A520(r13)
    @L_8004DE74:
    lhz r0, 0x4(r30)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_8004DEB0
    bl fn_801D1618
    lbz r4, 0x95(r31)
    addi r5, r3, 0x1
    addi r3, r4, 0x1
    extsb r0, r3
    stb r3, 0x95(r31)
    cmpw r0, r5
    blt @L_8004DEB0
    subi r0, r5, 0x1
    extsb r0, r0
    stb r0, 0x95(r31)
    @L_8004DEB0:
    lhz r0, 0x4(r30)
    clrlwi r0, r0, 31
    cmpwi r0, 0x0
    beq @L_8004DEE0
    lbz r3, 0x95(r31)
    subi r3, r3, 0x1
    extsb r0, r3
    stb r3, 0x95(r31)
    cmpwi r0, 0x0
    bge @L_8004DEE0
    li r0, 0x0
    stb r0, 0x95(r31)
    @L_8004DEE0:
    lbz r0, 0x95(r31)
    lbz r3, lbl_8047A524(r13)
    extsb r0, r0
    cmpw r3, r0
    beq @L_8004DF14
    bl fn_801D1650
    cmplwi r3, 0x0
    beq @L_8004DF0C
    li r4, 0x0
    li r5, 0x0
    bl fn_801669E4
    @L_8004DF0C:
    li r0, 0x1
    stw r0, lbl_8047A520(r13)
    @L_8004DF14:
    li r3, 0x0
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

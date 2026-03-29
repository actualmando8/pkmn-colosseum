    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r31, r3
    clrlwi r0, r31, 24
    stw r0, 0x8(r1)
    bl fn_8016557C
    mr r0, r3
    lis r3, 0x1
    mr r30, r0
    li r4, 0x20
    li r5, 0x0
    li r6, 0x408
    li r7, 0x0
    bl fn_800F9418
    stw r3, lbl_8047A52C(r13)
    li r4, 0x0
    li r5, 0x408
    li r6, 0x0
    bl fn_800F9378
    stb r31, lbl_8047A524(r13)
    mr r3, r31
    bl fn_801D1650
    mr r31, r3
    cmplwi r31, 0x0
    beq @L_8004E074
    lwz r4, lbl_8047A52C(r13)
    lis r5, 0x1
    bl fn_801654E0
    mr r3, r31
    li r4, 0x0
    li r5, 0x408
    bl fn_80166B3C
    mr r3, r31
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
    li r0, 0x0
    stw r0, lbl_8047A528(r13)
    @L_8004E074:
    bl fn_801046B8
    mr r4, r3
    addi r5, r1, 0x8
    li r3, 0x76
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    mr r31, r3
    cmpwi r31, 0x0
    blt @L_8004E0B0
    bl fn_801D1618
    cmpw r31, r3
    blt @L_8004E0B8
    @L_8004E0B0:
    li r31, 0xff
    b @L_8004E0BC
    @L_8004E0B8:
    clrlwi r31, r31, 24
    @L_8004E0BC:
    li r3, 0x76
    bl fn_80102510
    li r3, 0x76
    li r4, 0x1
    bl fn_80102428
    lbz r3, lbl_8047A524(r13)
    bl fn_801D1650
    cmplwi r3, 0x0
    beq @L_8004E0EC
    li r4, 0x0
    li r5, 0x0
    bl fn_801669E4
    @L_8004E0EC:
    lbz r3, lbl_8047A524(r13)
    bl fn_801D1650
    cmplwi r3, 0x0
    beq @L_8004E100
    bl fn_801666BC
    @L_8004E100:
    lbz r3, lbl_8047A524(r13)
    bl fn_801D1650
    cmplwi r3, 0x0
    beq @L_8004E114
    bl fn_80166B18
    @L_8004E114:
    li r3, 0x0
    li r4, 0x408
    bl fn_800F9210
    mr r3, r30
    bl fn_80165548
    mr r3, r31
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

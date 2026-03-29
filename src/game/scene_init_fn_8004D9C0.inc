    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r3, lbl_8047A518(r13)
    la r31, lbl_8047A518(r13)
    @L_8004D9DC:
    stw r31, 0x8(r1)
    bl fn_801046B8
    mr r4, r3
    addi r9, r1, 0x8
    li r3, 0x74
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    cmpwi r3, -0x1
    beq @L_8004DA34
    lwz r3, lbl_8047A518(r13)
    bl fn_8004BE40
    mr r30, r3
    bl fn_801D16F0
    cmplwi r3, 0x0
    beq @L_8004D9DC
    mr r3, r30
    bl fn_8004E9C0
    b @L_8004D9DC
    @L_8004DA34:
    li r3, 0x74
    bl fn_80102510
    li r3, 0x74
    li r4, 0x1
    bl fn_80102428
    lwz r3, lbl_8047A518(r13)
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

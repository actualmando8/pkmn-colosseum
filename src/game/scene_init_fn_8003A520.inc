    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    li r31, 0x0
    @L_8003A534:
    mr r3, r31
    bl fn_80039498
    mr r31, r3
    subi r0, r31, 0x3
    cmplwi r0, 0x1
    ble @L_8003A6AC
    cmpwi r31, 0x1
    beq @L_8003A5BC
    bge @L_8003A564
    cmpwi r31, 0x0
    bge @L_8003A570
    b @L_8003A534
    @L_8003A564:
    cmpwi r31, 0x3
    bge @L_8003A534
    b @L_8003A660
    @L_8003A570:
    li r3, 0x19
    bl fn_80102510
    li r3, 0x1b
    bl fn_80102510
    li r3, 0x19
    li r4, 0x1
    bl fn_80102428
    li r3, 0x1b
    li r4, 0x1
    bl fn_80102428
    li r3, 0x0
    bl fn_8003A10C
    li r3, 0x1b
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x19
    li r4, 0x0
    bl fn_8010264C
    b @L_8003A534
    @L_8003A5BC:
    li r3, 0x19
    bl fn_80102510
    li r3, 0x1a
    bl fn_80102510
    li r3, 0x1b
    bl fn_80102510
    li r3, 0x19
    li r4, 0x1
    bl fn_80102428
    li r3, 0x1a
    li r4, 0x1
    bl fn_80102428
    li r3, 0x1b
    li r4, 0x1
    bl fn_80102428
    li r3, 0x66f
    bl fn_8017B3E4
    b @L_8003A608
    @L_8003A604:
    bl fn_800F0308
    @L_8003A608:
    li r3, 0x66f
    bl fn_8017B2CC
    cmpwi r3, 0x1
    beq @L_8003A604
    li r3, 0x4
    li r4, 0x0
    li r5, 0x0
    bl fn_80018F54
    li r3, 0x66f
    bl fn_8017B1CC
    li r3, 0x66f
    bl fn_800F915C
    li r3, 0x1a
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x1b
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x19
    li r4, 0x0
    bl fn_8010264C
    b @L_8003A534
    @L_8003A660:
    li r3, 0x19
    bl fn_80102510
    li r3, 0x1b
    bl fn_80102510
    li r3, 0x19
    li r4, 0x1
    bl fn_80102428
    li r3, 0x1b
    li r4, 0x1
    bl fn_80102428
    li r3, 0x1
    bl fn_8003A10C
    li r3, 0x1b
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x19
    li r4, 0x0
    bl fn_8010264C
    b @L_8003A534
    @L_8003A6AC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

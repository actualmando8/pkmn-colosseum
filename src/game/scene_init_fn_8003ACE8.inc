    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r3, lbl_8047A4C8(r13)
    stw r5, 0x8(r1)
    stw r4, 0xc(r1)
    bl fn_801046B8
    mr r4, r3
    addi r9, r1, 0x8
    li r3, 0x26
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    mr r31, r3
    li r3, 0x26
    bl fn_80102510
    li r3, 0x26
    li r4, 0x1
    bl fn_80102428
    cmpwi r31, -0x1
    bne @L_8003AD54
    li r3, -0x1
    b @L_8003AD58
    @L_8003AD54:
    lwz r3, lbl_8047A4C8(r13)
    @L_8003AD58:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

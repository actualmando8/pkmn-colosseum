    nofralloc
    stwu r1, -0x7a0(r1)
    mflr r0
    stw r0, 0x7a4(r1)
    stw r31, 0x79c(r1)
    stw r30, 0x798(r1)
    mr r30, r3
    mr r31, r4
    clrlwi r0, r31, 24
    cmplwi r0, 0x3
    beq @L_8002C2C4
    cmplwi r0, 0x2
    beq @L_8002C2C4
    li r4, 0xa
    li r5, -0x1
    crxor 6, 6, 6
    bl fn_8002A1C4
    @L_8002C2C4:
    lwz r0, lbl_804788A8(r13)
    cmpwi r0, 0x0
    beq @L_8002C33C
    lis r3, lbl_802E4F68@ha
    lis r4, lbl_802EF0A8@ha
    addi r8, r3, lbl_802E4F68@l
    li r0, 0x0
    lwz r3, 0x0(r8)
    addi r4, r4, lbl_802EF0A8@l
    lwz r6, 0x8(r8)
    addi r9, r4, 0x4
    lwz r5, 0x10(r8)
    mulli r7, r3, 0x1c
    lwz r4, 0x18(r8)
    lwz r3, 0x20(r8)
    mulli r6, r6, 0x1c
    lhax r7, r9, r7
    stw r0, lbl_804788A8(r13)
    mulli r0, r5, 0x1c
    lhax r6, r9, r6
    sth r7, 0x4(r8)
    mulli r4, r4, 0x1c
    lhax r5, r9, r0
    sth r6, 0xc(r8)
    mulli r0, r3, 0x1c
    lhax r3, r9, r4
    sth r5, 0x14(r8)
    lhax r0, r9, r0
    sth r3, 0x1c(r8)
    sth r0, 0x24(r8)
    @L_8002C33C:
    li r7, 0x0
    la r5, lbl_8047A3F8(r13)
    lwz r3, lbl_80478E54(r13)
    slwi r0, r30, 2
    sth r7, lbl_8047A3F8(r13)
    add r3, r3, r0
    lwz r4, lbl_80478E44(r13)
    stw r5, 0x8(r1)
    lhz r0, 0x2(r3)
    slwi r0, r0, 1
    add r8, r4, r0
    mr r3, r8
    b @L_8002C378
    @L_8002C370:
    addi r3, r3, 0x2
    addi r7, r7, 0x1
    @L_8002C378:
    lhz r0, 0x0(r3)
    cmplwi r0, 0x0
    bne @L_8002C370
    la r6, lbl_8047A3F4(r13)
    la r5, lbl_8047A3F0(r13)
    la r4, lbl_8047A3EC(r13)
    la r3, lbl_8047A3E8(r13)
    li r0, 0x1
    stw r7, 0x10(r1)
    stw r8, 0xc(r1)
    stw r6, 0x14(r1)
    stw r5, 0x18(r1)
    stw r4, 0x1c(r1)
    stw r3, 0x20(r1)
    stb r31, 0x24(r1)
    stb r0, 0x25(r1)
    bl fn_801046B8
    mr r4, r3
    addi r9, r1, 0x8
    li r3, 0x60
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    li r3, 0x60
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    lwz r0, 0x7a4(r1)
    lwz r31, 0x79c(r1)
    lwz r30, 0x798(r1)
    mtlr r0
    addi r1, r1, 0x7a0
    blr

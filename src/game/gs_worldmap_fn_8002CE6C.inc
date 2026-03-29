    nofralloc
    stwu r1, -0x7b0(r1)
    mflr r0
    stw r0, 0x7b4(r1)
    stmw r26, 0x798(r1)
    mr r29, r3
    mr r30, r4
    slwi r31, r29, 2
    @L_8002CE88:
    li r3, 0x60
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x0
    li r4, 0xc
    li r5, 0x0
    bl fn_8012A5B0
    mr r6, r3
    mr r3, r29
    li r4, 0x0
    li r5, 0x4b
    li r7, -0x1
    crxor 6, 6, 6
    bl fn_8002A2CC
    li r28, 0x0
    b @L_8002CED8
    @L_8002CECC:
    bl fn_800F0308
    bl fn_800D3088
    add r28, r28, r3
    @L_8002CED8:
    cmpwi r28, 0x1e
    blt @L_8002CECC
    lwz r0, lbl_804788A8(r13)
    cmpwi r0, 0x0
    beq @L_8002CF68
    lis r4, lbl_802E4F68@ha
    lis r3, lbl_802EF0A8@ha
    addi r9, r4, lbl_802E4F68@l
    li r0, 0x0
    lwz r4, 0x0(r9)
    addi r8, r3, lbl_802EF0A8@l
    lwz r6, 0x8(r9)
    lwz r5, 0x10(r9)
    mulli r7, r4, 0x1c
    lwz r4, 0x18(r9)
    lwz r3, 0x20(r9)
    mulli r6, r6, 0x1c
    stw r0, lbl_804788A8(r13)
    add r7, r8, r7
    lha r7, 0x4(r7)
    mulli r0, r5, 0x1c
    add r5, r8, r6
    sth r7, 0x4(r9)
    lha r6, 0x4(r5)
    mulli r4, r4, 0x1c
    add r5, r8, r0
    sth r6, 0xc(r9)
    lha r5, 0x4(r5)
    mulli r0, r3, 0x1c
    add r3, r8, r4
    sth r5, 0x14(r9)
    lha r4, 0x4(r3)
    add r3, r8, r0
    lha r0, 0x4(r3)
    sth r4, 0x1c(r9)
    sth r0, 0x24(r9)
    @L_8002CF68:
    li r7, 0x0
    lwz r3, lbl_80478E54(r13)
    la r4, lbl_8047A3F8(r13)
    sth r7, lbl_8047A3F8(r13)
    addi r0, r3, 0x2
    lwz r3, lbl_80478E44(r13)
    stw r4, 0x8(r1)
    lhzx r0, r31, r0
    slwi r0, r0, 1
    add r8, r3, r0
    mr r3, r8
    b @L_8002CFA0
    @L_8002CF98:
    addi r3, r3, 0x2
    addi r7, r7, 0x1
    @L_8002CFA0:
    lhz r0, 0x0(r3)
    cmplwi r0, 0x0
    bne @L_8002CF98
    la r6, lbl_8047A3F4(r13)
    la r5, lbl_8047A3F0(r13)
    la r4, lbl_8047A3EC(r13)
    la r3, lbl_8047A3E8(r13)
    li r0, 0x0
    stw r7, 0x10(r1)
    stw r8, 0xc(r1)
    stw r6, 0x14(r1)
    stw r5, 0x18(r1)
    stw r4, 0x1c(r1)
    stw r3, 0x20(r1)
    stb r30, 0x24(r1)
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
    cmpwi r3, -0x1
    bne @L_8002D018
    li r28, 0x0
    b @L_8002D020
    @L_8002D018:
    lwz r3, 0x8(r1)
    lhz r28, 0x0(r3)
    @L_8002D020:
    clrlwi r0, r28, 16
    cmplwi r0, 0x0
    bne @L_8002D044
    mr r3, r29
    li r4, 0x2
    li r5, -0x1
    crxor 6, 6, 6
    bl fn_8002A2CC
    b @L_8002D130
    @L_8002D044:
    mr r3, r28
    bl fn_801440A0
    bl fn_80143FFC
    clrlwi r27, r3, 16
    mr r3, r28
    bl fn_801440A0
    bl fn_80143FFC
    clrlwi r26, r3, 16
    cmpwi r26, 0x0
    ble @L_8002D090
    li r3, 0x0
    li r4, 0xc
    li r5, 0x0
    bl fn_8012A5B0
    divw r0, r3, r26
    cmpwi r0, 0x63
    ble @L_8002D094
    li r0, 0x63
    b @L_8002D094
    @L_8002D090:
    li r0, 0x63
    @L_8002D094:
    cmpwi r0, 0x0
    bgt @L_8002D0B4
    mr r3, r29
    li r4, 0x5
    li r5, -0x1
    crxor 6, 6, 6
    bl fn_8002A2CC
    b @L_8002D130
    @L_8002D0B4:
    mr r4, r28
    li r3, 0x0
    bl fn_801298B8
    cmpwi r3, 0x1
    bge @L_8002D0E0
    mr r3, r29
    li r4, 0x6
    li r5, -0x1
    crxor 6, 6, 6
    bl fn_8002A2CC
    b @L_8002D130
    @L_8002D0E0:
    li r3, 0x3cb
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    mr r4, r27
    li r3, 0x0
    bl fn_80129474
    mr r4, r28
    li r3, 0x0
    li r5, 0x1
    li r6, -0x1
    bl fn_80129A78
    mr r3, r29
    clrlwi r6, r28, 16
    li r4, 0x4
    li r5, 0x2d
    li r7, -0x1
    crxor 6, 6, 6
    bl fn_8002A2CC
    b @L_8002CE88
    @L_8002D130:
    li r3, 0x60
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    lmw r26, 0x798(r1)
    lwz r0, 0x7b4(r1)
    mtlr r0
    addi r1, r1, 0x7b0
    blr

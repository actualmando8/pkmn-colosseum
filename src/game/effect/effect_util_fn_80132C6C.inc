    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    mr. r0, r3
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    beq @L_80132F60
    cmplwi r4, 0x0
    bne @L_80132C98
    b @L_80132F60
    @L_80132C98:
    mulli r3, r0, 0x18
    stw r0, lbl_8047AEB4(r13)
    stw r4, lbl_8047AEC0(r13)
    stw r6, lbl_8047AECC(r13)
    stw r5, lbl_8047AEC8(r13)
    bl fn_800E3534
    clrlwi. r0, r3, 16
    sth r3, lbl_8047AEB8(r13)
    beq @L_80132F60
    mr r3, r0
    bl fn_800E27B0
    li r10, 0x0
    stw r3, lbl_8047AEB0(r13)
    mr r8, r10
    li r11, 0x0
    mr r7, r10
    mr r6, r10
    mr r5, r10
    mr r4, r10
    mr r3, r10
    mr r0, r10
    b @L_80132D1C
    @L_80132CF0:
    lwz r9, lbl_8047AEB0(r13)
    addi r11, r11, 0x1
    add r9, r9, r10
    addi r10, r10, 0x18
    stw r8, 0x0(r9)
    stw r7, 0x4(r9)
    stw r6, 0x8(r9)
    stw r5, 0xc(r9)
    stw r4, 0x10(r9)
    stb r3, 0x14(r9)
    stb r0, 0x15(r9)
    @L_80132D1C:
    lwz r9, lbl_8047AEB4(r13)
    cmplw r11, r9
    blt @L_80132CF0
    lwz r0, lbl_8047AEC0(r13)
    mullw r31, r9, r0
    slwi r3, r31, 5
    bl fn_800E3534
    clrlwi. r0, r3, 16
    sth r3, lbl_8047AEC4(r13)
    beq @L_80132F60
    mr r3, r0
    bl fn_800E27B0
    cmplwi r31, 0x0
    stw r3, lbl_8047AEBC(r13)
    li r7, 0x0
    ble @L_80132F60
    cmplwi r31, 0x8
    subi r3, r31, 0x8
    ble @L_80132F10
    addi r0, r3, 0x7
    mr r6, r7
    srwi r0, r0, 3
    mtctr r0
    cmplwi r3, 0x0
    ble @L_80132F10
    @L_80132D80:
    lwz r0, lbl_8047AEBC(r13)
    li r4, -0x1
    li r3, 0x0
    addi r29, r6, 0x20
    add r5, r0, r6
    addi r30, r6, 0x40
    stw r4, 0x0(r5)
    addi r12, r6, 0x60
    addi r11, r6, 0x80
    addi r10, r6, 0xa0
    sth r3, 0x4(r5)
    addi r9, r6, 0xc0
    addi r8, r6, 0xe0
    addi r6, r6, 0x100
    sth r3, 0x6(r5)
    addi r7, r7, 0x8
    stw r3, 0x8(r5)
    stw r3, 0xc(r5)
    stw r4, 0x10(r5)
    stb r3, 0x14(r5)
    stw r3, 0x18(r5)
    stw r3, 0x1c(r5)
    lwz r0, lbl_8047AEBC(r13)
    add r29, r0, r29
    stw r4, 0x0(r29)
    sth r3, 0x4(r29)
    sth r3, 0x6(r29)
    stw r3, 0x8(r29)
    stw r3, 0xc(r29)
    stw r4, 0x10(r29)
    stb r3, 0x14(r29)
    stw r3, 0x18(r29)
    stw r3, 0x1c(r29)
    lwz r0, lbl_8047AEBC(r13)
    add r30, r0, r30
    stw r4, 0x0(r30)
    sth r3, 0x4(r30)
    sth r3, 0x6(r30)
    stw r3, 0x8(r30)
    stw r3, 0xc(r30)
    stw r4, 0x10(r30)
    stb r3, 0x14(r30)
    stw r3, 0x18(r30)
    stw r3, 0x1c(r30)
    lwz r0, lbl_8047AEBC(r13)
    add r12, r0, r12
    stw r4, 0x0(r12)
    sth r3, 0x4(r12)
    sth r3, 0x6(r12)
    stw r3, 0x8(r12)
    stw r3, 0xc(r12)
    stw r4, 0x10(r12)
    stb r3, 0x14(r12)
    stw r3, 0x18(r12)
    stw r3, 0x1c(r12)
    lwz r0, lbl_8047AEBC(r13)
    add r11, r0, r11
    stw r4, 0x0(r11)
    sth r3, 0x4(r11)
    sth r3, 0x6(r11)
    stw r3, 0x8(r11)
    stw r3, 0xc(r11)
    stw r4, 0x10(r11)
    stb r3, 0x14(r11)
    stw r3, 0x18(r11)
    stw r3, 0x1c(r11)
    lwz r0, lbl_8047AEBC(r13)
    add r10, r0, r10
    stw r4, 0x0(r10)
    sth r3, 0x4(r10)
    sth r3, 0x6(r10)
    stw r3, 0x8(r10)
    stw r3, 0xc(r10)
    stw r4, 0x10(r10)
    stb r3, 0x14(r10)
    stw r3, 0x18(r10)
    stw r3, 0x1c(r10)
    lwz r0, lbl_8047AEBC(r13)
    add r9, r0, r9
    stw r4, 0x0(r9)
    sth r3, 0x4(r9)
    sth r3, 0x6(r9)
    stw r3, 0x8(r9)
    stw r3, 0xc(r9)
    stw r4, 0x10(r9)
    stb r3, 0x14(r9)
    stw r3, 0x18(r9)
    stw r3, 0x1c(r9)
    lwz r0, lbl_8047AEBC(r13)
    add r8, r0, r8
    stw r4, 0x0(r8)
    sth r3, 0x4(r8)
    sth r3, 0x6(r8)
    stw r3, 0x8(r8)
    stw r3, 0xc(r8)
    stw r4, 0x10(r8)
    stb r3, 0x14(r8)
    stw r3, 0x18(r8)
    stw r3, 0x1c(r8)
    bdnz @L_80132D80
    @L_80132F10:
    subf r0, r7, r31
    slwi r5, r7, 5
    li r4, -0x1
    li r3, 0x0
    mtctr r0
    cmplw r7, r31
    bge @L_80132F60
    @L_80132F2C:
    lwz r0, lbl_8047AEBC(r13)
    add r6, r0, r5
    addi r5, r5, 0x20
    stw r4, 0x0(r6)
    sth r3, 0x4(r6)
    sth r3, 0x6(r6)
    stw r3, 0x8(r6)
    stw r3, 0xc(r6)
    stw r4, 0x10(r6)
    stb r3, 0x14(r6)
    stw r3, 0x18(r6)
    stw r3, 0x1c(r6)
    bdnz @L_80132F2C
    @L_80132F60:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

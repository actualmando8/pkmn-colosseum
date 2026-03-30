    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r4, 0x2
    li r3, 0x3
    stw r0, 0x24(r1)
    subi r7, r4, 0x2000
    li r0, 0x0
    li r9, 0x0
    stw r31, 0x1c(r1)
    li r31, -0x1
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mtctr r3
@L_801CFA00:
    lwz r8, -21580(r13)
    lwz r3, 0x50(r8)
    add r6, r3, r7
    lwz r5, 0x0(r6)
    lwz r3, 0x4(r6)
    lwz r4, 0x8(r6)
    add r5, r5, r3
    lwz r3, 0xc(r6)
    add r5, r5, r4
    lwz r4, 0x10(r6)
    add r5, r5, r3
    lwz r3, 0x14(r6)
    add r5, r5, r4
    lwz r4, 0x18(r6)
    add r5, r5, r3
    lwz r3, 0x1c(r6)
    add r5, r5, r4
    add. r5, r5, r3
    beq @L_801CFA5C
    li r0, 0x11
    li r3, 0x2b
    stw r0, 0x4(r8)
    b @L_801CFCE8
@L_801CFA5C:
    lbz r3, 0x0(r6)
    li r4, 0x0
    cmplwi r3, 0x1
    bne @L_801CFA7C
    lbz r3, 0x1(r6)
    cmplwi r3, 0x1
    bne @L_801CFA7C
    li r4, 0x1
@L_801CFA7C:
    cmpwi r4, 0x0
    bne @L_801CFA94
    li r0, 0x13
    li r3, 0x2b
    stw r0, 0x4(r8)
    b @L_801CFCE8
@L_801CFA94:
    lwz r4, 0x4(r6)
    cmpw r4, r0
    ble @L_801CFAB8
    lwz r3, 0x30(r8)
    mr r0, r4
    mr r31, r9
    cmpw r4, r3
    ble @L_801CFAB8
    stw r4, 0x30(r8)
@L_801CFAB8:
    addi r7, r7, 0x200
    addi r9, r9, 0x1
    bdnz @L_801CFA00
    lwz r3, -21580(r13)
    stw r31, 0x20(r3)
    lwz r5, -21580(r13)
    lwz r3, 0x0(r5)
    cmplwi r3, 0xd
    bgt @L_801CFCD8
    lis r4, -32713
    rlwinm r3, r3, 2, 0, 29
    addi r4, r4, -8332
    lwzx r3, r4, r3
    mtctr r3
    bctr
    lwz r29, 0x50(r5)
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    li r0, 0x3bfa
    addi r5, r29, 0x4
    subi r4, r3, 0x4
    mtctr r0
@L_801CFB14:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_801CFB14
    lwz r4, -21580(r13)
    li r3, 0x25
    lwz r0, 0x30(r4)
    stw r0, 0x2c(r4)
    b @L_801CFCE8
    cmpwi r31, -0x1
    bne @L_801CFC14
    lwz r0, 0x30(r5)
    cmpwi r0, 0x0
    ble @L_801CFB84
    lwz r0, 0x0(r5)
    cmpwi r0, 0x9
    beq @L_801CFB6C
    bge @L_801CFB74
    cmpwi r0, 0x3
    beq @L_801CFB84
    b @L_801CFB74
@L_801CFB6C:
    li r3, 0xb
    b @L_801CFCE8
@L_801CFB74:
    li r0, 0x11
    li r3, 0x2b
    stw r0, 0x4(r5)
    b @L_801CFCE8
@L_801CFB84:
    lwz r0, 0x0(r5)
    cmpwi r0, 0x4
    bge @L_801CFBCC
    cmpwi r0, 0x1
    bge @L_801CFB9C
    b @L_801CFBCC
@L_801CFB9C:
    li r3, 0xc
    li r0, 0x1
    stw r3, 0x4(r5)
    li r3, 0x2c
    lwz r4, -21580(r13)
    stw r0, 0x8(r4)
    lwz r4, -21580(r13)
    lbz r0, 0x40(r4)
    cmplwi r0, 0x0
    beq @L_801CFCE8
    li r3, 0x11
    b @L_801CFCE8
@L_801CFBCC:
    lis r4, 0x2
    lwz r3, 0x50(r5)
    subi r5, r4, 0x2000
    li r4, 0x0
    bl memset
    lwz r3, -21580(r13)
    li r4, 0x0
    lwz r3, 0x50(r3)
    addi r3, r3, 0x8
    bl fn_80128E38
    lwz r4, -21580(r13)
    li r5, 0x0
    li r0, -0x1
    li r3, 0x24
    stw r5, 0x2c(r4)
    lwz r4, -21580(r13)
    stw r0, 0x20(r4)
    b @L_801CFCE8
@L_801CFC14:
    li r3, 0x20
    b @L_801CFCE8
    cmpwi r31, -0x1
    beq @L_801CFCD8
    lwz r3, 0x2c(r5)
    cmpw r0, r3
    bgt @L_801CFCD8
    rlwinm r3, r31, 9, 0, 22
    lwz r4, 0x50(r5)
    addis r0, r3, 0x2
    li r3, 0x0
    add r28, r0, r4
    li r4, 0x1
    bl fn_80129280
    bl fn_80135CD0
    bl fn_80135C58
    mr r29, r4
    mr r30, r3
    subi r3, r28, 0x1ff8
    li r4, 0x1
    bl fn_80129280
    bl fn_80135CD0
    bl fn_80135C58
    xor r4, r29, r4
    xor r0, r30, r3
    or. r0, r4, r0
    bne @L_801CFCD8
    lwz r3, -21580(r13)
    lwz r0, 0x0(r3)
    cmpwi r0, 0x6
    bne @L_801CFC9C
    stw r31, 0x20(r3)
    li r3, 0x20
    b @L_801CFCE8
@L_801CFC9C:
    lwz r31, 0x50(r3)
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    li r0, 0x3bfa
    addi r5, r31, 0x4
    subi r4, r3, 0x4
    mtctr r0
@L_801CFCBC:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_801CFCBC
    li r3, 0x25
    b @L_801CFCE8
@L_801CFCD8:
    lwz r4, -21580(r13)
    li r0, 0x13
    li r3, 0x2b
    stw r0, 0x4(r4)
@L_801CFCE8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

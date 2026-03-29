    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    li r30, 0x0
    lis r3, lbl_803A6AB0@ha
    addi r31, r3, lbl_803A6AB0@l
    b @L_8004F8C4
    @L_8004F884:
    mr r3, r30
    bl fn_801FBFBC
    clrlwi r0, r30, 16
    lwz r4, 0x0(r3)
    mulli r5, r0, 0x14
    lwz r0, 0x4(r3)
    addi r30, r30, 0x1
    add r5, r31, r5
    stw r4, 0x0(r5)
    stw r0, 0x4(r5)
    lwz r4, 0x8(r3)
    lwz r0, 0xc(r3)
    stw r4, 0x8(r5)
    stw r0, 0xc(r5)
    lwz r0, 0x10(r3)
    stw r0, 0x10(r5)
    @L_8004F8C4:
    clrlwi r0, r30, 16
    cmplwi r0, 0x229
    blt @L_8004F884
    @L_8004F8D0:
    li r3, 0x87
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    cmpwi r3, -0x1
    bne @L_8004F96C
    li r3, 0x87
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r30, 0x0
    lis r3, lbl_803A6AB0@ha
    addi r31, r3, lbl_803A6AB0@l
    b @L_8004F958
    @L_8004F918:
    mr r3, r30
    bl fn_801FBFBC
    clrlwi r0, r30, 16
    addi r30, r30, 0x1
    mulli r0, r0, 0x14
    add r5, r31, r0
    lwz r4, 0x0(r5)
    lwz r0, 0x4(r5)
    stw r4, 0x0(r3)
    stw r0, 0x4(r3)
    lwz r4, 0x8(r5)
    lwz r0, 0xc(r5)
    stw r4, 0x8(r3)
    stw r0, 0xc(r3)
    lwz r0, 0x10(r5)
    stw r0, 0x10(r3)
    @L_8004F958:
    clrlwi r0, r30, 16
    cmplwi r0, 0x229
    blt @L_8004F918
    li r3, -0x1
    b @L_8004FE24
    @L_8004F96C:
    cmpwi r3, -0x2
    bne @L_8004F9B0
    li r3, 0x44
    li r4, 0x1
    bl fn_8010264C
    cmpwi r3, 0x0
    beq @L_8004F99C
    li r3, 0x44
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    b @L_8004F8D0
    @L_8004F99C:
    li r3, 0x44
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    b @L_8004FE10
    @L_8004F9B0:
    li r3, 0x87
    bl fn_801022B8
    cmpwi r3, 0xfd5
    beq @L_8004FC10
    bge @L_8004F9EC
    cmpwi r3, 0x5e5
    beq @L_8004FB10
    bge @L_8004F9E0
    cmpwi r3, 0x5e3
    beq @L_8004FA10
    bge @L_8004FA90
    b @L_8004F8D0
    @L_8004F9E0:
    cmpwi r3, 0x5e7
    bge @L_8004F8D0
    b @L_8004FB90
    @L_8004F9EC:
    cmpwi r3, 0xfea
    beq @L_8004FD10
    bge @L_8004FA04
    cmpwi r3, 0xfe9
    bge @L_8004FC90
    b @L_8004F8D0
    @L_8004FA04:
    cmpwi r3, 0xfec
    bge @L_8004F8D0
    b @L_8004FD90
    @L_8004FA10:
    li r3, 0x0
    li r4, 0x34
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x24
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8004FA88
    lwz r0, 0x24(r1)
    cmpwi r0, 0xc8
    ble @L_8004FA5C
    li r0, 0xc8
    stw r0, 0x24(r1)
    @L_8004FA5C:
    lwz r0, 0x24(r1)
    cmpwi r0, -0xc8
    bge @L_8004FA70
    li r0, -0xc8
    stw r0, 0x24(r1)
    @L_8004FA70:
    lwz r7, 0x24(r1)
    li r3, 0x0
    li r4, 0x34
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_8004FA88:
    bl fn_8001E200
    b @L_8004F8D0
    @L_8004FA90:
    li r3, 0x0
    li r4, 0x35
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x20
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8004FB08
    lwz r0, 0x20(r1)
    cmpwi r0, 0xc8
    ble @L_8004FADC
    li r0, 0xc8
    stw r0, 0x20(r1)
    @L_8004FADC:
    lwz r0, 0x20(r1)
    cmpwi r0, -0xc8
    bge @L_8004FAF0
    li r0, -0xc8
    stw r0, 0x20(r1)
    @L_8004FAF0:
    lwz r7, 0x20(r1)
    li r3, 0x0
    li r4, 0x35
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_8004FB08:
    bl fn_8001E200
    b @L_8004F8D0
    @L_8004FB10:
    li r3, 0x0
    li r4, 0x36
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x1c
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8004FB88
    lwz r0, 0x1c(r1)
    cmpwi r0, 0xc8
    ble @L_8004FB5C
    li r0, 0xc8
    stw r0, 0x1c(r1)
    @L_8004FB5C:
    lwz r0, 0x1c(r1)
    cmpwi r0, -0xc8
    bge @L_8004FB70
    li r0, -0xc8
    stw r0, 0x1c(r1)
    @L_8004FB70:
    lwz r7, 0x1c(r1)
    li r3, 0x0
    li r4, 0x36
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_8004FB88:
    bl fn_8001E200
    b @L_8004F8D0
    @L_8004FB90:
    li r3, 0x0
    li r4, 0x37
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x18
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8004FC08
    lwz r0, 0x18(r1)
    cmpwi r0, 0xc8
    ble @L_8004FBDC
    li r0, 0xc8
    stw r0, 0x18(r1)
    @L_8004FBDC:
    lwz r0, 0x18(r1)
    cmpwi r0, -0xc8
    bge @L_8004FBF0
    li r0, -0xc8
    stw r0, 0x18(r1)
    @L_8004FBF0:
    lwz r7, 0x18(r1)
    li r3, 0x0
    li r4, 0x37
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_8004FC08:
    bl fn_8001E200
    b @L_8004F8D0
    @L_8004FC10:
    li r3, 0x0
    li r4, 0x38
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x14
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8004FC88
    lwz r0, 0x14(r1)
    cmpwi r0, 0xc8
    ble @L_8004FC5C
    li r0, 0xc8
    stw r0, 0x14(r1)
    @L_8004FC5C:
    lwz r0, 0x14(r1)
    cmpwi r0, -0xc8
    bge @L_8004FC70
    li r0, -0xc8
    stw r0, 0x14(r1)
    @L_8004FC70:
    lwz r7, 0x14(r1)
    li r3, 0x0
    li r4, 0x38
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_8004FC88:
    bl fn_8001E200
    b @L_8004F8D0
    @L_8004FC90:
    li r3, 0x0
    li r4, 0x39
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x10
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8004FD08
    lwz r0, 0x10(r1)
    cmpwi r0, 0xc8
    ble @L_8004FCDC
    li r0, 0xc8
    stw r0, 0x10(r1)
    @L_8004FCDC:
    lwz r0, 0x10(r1)
    cmpwi r0, -0xc8
    bge @L_8004FCF0
    li r0, -0xc8
    stw r0, 0x10(r1)
    @L_8004FCF0:
    lwz r7, 0x10(r1)
    li r3, 0x0
    li r4, 0x39
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_8004FD08:
    bl fn_8001E200
    b @L_8004F8D0
    @L_8004FD10:
    li r3, 0x0
    li r4, 0x3a
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0xc
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8004FD88
    lwz r0, 0xc(r1)
    cmpwi r0, 0xc8
    ble @L_8004FD5C
    li r0, 0xc8
    stw r0, 0xc(r1)
    @L_8004FD5C:
    lwz r0, 0xc(r1)
    cmpwi r0, -0xc8
    bge @L_8004FD70
    li r0, -0xc8
    stw r0, 0xc(r1)
    @L_8004FD70:
    lwz r7, 0xc(r1)
    li r3, 0x0
    li r4, 0x3a
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_8004FD88:
    bl fn_8001E200
    b @L_8004F8D0
    @L_8004FD90:
    li r3, 0x0
    li r4, 0x3b
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x8
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8004FE08
    lwz r0, 0x8(r1)
    cmpwi r0, 0xc8
    ble @L_8004FDDC
    li r0, 0xc8
    stw r0, 0x8(r1)
    @L_8004FDDC:
    lwz r0, 0x8(r1)
    cmpwi r0, -0xc8
    bge @L_8004FDF0
    li r0, -0xc8
    stw r0, 0x8(r1)
    @L_8004FDF0:
    lwz r7, 0x8(r1)
    li r3, 0x0
    li r4, 0x3b
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_8004FE08:
    bl fn_8001E200
    b @L_8004F8D0
    @L_8004FE10:
    li r3, 0x87
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x1
    @L_8004FE24:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

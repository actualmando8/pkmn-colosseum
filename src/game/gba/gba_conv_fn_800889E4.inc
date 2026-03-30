    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r27, 0x2c(r1)
    mr r29, r3
    lis r4, -32729
    li r3, 0x0
    addi r30, r4, -2824
    li r4, 0xe
    bl fn_80129280
    mr r31, r3
    addis r27, r31, 0x1
    subi r28, r27, 0x3658
    bl fn_8006A7BC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80088A38
    addi r3, r30, 0x0
    addi r5, r30, 0x14
    li r4, 0x7b
    bl fn_80196E10
@L_80088A38:
    lwz r0, 0x0(r31)
    cmpwi r29, 0x0
    stw r0, -0x3674(r27)
    lwz r0, 0x4(r31)
    stw r0, -0x3670(r27)
    lwz r0, 0x8(r31)
    stw r0, -0x366c(r27)
    lwz r0, 0xc(r31)
    stw r0, -0x3668(r27)
    lwz r0, 0x10(r31)
    stw r0, -0x3664(r27)
    lwz r0, 0x14(r31)
    stw r0, -0x3660(r27)
    lwz r0, 0x18(r31)
    stw r0, -0x365c(r27)
    beq @L_80088B20
    lwz r0, -0x3674(r27)
    cmpwi r0, 0x1
    beq @L_80088A94
    addi r3, r30, 0x0
    addi r5, r30, 0x38
    li r4, 0x89
    bl fn_80196E10
@L_80088A94:
    addi r3, r1, 0x8
    bl fn_8012D32C
    addi r3, r1, 0x14
    bl fn_8012D2BC
    lfs f0, 0x8(r1)
    stfs f0, 0xc(r28)
    lfs f0, 0xc(r1)
    stfs f0, 0x10(r28)
    lfs f0, 0x10(r1)
    stfs f0, 0x14(r28)
    lfs f0, 0x14(r1)
    stfs f0, 0x18(r28)
    lfs f0, 0x18(r1)
    stfs f0, 0x1c(r28)
    lfs f0, 0x1c(r1)
    stfs f0, 0x20(r28)
    bl fn_800FF56C
    stw r3, 0x4(r28)
    bl fn_8011394C
    stw r3, 0x8(r28)
    li r0, 0x1
    li r3, 0xafc
    stb r0, 0x0(r28)
    bl fn_801906A0
    stw r3, 0x24(r28)
    li r3, 0xafd
    bl fn_801906A0
    stw r3, 0x28(r28)
    li r3, 0xb11
    bl fn_801906A0
    stw r3, 0x2c(r28)
    li r3, 0xde1
    bl fn_801906A0
    stw r3, 0x30(r28)
    b @L_80088B48
@L_80088B20:
    lwz r0, -0x3674(r27)
    cmpwi r0, 0x1
    bne @L_80088B38
    li r0, 0x1
    stb r0, 0x0(r28)
    b @L_80088B48
@L_80088B38:
    mr r3, r28
    li r4, 0x0
    li r5, 0x34
    bl memset
@L_80088B48:
    mr r3, r31
    bl fn_8006A7AC
    bl fn_8006A76C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80088B68
    li r30, 0x0
    b @L_80088C38
@L_80088B68:
    lis r3, 0x2
    subi r3, r3, 0x2030
    bl fn_80071104
    mr r29, r3
    bl fn_80128E24
    lis r5, 0x2
    mr r4, r3
    mr r3, r29
    subi r5, r5, 0x2030
    bl memcpy
    li r28, 0x0
    mr r27, r28
@L_80088B98:
    rlwinm r30, r28, 0, 16, 31
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    addi r0, r27, 0x59aa
    li r4, 0xe
    sthx r30, r3, r0
    li r3, 0x0
    bl fn_80129280
    addi r0, r27, 0x26
    addi r27, r27, 0x1660
    sthx r30, r3, r0
    addi r28, r28, 0x1
    cmplwi r28, 0x4
    blt @L_80088B98
    li r30, 0x0
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    stb r30, 0x1c(r3)
    li r3, 0x8ae
    li r4, 0x0
    bl fn_8019075C
    li r3, 0x6
    li r4, 0x2
    li r5, 0x0
    bl fn_801D0748
    mr r30, r3
    bl fn_80128E24
    lis r5, 0x2
    mr r4, r29
    subi r5, r5, 0x2030
    bl memcpy
    mr r3, r29
    bl fn_8007109C
    cmpwi r30, 0x4
    bne @L_80088C34
    li r30, 0x0
    b @L_80088C38
@L_80088C34:
    li r30, -0x1
@L_80088C38:
    cmpwi r30, 0x0
    bge @L_80088C48
    mr r3, r31
    bl fn_8006A79C
@L_80088C48:
    mr r3, r30
    lmw r27, 0x2c(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr

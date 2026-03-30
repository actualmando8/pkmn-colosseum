    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r27, 0x3c(r1)
    mr r27, r3
    lhz r0, 0x32(r3)
    lwz r3, 0x2c(r3)
    mulli r0, r0, 0xd4
    lwz r31, 0x24(r27)
    add r29, r3, r0
    bl fn_800D2248
    bl fn_800D2584
    addi r4, r1, 0x1c
    mr r30, r3
    bl fn_800D1F84
    addi r5, r1, 0x8
    mr r3, r30
    mr r6, r5
    addi r4, r1, 0xc
    mr r7, r5
    bl fn_800D1FDC
    lwz r0, 0x10(r27)
    cmpwi r0, 0x1
    beq @L_801D7C44
    bge @L_801D7C10
    cmpwi r0, -0x1
    beq @L_801D7C2C
    bge @L_801D7C38
    cmpwi r0, -0x2
    bge @L_801D7C20
    b @L_801D7C64
@L_801D7C10:
    cmpwi r0, 0x3
    beq @L_801D7C5C
    bge @L_801D7C64
    b @L_801D7C50
@L_801D7C20:
    lfs f0, -21384(r2)
    stfs f0, 0x8(r1)
    b @L_801D7C64
@L_801D7C2C:
    lfs f0, -21360(r2)
    stfs f0, 0x8(r1)
    b @L_801D7C64
@L_801D7C38:
    lfs f0, -21396(r2)
    stfs f0, 0x8(r1)
    b @L_801D7C64
@L_801D7C44:
    lfs f0, -21356(r2)
    stfs f0, 0x8(r1)
    b @L_801D7C64
@L_801D7C50:
    lfs f0, -21352(r2)
    stfs f0, 0x8(r1)
    b @L_801D7C64
@L_801D7C5C:
    lfs f0, -21348(r2)
    stfs f0, 0x8(r1)
@L_801D7C64:
    li r3, 0x1
    li r4, 0x6
    li r5, 0x1
    bl fn_800DA4C4
    li r3, 0x1
    li r4, 0x1
    li r5, 0x1
    bl fn_800DA2BC
    li r3, 0x0
    li r4, 0x7
    li r5, 0x0
    bl fn_800DA1E8
    li r3, 0x0
    bl fn_800DA028
    addi r3, r1, 0x1c
    bl fn_800E008C
    lfs f0, 0xc(r1)
    lfs f3, -21344(r2)
    lfs f2, 0x8(r1)
    fmuls f4, f0, f1
    lfs f0, -21396(r2)
    fmuls f1, f3, f2
    fdivs f1, f1, f4
    fcmpo cr0, f1, f0
    ble @L_801D7CCC
    fmr f1, f0
@L_801D7CCC:
    lfs f0, -21340(r2)
    li r4, 0x5
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x28(r1)
    lwz r3, 0x2c(r1)
    bl fn_800B944C
    li r3, 0x1
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    lis r3, -32719
    addi r3, r3, 17976
    bl fn_800D7820
    li r3, 0x0
    bl fn_800D6A00
    lis r3, -32698
    addi r30, r3, 29688
    lwz r0, 0x18(r30)
    cmpwi r0, 0x0
    bge @L_801D7DB8
    li r3, 0x10
    bl fn_800D67BC
    lis r3, -32698
    li r27, 0x0
    addi r30, r3, 29688
@L_801D7D34:
    lwz r4, 0x4c(r29)
    mr r3, r31
    bl fn_800EE150
    mr. r28, r3
    beq @L_801D7D64
    addi r4, r1, 0x10
    li r5, 0x0
    li r6, 0x0
    bl fn_800EE3BC
    mr r3, r28
    bl fn_800EE828
    b @L_801D7D78
@L_801D7D64:
    addi r3, r1, 0x10
    bl fn_800E0204
    lwz r0, 0xc(r30)
    ori r0, r0, 0x2
    stw r0, 0xc(r30)
@L_801D7D78:
    lfs f1, 0x10(r1)
    lfs f2, 0x14(r1)
    lfs f3, 0x18(r1)
    bl fn_800D6680
    li r3, 0x0
    li r4, 0xff
    li r5, 0x0
    li r6, 0x0
    li r7, 0xdc
    bl fn_800D5CB8
    addi r27, r27, 0x1
    addi r29, r29, 0x4
    cmpwi r27, 0x10
    blt @L_801D7D34
    bl fn_800D6728
    b @L_801D7E44
@L_801D7DB8:
    li r3, 0x1
    bl fn_800D67BC
    lwz r0, 0x18(r30)
    mr r3, r31
    rlwinm r0, r0, 2, 0, 29
    add r4, r29, r0
    lwz r4, 0x4c(r4)
    bl fn_800EE150
    mr. r29, r3
    beq @L_801D7DFC
    addi r4, r1, 0x10
    li r5, 0x0
    li r6, 0x0
    bl fn_800EE3BC
    mr r3, r29
    bl fn_800EE828
    b @L_801D7E18
@L_801D7DFC:
    addi r3, r1, 0x10
    bl fn_800E0204
    lis r3, -32698
    addi r3, r3, 29688
    lwz r0, 0xc(r3)
    ori r0, r0, 0x2
    stw r0, 0xc(r3)
@L_801D7E18:
    lfs f1, 0x10(r1)
    lfs f2, 0x14(r1)
    lfs f3, 0x18(r1)
    bl fn_800D6680
    li r3, 0x0
    li r4, 0xff
    li r5, 0x0
    li r6, 0x0
    li r7, 0xdc
    bl fn_800D5CB8
    bl fn_800D6728
@L_801D7E44:
    lmw r27, 0x3c(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr

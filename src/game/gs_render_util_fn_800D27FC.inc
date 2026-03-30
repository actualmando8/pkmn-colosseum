    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lwz r0, -23984(r13)
    lwz r31, -23988(r13)
    mtctr r0
    cmplwi r0, 0x0
    ble @L_800D2838
@L_800D2820:
    lbz r0, 0x0(r31)
    cmplwi r0, 0x0
    bne @L_800D2830
    b @L_800D283C
@L_800D2830:
    addi r31, r31, 0x128
    bdnz @L_800D2820
@L_800D2838:
    li r31, 0x0
@L_800D283C:
    cmplwi r31, 0x0
    bne @L_800D284C
    li r3, 0x0
    b @L_800D298C
@L_800D284C:
    stw r3, 0x8(r31)
    lwz r3, 0x8(r31)
    lwz r3, 0x0(r3)
    bl fn_80193F44
    stw r3, 0xc(r31)
    li r5, 0x1
    li r4, 0x0
    stb r5, 0x0(r31)
    stb r4, 0x4(r31)
    lwz r3, 0x8(r31)
    lwz r0, 0x4(r3)
    cmplwi r0, 0x0
    beq @L_800D294C
    stb r5, 0x3(r31)
    lfs f0, -27920(r2)
    stfs f0, 0x118(r31)
    stw r5, 0x10c(r31)
    stb r4, 0x124(r31)
    stw r4, 0x110(r31)
    b @L_800D28A8
@L_800D289C:
    lwz r3, 0x110(r31)
    addi r0, r3, 0x1
    stw r0, 0x110(r31)
@L_800D28A8:
    lwz r3, 0x8(r31)
    lwz r0, 0x110(r31)
    lwz r3, 0x4(r3)
    rlwinm r0, r0, 2, 0, 29
    lwzx r0, r3, r0
    cmplwi r0, 0x0
    bne @L_800D289C
    lbz r0, 0x3(r31)
    cmplwi r0, 0x0
    beq @L_800D2950
    lwz r3, 0xc(r31)
    bl fn_80196BB8
    lwz r0, 0x110(r31)
    cmplwi r0, 0x0
    blt @L_800D2950
    li r0, 0x0
    stw r0, 0x114(r31)
    lwz r3, 0x8(r31)
    lwz r0, 0x114(r31)
    lwz r4, 0x4(r3)
    rlwinm r0, r0, 2, 0, 29
    lwz r3, 0xc(r31)
    lwzx r4, r4, r0
    bl fn_80196B10
    lwz r3, 0xc(r31)
    lfs f1, -27912(r2)
    bl fn_80196698
    lfs f0, -27912(r2)
    lis r3, -32755
    addi r6, r3, 11076
    li r4, 0x2
    stfs f0, -23976(r13)
    lis r3, 0x1
    subi r5, r3, 0x1
    li r7, 0x0
    lwz r3, 0xc(r31)
    crxor 6, 6, 6
    bl fn_801C028C
    lfs f0, -23976(r13)
    stfs f0, 0x120(r31)
    b @L_800D2950
@L_800D294C:
    stb r4, 0x3(r31)
@L_800D2950:
    li r3, 0x1
    li r0, 0x0
    stb r3, 0x0(r31)
    addi r3, r31, 0x70
    stb r0, 0x1(r31)
    lwz r4, 0xc(r31)
    lwz r4, 0x24(r4)
    addi r4, r4, 0xc
    bl fn_800E01D0
    lwz r4, 0xc(r31)
    addi r3, r31, 0x100
    lwz r4, 0x28(r4)
    addi r4, r4, 0xc
    bl fn_800E01D0
    mr r3, r31
@L_800D298C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stw r31, 0x5c(r1)
    stw r30, 0x58(r1)
    stw r29, 0x54(r1)
    mr r29, r3
    lwz r3, 0x6c(r3)
    cmpwi r3, 0x2
    bne @L_801D7E88
    li r3, 0x1
    b @L_801D81B0
@L_801D7E88:
    rlwinm. r0, r4, 0, 24, 31
    bne @L_801D7EA0
    cmpwi r3, 0x1
    beq @L_801D7EA0
    li r3, 0x0
    b @L_801D81B0
@L_801D7EA0:
    lwz r3, 0xb0(r29)
    lwz r0, 0x4(r29)
    lwz r3, 0x3c(r3)
    cmpwi r0, 0x4
    lwz r31, 0x24(r3)
    beq @L_801D80CC
    bge @L_801D7ED4
    cmpwi r0, 0x2
    beq @L_801D7EE4
    bge @L_801D8098
    cmpwi r0, 0x0
    bge @L_801D81A4
    b @L_801D8184
@L_801D7ED4:
    cmpwi r0, 0x6
    beq @L_801D81A4
    bge @L_801D8184
    b @L_801D8160
@L_801D7EE4:
    lwz r30, 0xa4(r29)
    cmplwi r30, 0x0
    beq @L_801D81A4
    mr r3, r30
    bl fn_800EC96C
    mr r3, r30
    bl fn_800EC1D4
    mr r3, r30
    li r4, 0x0
    bl fn_800E4014
    lwz r0, 0x94(r29)
    cmpwi r0, 0x0
    beq @L_801D8028
    mr r3, r30
    addi r4, r1, 0x44
    bl fn_800E3D98
    mr r3, r30
    addi r4, r1, 0x38
    bl fn_800E3D6C
    mr r3, r30
    addi r4, r1, 0x2c
    bl fn_800E3D40
    mr r3, r31
    addi r4, r1, 0x20
    bl fn_800E3D98
    addi r3, r1, 0x20
    addi r5, r1, 0x44
    mr r4, r3
    bl fn_800E0168
    lwz r0, 0xa0(r29)
    cmpwi r0, 0x0
    beq @L_801D7F70
    mr r3, r31
    li r4, 0x0
    bl fn_800E465C
@L_801D7F70:
    mr r3, r31
    addi r4, r1, 0x44
    bl fn_800E43A4
    mr r3, r31
    addi r4, r1, 0x38
    bl fn_800E4170
    lis r4, -32713
    mr r3, r31
    addi r4, r4, 9660
    bl fn_800E407C
    lwz r0, 0x9c(r29)
    rlwinm r0, r0, 0, 27, 27
    cmpwi r0, 0x10
    bne @L_801D7FEC
    mr r3, r31
    bl fn_800E3C5C
    mr r5, r3
    lfs f1, -21336(r2)
    addi r3, r1, 0x14
    addi r4, r5, 0x10
    addi r5, r5, 0x1c
    bl fn_800E090C
    addi r3, r1, 0x44
    addi r5, r1, 0x14
    mr r4, r3
    bl fn_800E0168
    mr r3, r31
    addi r4, r1, 0x44
    bl fn_800E43A4
    mr r3, r31
    bl fn_800E6DCC
@L_801D7FEC:
    lwz r0, 0x9c(r29)
    rlwinm r0, r0, 0, 28, 28
    cmpwi r0, 0x8
    bne @L_801D8064
    mr r3, r31
    addi r4, r1, 0x20
    li r5, 0x0
    li r6, 0x0
    bl fn_800E732C
    lwz r3, 0xb0(r29)
    addi r4, r1, 0x20
    lwz r3, 0x3c(r3)
    addi r3, r3, 0x5c
    bl fn_800E01D0
    b @L_801D8064
@L_801D8028:
    lwz r0, 0xa0(r29)
    cmpwi r0, 0x0
    beq @L_801D8064
    mr r3, r30
    li r4, 0x0
    bl fn_800E465C
    lwz r0, 0x1c(r29)
    rlwinm r0, r0, 0, 31, 31
    cmpwi r0, 0x1
    beq @L_801D8064
    lwz r0, 0x20(r29)
    cmpwi r0, 0x10
    bne @L_801D8064
    mr r3, r31
    bl fn_800E6B20
@L_801D8064:
    lis r4, -32713
    mr r3, r30
    addi r4, r4, 9648
    bl fn_800E43A4
    lis r4, -32713
    mr r3, r30
    addi r4, r4, 9648
    bl fn_800E4170
    lis r4, -32713
    mr r3, r30
    addi r4, r4, 9660
    bl fn_800E407C
    b @L_801D81A4
@L_801D8098:
    lwz r0, 0x90(r29)
    cmpwi r0, 0x0
    beq @L_801D81A4
    lwz r0, 0x1c(r29)
    rlwinm r0, r0, 0, 31, 31
    cmpwi r0, 0x1
    beq @L_801D81A4
    lwz r0, 0x20(r29)
    cmpwi r0, 0x10
    bne @L_801D81A4
    mr r3, r31
    bl fn_800E6B20
    b @L_801D81A4
@L_801D80CC:
    lwz r3, 0x78(r29)
    cmpwi r3, 0x0
    beq @L_801D8154
    lwz r0, 0x88(r29)
    cmpwi r0, 0x6
    beq @L_801D80F4
    bge @L_801D8154
    cmpwi r0, 0x0
    beq @L_801D8134
    b @L_801D8154
@L_801D80F4:
    bl fn_8013151C
    mr r30, r3
    lwz r0, 0xa8(r3)
    cmpwi r0, 0x0
    beq @L_801D8154
    mr r3, r31
    addi r4, r1, 0x8
    bl fn_800E3D98
    addi r3, r1, 0x8
    addi r5, r30, 0x48
    mr r4, r3
    bl fn_800E0168
    mr r3, r31
    addi r4, r1, 0x8
    bl fn_800E43A4
    b @L_801D8154
@L_801D8134:
    bl fn_8013151C
    mr r4, r3
    lbz r0, 0x4d(r3)
    cmplwi r0, 0x0
    beq @L_801D8154
    mr r3, r31
    addi r4, r4, 0x44
    bl fn_800E5B68
@L_801D8154:
    lwz r3, 0x78(r29)
    bl fn_80131010
    b @L_801D81A4
@L_801D8160:
    lwz r0, 0x7c(r29)
    rlwinm r0, r0, 0, 31, 31
    cmpwi r0, 0x1
    bne @L_801D8178
    li r0, 0x0
    stw r0, -21528(r13)
@L_801D8178:
    lwz r3, 0x78(r29)
    bl fn_80166B18
    b @L_801D81A4
@L_801D8184:
    lis r3, -32728
    addi r3, r3, -27256
    crxor 6, 6, 6
    bl fn_800DD970
    li r0, -0x1
    li r3, 0x0
    stw r0, 0x6c(r29)
    b @L_801D81B0
@L_801D81A4:
    li r0, 0x2
    li r3, 0x1
    stw r0, 0x6c(r29)
@L_801D81B0:
    lwz r0, 0x64(r1)
    lwz r31, 0x5c(r1)
    lwz r30, 0x58(r1)
    lwz r29, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr

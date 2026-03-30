    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    li r31, 0x0
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
@L_801BACAC:
    li r4, -0x1
    li r5, -0x1
    li r6, -0x1
    li r7, -0x1
    li r30, 0x0
    b @L_801BAD28
@L_801BACC4:
    rlwinm r0, r30, 3, 0, 28
    add r3, r29, r0
    lbz r0, 0x34(r3)
    cmpwi r0, 0x3
    beq @L_801BACF0
    bge @L_801BACF4
    cmpwi r0, 0x2
    bge @L_801BACE8
    b @L_801BACF4
@L_801BACE8:
    mr r4, r30
    b @L_801BACF4
@L_801BACF0:
    mr r6, r30
@L_801BACF4:
    rlwinm r0, r30, 3, 0, 28
    add r3, r29, r0
    lbz r0, 0x54(r3)
    cmpwi r0, 0x3
    beq @L_801BAD20
    bge @L_801BAD24
    cmpwi r0, 0x2
    bge @L_801BAD18
    b @L_801BAD24
@L_801BAD18:
    mr r5, r30
    b @L_801BAD24
@L_801BAD20:
    mr r7, r30
@L_801BAD24:
    addi r30, r30, 0x1
@L_801BAD28:
    cmpwi r30, 0x4
    blt @L_801BACC4
    cmpwi r4, -0x1
    bne @L_801BAD50
    cmpwi r5, -0x1
    bne @L_801BAD50
    li r3, 0x0
    li r0, 0xff
    stw r3, 0x74(r29)
    stb r0, 0x1e(r29)
@L_801BAD50:
    cmpwi r6, -0x1
    bne @L_801BAD70
    cmpwi r7, -0x1
    bne @L_801BAD70
    li r3, 0xff
    li r0, 0xff
    stb r3, 0x78(r29)
    stb r0, 0x1f(r29)
@L_801BAD70:
    lbz r3, 0x19(r29)
    li r0, 0x0
    cmplwi r3, 0xff
    beq @L_801BADB0
    lbz r3, 0x19(r29)
    cmplwi r3, 0xe
    beq @L_801BADB0
    lbz r3, 0x19(r29)
    cmplwi r3, 0xf
    beq @L_801BADB0
    lbz r3, 0x19(r29)
    cmplwi r3, 0x0
    beq @L_801BADB0
    lbz r3, 0x19(r29)
    cmplwi r3, 0x1
    bne @L_801BB128
@L_801BADB0:
    lbz r3, 0xd(r29)
    cmplwi r3, 0xff
    beq @L_801BAE1C
    lwz r3, 0x8(r29)
    cmpwi r3, 0x0
    bne @L_801BAE1C
    li r0, 0xff
    li r30, 0x0
    stb r0, 0xd(r29)
    b @L_801BAE10
@L_801BADD8:
    rlwinm r3, r30, 3, 0, 28
    rlwinm r0, r30, 3, 0, 28
    add r3, r29, r3
    add r4, r29, r0
    lwz r3, 0x38(r3)
    lbz r4, 0x35(r4)
    bl fn_801B750C
    rlwinm r4, r30, 3, 0, 28
    lwz r3, -31616(r13)
    lwz r0, -31612(r13)
    add r4, r29, r4
    addi r30, r30, 0x1
    stw r3, 0x34(r4)
    stw r0, 0x38(r4)
@L_801BAE10:
    cmpwi r30, 0x4
    blt @L_801BADD8
    li r0, 0x1
@L_801BAE1C:
    lbz r3, 0xd(r29)
    cmplwi r3, 0xf
    bgt @L_801BB128
    lis r4, -32713
    rlwinm r3, r3, 2, 0, 29
    addi r4, r4, -11344
    lwzx r4, r4, r3
    mtctr r4
    bctr
    lbz r3, 0x45(r29)
    cmplwi r3, 0x7
    bne @L_801BAEBC
    lbz r3, 0x3d(r29)
    cmplwi r3, 0x7
    beq @L_801BAE78
    lwz r3, 0x40(r29)
    lbz r4, 0x3d(r29)
    bl fn_801B750C
    lwz r4, -31616(r13)
    li r0, 0x1
    lwz r3, -31612(r13)
    stw r4, 0x3c(r29)
    stw r3, 0x40(r29)
@L_801BAE78:
    lbz r3, 0xd(r29)
    cmplwi r3, 0x0
    bne @L_801BAEBC
    lbz r3, 0x4d(r29)
    cmplwi r3, 0x7
    bne @L_801BAEBC
    lwz r5, 0x34(r29)
    li r3, 0x1
    lwz r4, 0x38(r29)
    li r0, 0x1
    stw r5, 0x4c(r29)
    stw r4, 0x50(r29)
    lwz r5, -31616(r13)
    lwz r4, -31612(r13)
    stw r5, 0x34(r29)
    stw r4, 0x38(r29)
    stb r3, 0xe(r29)
@L_801BAEBC:
    lbz r3, 0x45(r29)
    cmplwi r3, 0x8
    bne @L_801BAF40
    lbz r3, 0x35(r29)
    cmplwi r3, 0x7
    beq @L_801BAEF4
    lwz r3, 0x38(r29)
    lbz r4, 0x35(r29)
    bl fn_801B750C
    lwz r4, -31616(r13)
    li r0, 0x1
    lwz r3, -31612(r13)
    stw r4, 0x34(r29)
    stw r3, 0x38(r29)
@L_801BAEF4:
    lbz r3, 0xd(r29)
    cmplwi r3, 0x0
    bne @L_801BAF40
    lbz r3, 0x4d(r29)
    cmplwi r3, 0x7
    bne @L_801BAF40
    lwz r4, 0x3c(r29)
    li r0, 0x1
    lwz r3, 0x40(r29)
    stw r4, 0x4c(r29)
    stw r3, 0x50(r29)
    lwz r4, -31616(r13)
    lwz r3, -31612(r13)
    stw r4, 0x3c(r29)
    stw r3, 0x40(r29)
    lwz r4, -31616(r13)
    lwz r3, -31612(r13)
    stw r4, 0x44(r29)
    stw r3, 0x48(r29)
@L_801BAF40:
    lbz r3, 0x35(r29)
    cmplwi r3, 0x7
    bne @L_801BAF8C
    lbz r3, 0x3d(r29)
    cmplwi r3, 0x8
    bne @L_801BAF8C
    lwz r4, 0x44(r29)
    li r0, 0x1
    lwz r3, 0x48(r29)
    stw r4, 0x34(r29)
    stw r3, 0x38(r29)
    lwz r4, -31616(r13)
    lwz r3, -31612(r13)
    stw r4, 0x3c(r29)
    stw r3, 0x40(r29)
    lwz r4, -31616(r13)
    lwz r3, -31612(r13)
    stw r4, 0x44(r29)
    stw r3, 0x48(r29)
@L_801BAF8C:
    lbz r3, 0x35(r29)
    cmplwi r3, 0x7
    bne @L_801BB128
    lbz r3, 0x3d(r29)
    cmplwi r3, 0x7
    bne @L_801BB128
    lbz r3, 0x4d(r29)
    cmplwi r3, 0x7
    bne @L_801BB128
    lbz r3, 0xf(r29)
    cmplwi r3, 0x0
    bne @L_801BB128
    li r0, 0xff
    stb r0, 0xd(r29)
    lwz r3, 0x48(r29)
    lbz r4, 0x45(r29)
    bl fn_801B750C
    lwz r4, -31616(r13)
    li r0, 0x1
    lwz r3, -31612(r13)
    stw r4, 0x44(r29)
    stw r3, 0x48(r29)
    b @L_801BB128
    lbz r3, 0x45(r29)
    cmplwi r3, 0x7
    bne @L_801BB03C
    li r0, 0x0
    stb r0, 0xd(r29)
    lwz r3, 0x38(r29)
    lbz r4, 0x35(r29)
    bl fn_801B750C
    lwz r3, -31616(r13)
    lwz r0, -31612(r13)
    stw r3, 0x34(r29)
    stw r0, 0x38(r29)
    lwz r3, 0x40(r29)
    lbz r4, 0x3d(r29)
    bl fn_801B750C
    lwz r4, -31616(r13)
    li r0, 0x1
    lwz r3, -31612(r13)
    stw r4, 0x3c(r29)
    stw r3, 0x40(r29)
    b @L_801BB128
@L_801BB03C:
    lbz r3, 0x35(r29)
    cmplwi r3, 0x7
    bne @L_801BB128
    li r0, 0x0
    stb r0, 0xd(r29)
    lwz r3, 0x40(r29)
    lbz r4, 0x3d(r29)
    bl fn_801B750C
    lwz r3, -31616(r13)
    lwz r0, -31612(r13)
    stw r3, 0x3c(r29)
    stw r0, 0x40(r29)
    lwz r3, 0x48(r29)
    lbz r4, 0x45(r29)
    bl fn_801B750C
    lwz r4, -31616(r13)
    li r0, 0x1
    lwz r3, -31612(r13)
    stw r4, 0x44(r29)
    stw r3, 0x48(r29)
    b @L_801BB128
    lbz r3, 0x45(r29)
    cmplwi r3, 0x7
    bne @L_801BB0E4
    li r0, 0x0
    stb r0, 0xd(r29)
    lwz r3, 0x38(r29)
    lbz r4, 0x35(r29)
    bl fn_801B750C
    lwz r3, -31616(r13)
    lwz r0, -31612(r13)
    stw r3, 0x34(r29)
    stw r0, 0x38(r29)
    lwz r3, 0x40(r29)
    lbz r4, 0x3d(r29)
    bl fn_801B750C
    lwz r4, -31616(r13)
    li r0, 0x1
    lwz r3, -31612(r13)
    stw r4, 0x3c(r29)
    stw r3, 0x40(r29)
    b @L_801BB128
@L_801BB0E4:
    lbz r3, 0x35(r29)
    cmplwi r3, 0x7
    bne @L_801BB128
    lbz r3, 0x3d(r29)
    cmplwi r3, 0x7
    bne @L_801BB128
    li r3, 0x0
    li r0, 0x1
    stb r3, 0xd(r29)
    lwz r4, 0x44(r29)
    lwz r3, 0x48(r29)
    stw r4, 0x34(r29)
    stw r3, 0x38(r29)
    lwz r4, -31616(r13)
    lwz r3, -31612(r13)
    stw r4, 0x44(r29)
    stw r3, 0x48(r29)
@L_801BB128:
    lbz r3, 0x19(r29)
    cmplwi r3, 0xff
    beq @L_801BB194
    lwz r3, 0x14(r29)
    cmpwi r3, 0x0
    bne @L_801BB194
    li r0, 0xff
    li r30, 0x0
    stb r0, 0x19(r29)
    b @L_801BB188
@L_801BB150:
    rlwinm r3, r30, 3, 0, 28
    rlwinm r0, r30, 3, 0, 28
    add r3, r29, r3
    add r4, r29, r0
    lwz r3, 0x58(r3)
    lbz r4, 0x55(r4)
    bl fn_801B750C
    rlwinm r4, r30, 3, 0, 28
    lwz r3, -31616(r13)
    lwz r0, -31612(r13)
    add r4, r29, r4
    addi r30, r30, 0x1
    stw r3, 0x54(r4)
    stw r0, 0x58(r4)
@L_801BB188:
    cmpwi r30, 0x4
    blt @L_801BB150
    li r0, 0x1
@L_801BB194:
    lbz r3, 0x19(r29)
    cmpwi r3, 0xe
    beq @L_801BB350
    bge @L_801BB1C4
    cmpwi r3, 0x2
    bge @L_801BB1B8
    cmpwi r3, 0x0
    bge @L_801BB1D0
    b @L_801BB490
@L_801BB1B8:
    cmpwi r3, 0x8
    bge @L_801BB2FC
    b @L_801BB490
@L_801BB1C4:
    cmpwi r3, 0x10
    bge @L_801BB490
    b @L_801BB3F8
@L_801BB1D0:
    lbz r3, 0x65(r29)
    cmplwi r3, 0x7
    bne @L_801BB244
    lbz r3, 0x5d(r29)
    cmplwi r3, 0x7
    beq @L_801BB208
    lwz r3, 0x60(r29)
    lbz r4, 0x5d(r29)
    bl fn_801B750C
    lwz r4, -31616(r13)
    li r0, 0x1
    lwz r3, -31612(r13)
    stw r4, 0x5c(r29)
    stw r3, 0x60(r29)
@L_801BB208:
    lbz r3, 0x19(r29)
    cmplwi r3, 0x0
    bne @L_801BB244
    lbz r3, 0x6d(r29)
    cmplwi r3, 0x7
    bne @L_801BB244
    lwz r4, 0x54(r29)
    li r0, 0x1
    lwz r3, 0x58(r29)
    stw r4, 0x6c(r29)
    stw r3, 0x70(r29)
    lwz r4, -31616(r13)
    lwz r3, -31612(r13)
    stw r4, 0x54(r29)
    stw r3, 0x58(r29)
@L_801BB244:
    lbz r3, 0x65(r29)
    cmplwi r3, 0x8
    bne @L_801BB2C8
    lbz r3, 0x55(r29)
    cmplwi r3, 0x7
    beq @L_801BB27C
    lwz r3, 0x58(r29)
    lbz r4, 0x55(r29)
    bl fn_801B750C
    lwz r4, -31616(r13)
    li r0, 0x1
    lwz r3, -31612(r13)
    stw r4, 0x54(r29)
    stw r3, 0x58(r29)
@L_801BB27C:
    lbz r3, 0x19(r29)
    cmplwi r3, 0x0
    bne @L_801BB2C8
    lbz r3, 0x6d(r29)
    cmplwi r3, 0x7
    bne @L_801BB2C8
    lwz r4, 0x5c(r29)
    li r0, 0x1
    lwz r3, 0x60(r29)
    stw r4, 0x6c(r29)
    stw r3, 0x70(r29)
    lwz r4, -31616(r13)
    lwz r3, -31612(r13)
    stw r4, 0x5c(r29)
    stw r3, 0x60(r29)
    lwz r4, -31616(r13)
    lwz r3, -31612(r13)
    stw r4, 0x64(r29)
    stw r3, 0x68(r29)
@L_801BB2C8:
    lbz r3, 0x55(r29)
    cmplwi r3, 0x7
    bne @L_801BB490
    lbz r3, 0x5d(r29)
    cmplwi r3, 0x7
    bne @L_801BB490
    lbz r3, 0x6d(r29)
    cmplwi r3, 0x7
    bne @L_801BB490
    li r3, 0xff
    li r0, 0x1
    stb r3, 0x19(r29)
    b @L_801BB490
@L_801BB2FC:
    lbz r3, 0x65(r29)
    cmplwi r3, 0x7
    bne @L_801BB490
    li r0, 0x0
    stb r0, 0x19(r29)
    lwz r3, 0x58(r29)
    lbz r4, 0x55(r29)
    bl fn_801B750C
    lwz r3, -31616(r13)
    lwz r0, -31612(r13)
    stw r3, 0x54(r29)
    stw r0, 0x58(r29)
    lwz r3, 0x60(r29)
    lbz r4, 0x5d(r29)
    bl fn_801B750C
    lwz r4, -31616(r13)
    li r0, 0x1
    lwz r3, -31612(r13)
    stw r4, 0x5c(r29)
    stw r3, 0x60(r29)
    b @L_801BB490
@L_801BB350:
    lbz r3, 0x65(r29)
    cmplwi r3, 0x7
    bne @L_801BB3A4
    li r0, 0x0
    stb r0, 0x19(r29)
    lwz r3, 0x58(r29)
    lbz r4, 0x55(r29)
    bl fn_801B750C
    lwz r3, -31616(r13)
    lwz r0, -31612(r13)
    stw r3, 0x54(r29)
    stw r0, 0x58(r29)
    lwz r3, 0x60(r29)
    lbz r4, 0x5d(r29)
    bl fn_801B750C
    lwz r4, -31616(r13)
    li r0, 0x1
    lwz r3, -31612(r13)
    stw r4, 0x5c(r29)
    stw r3, 0x60(r29)
    b @L_801BB490
@L_801BB3A4:
    lbz r3, 0x55(r29)
    cmplwi r3, 0x7
    bne @L_801BB490
    li r0, 0x0
    stb r0, 0x19(r29)
    lwz r3, 0x60(r29)
    lbz r4, 0x5d(r29)
    bl fn_801B750C
    lwz r3, -31616(r13)
    lwz r0, -31612(r13)
    stw r3, 0x5c(r29)
    stw r0, 0x60(r29)
    lwz r3, 0x68(r29)
    lbz r4, 0x65(r29)
    bl fn_801B750C
    lwz r4, -31616(r13)
    li r0, 0x1
    lwz r3, -31612(r13)
    stw r4, 0x64(r29)
    stw r3, 0x68(r29)
    b @L_801BB490
@L_801BB3F8:
    lbz r3, 0x65(r29)
    cmplwi r3, 0x7
    bne @L_801BB44C
    li r0, 0x0
    stb r0, 0x19(r29)
    lwz r3, 0x58(r29)
    lbz r4, 0x55(r29)
    bl fn_801B750C
    lwz r3, -31616(r13)
    lwz r0, -31612(r13)
    stw r3, 0x54(r29)
    stw r0, 0x58(r29)
    lwz r3, 0x60(r29)
    lbz r4, 0x5d(r29)
    bl fn_801B750C
    lwz r4, -31616(r13)
    li r0, 0x1
    lwz r3, -31612(r13)
    stw r4, 0x5c(r29)
    stw r3, 0x60(r29)
    b @L_801BB490
@L_801BB44C:
    lbz r3, 0x55(r29)
    cmplwi r3, 0x7
    bne @L_801BB490
    lbz r3, 0x5d(r29)
    cmplwi r3, 0x7
    bne @L_801BB490
    li r3, 0x0
    li r0, 0x1
    stb r3, 0x19(r29)
    lwz r4, 0x64(r29)
    lwz r3, 0x68(r29)
    stw r4, 0x54(r29)
    stw r3, 0x58(r29)
    lwz r4, -31616(r13)
    lwz r3, -31612(r13)
    stw r4, 0x64(r29)
    stw r3, 0x68(r29)
@L_801BB490:
    cmpwi r0, 0x0
    beq @L_801BB49C
    li r31, 0x1
@L_801BB49C:
    cmpwi r0, 0x0
    bne @L_801BACAC
    lwz r0, 0x24(r1)
    mr r3, r31
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

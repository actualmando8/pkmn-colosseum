    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r28, 0x30(r1)
    mr r30, r3
    mr r31, r5
@L_80265054:
    mr r4, r30
    mr r5, r31
    li r3, 0xf
    bl fn_801F02AC
    mr r28, r3
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802650F8
    mr r4, r28
    mr r5, r31
    li r3, 0x2
    bl fn_801F02AC
    cmplwi r3, 0x0
    bne @L_80265098
    li r3, 0x0
    b @L_802650FC
@L_80265098:
    li r4, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_801F76B8
    rlwinm r29, r3, 0, 16, 31
    mr r3, r28
    mr r4, r31
    bl fn_801F0134
    rlwinm. r0, r3, 0, 16, 31
    bne @L_802650C8
    li r3, 0x0
    b @L_802650FC
@L_802650C8:
    bl fn_801F0234
    bl fn_801F0204
    cmpwi r3, 0x0
    bge @L_802650E0
    li r3, 0x0
    b @L_802650FC
@L_802650E0:
    mr r4, r29
    rlwinm r6, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x3
    bl fn_801F76B8
    b @L_802650FC
@L_802650F8:
    li r3, 0x0
@L_802650FC:
    stw r3, 0xc(r1)
    mr r4, r30
    mr r5, r31
    li r3, 0x10
    bl fn_801F02AC
    mr r28, r3
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802651A4
    mr r4, r28
    mr r5, r31
    li r3, 0x2
    bl fn_801F02AC
    cmplwi r3, 0x0
    bne @L_80265144
    li r3, 0x0
    b @L_802651A8
@L_80265144:
    li r4, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_801F76B8
    rlwinm r29, r3, 0, 16, 31
    mr r3, r28
    mr r4, r31
    bl fn_801F0134
    rlwinm. r0, r3, 0, 16, 31
    bne @L_80265174
    li r3, 0x0
    b @L_802651A8
@L_80265174:
    bl fn_801F0234
    bl fn_801F0204
    cmpwi r3, 0x0
    bge @L_8026518C
    li r3, 0x0
    b @L_802651A8
@L_8026518C:
    mr r4, r29
    rlwinm r6, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x3
    bl fn_801F76B8
    b @L_802651A8
@L_802651A4:
    li r3, 0x0
@L_802651A8:
    stw r3, 0x14(r1)
    mr r4, r30
    mr r5, r31
    li r3, 0xe
    bl fn_801F02AC
    mr r28, r3
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80265250
    mr r4, r28
    mr r5, r31
    li r3, 0x2
    bl fn_801F02AC
    cmplwi r3, 0x0
    bne @L_802651F0
    li r3, 0x0
    b @L_80265254
@L_802651F0:
    li r4, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_801F76B8
    rlwinm r29, r3, 0, 16, 31
    mr r3, r28
    mr r4, r31
    bl fn_801F0134
    rlwinm. r0, r3, 0, 16, 31
    bne @L_80265220
    li r3, 0x0
    b @L_80265254
@L_80265220:
    bl fn_801F0234
    bl fn_801F0204
    cmpwi r3, 0x0
    bge @L_80265238
    li r3, 0x0
    b @L_80265254
@L_80265238:
    mr r4, r29
    rlwinm r6, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x3
    bl fn_801F76B8
    b @L_80265254
@L_80265250:
    li r3, 0x0
@L_80265254:
    li r4, 0x0
    li r0, 0x3
    stw r3, 0x1c(r1)
    li r3, 0x0
    stw r4, 0x24(r1)
    stb r0, 0x28(r1)
    bl fn_801F18DC
    stb r3, 0x29(r1)
    addi r3, r1, 0x8
    li r4, 0x0
    li r5, 0x1
    bl fn_80011288
    li r0, 0x1
    cmpwi r3, 0x0
    stb r0, -20904(r13)
    bge @L_802652A4
    li r3, 0x1
    bl fn_8001120C
    li r3, 0x0
    b @L_80265318
@L_802652A4:
    bne @L_802652C0
    mr r4, r30
    mr r5, r31
    li r3, 0xf
    bl fn_801F02AC
    mr r29, r3
    b @L_802652FC
@L_802652C0:
    cmpwi r3, 0x1
    bne @L_802652E0
    mr r4, r30
    mr r5, r31
    li r3, 0x10
    bl fn_801F02AC
    mr r29, r3
    b @L_802652FC
@L_802652E0:
    cmpwi r3, 0x2
    bne @L_80265054
    mr r4, r30
    mr r5, r31
    li r3, 0xe
    bl fn_801F02AC
    mr r29, r3
@L_802652FC:
    mr r3, r29
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80265054
    li r3, 0x1
    bl fn_8001120C
    mr r3, r29
@L_80265318:
    lmw r28, 0x30(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr

    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r25, r3
    mr r26, r4
    lbz r0, 0x74(r26)
    cmpwi r0, 0x1
    beq @L_80108C68
    bge @L_80108C48
    cmpwi r0, 0x0
    bge @L_80108C54
    b @L_80108CA8
@L_80108C48:
    cmpwi r0, 0x3
    bge @L_80108CA8
    b @L_80108C98
@L_80108C54:
    li r3, 0x1
    li r4, 0x6
    li r5, 0x7
    bl fn_800DA4C4
    b @L_80108CA8
@L_80108C68:
    li r3, 0x1
    li r4, 0x6
    li r5, 0x0
    li r6, 0x1
    li r7, 0x0
    li r8, 0x0
    bl fn_800DA100
    li r3, 0x1
    li r4, 0x4
    li r5, 0x0
    bl fn_800DA4C4
    b @L_80108CA8
@L_80108C98:
    li r3, 0x1
    li r4, 0x6
    li r5, 0x1
    bl fn_800DA4C4
@L_80108CA8:
    lbz r0, 0x5(r26)
    rlwinm r0, r0, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_80108D80
    lha r5, 0x84(r25)
    lha r3, 0x50(r26)
    lha r4, 0x86(r25)
    lha r0, 0x52(r26)
    add r3, r5, r3
    extsh r3, r3
    add r0, r4, r0
    extsh r4, r0
    bl fn_800FE6D0
    bl fn_800FE4D4
    lwz r12, 0x48(r26)
    mr r3, r25
    mr r4, r26
    mtctr r12
    bctrl
    lha r3, 0x84(r25)
    lha r4, 0x86(r25)
    bl fn_800FE6D0
    bl fn_800FE4D4
    lbz r0, 0x74(r26)
    cmpwi r0, 0x1
    beq @L_80108D40
    bge @L_80108D20
    cmpwi r0, 0x0
    bge @L_80108D2C
    b @L_80108D80
@L_80108D20:
    cmpwi r0, 0x3
    bge @L_80108D80
    b @L_80108D70
@L_80108D2C:
    li r3, 0x1
    li r4, 0x6
    li r5, 0x7
    bl fn_800DA4C4
    b @L_80108D80
@L_80108D40:
    li r3, 0x1
    li r4, 0x6
    li r5, 0x0
    li r6, 0x1
    li r7, 0x0
    li r8, 0x0
    bl fn_800DA100
    li r3, 0x1
    li r4, 0x4
    li r5, 0x0
    bl fn_800DA4C4
    b @L_80108D80
@L_80108D70:
    li r3, 0x1
    li r4, 0x6
    li r5, 0x1
    bl fn_800DA4C4
@L_80108D80:
    cmplwi r26, 0x0
    bne @L_80108D90
    li r0, 0x0
    b @L_80108DB0
@L_80108D90:
    lbz r0, 0x4(r26)
    extsb r0, r0
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_80108DAC
    li r0, 0x1
    b @L_80108DB0
@L_80108DAC:
    li r0, 0x0
@L_80108DB0:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801091E0
    lbz r4, 0x64(r26)
    lis r3, 0x8081
    lbz r0, 0x88(r25)
    subi r9, r3, 0x7f7f
    lbz r7, 0x65(r26)
    lbz r6, 0x89(r25)
    mullw r8, r4, r0
    lbz r5, 0x66(r26)
    lbz r4, 0x8a(r25)
    lbz r3, 0x67(r26)
    lbz r0, 0x8b(r25)
    mullw r6, r7, r6
    mullw r4, r5, r4
    mullw r0, r3, r0
    mulhw r5, r9, r8
    mulhw r3, r9, r6
    add r5, r5, r8
    srawi r7, r5, 7
    mulhw r5, r9, r4
    rlwinm r8, r7, 1, 31, 31
    add r3, r3, r6
    add r7, r7, r8
    srawi r6, r3, 7
    rlwinm r31, r7, 0, 24, 31
    mulhw r3, r9, r0
    add r4, r5, r4
    rlwinm r5, r6, 1, 31, 31
    srawi r4, r4, 7
    add r6, r6, r5
    rlwinm r5, r4, 1, 31, 31
    add r0, r3, r0
    rlwinm r30, r6, 0, 24, 31
    srawi r0, r0, 7
    add r5, r4, r5
    rlwinm r4, r0, 1, 31, 31
    rlwinm r3, r31, 24, 0, 7
    add r0, r0, r4
    rlwinm r29, r5, 0, 24, 31
    rlwinm r28, r0, 0, 24, 31
    rlwinm r0, r30, 16, 0, 15
    cmplwi r28, 0x0
    rlwinm r4, r29, 8, 0, 23
    or r0, r3, r0
    or r0, r4, r0
    or r27, r28, r0
    beq @L_801091E0
    lbz r0, 0x5(r26)
    rlwinm r0, r0, 0, 31, 31
    cmpwi r0, 0x0
    beq @L_80108E90
    mr r3, r25
    mr r4, r26
    bl fn_80108580
@L_80108E90:
    lbz r0, 0x5(r26)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_801091BC
    li r3, 0x1
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    lis r3, 0x1
    lwz r4, 0x8(r26)
    addi r0, r3, 0x2
    cmpw r4, r0
    beq @L_80108FE0
    bge @L_80108ED8
    cmpw r4, r3
    beq @L_80108EEC
    bge @L_80108FAC
    b @L_801091BC
@L_80108ED8:
    addi r0, r3, 0x4
    cmpw r4, r0
    beq @L_80108FC4
    bge @L_801091BC
    b @L_801090D8
@L_80108EEC:
    li r3, 0x3
    bl fn_800D6A00
    lis r3, lbl_80314E08@ha
    addi r3, r3, lbl_80314E08@l
    bl fn_800D7820
    li r3, 0x3
    bl fn_800D67BC
    lha r3, 0x50(r26)
    lha r4, 0x52(r26)
    bl fn_800D61E4
    mr r4, r31
    mr r5, r30
    mr r6, r29
    mr r7, r28
    li r3, 0x0
    bl fn_800D5CB8
    lha r5, 0x50(r26)
    lha r3, 0x5c(r26)
    lha r4, 0x52(r26)
    lha r0, 0x5e(r26)
    add r3, r5, r3
    extsh r3, r3
    add r0, r4, r0
    extsh r4, r0
    bl fn_800D61E4
    mr r4, r31
    mr r5, r30
    mr r6, r29
    mr r7, r28
    li r3, 0x0
    bl fn_800D5CB8
    lha r5, 0x50(r26)
    lha r3, 0x54(r26)
    lha r4, 0x52(r26)
    lha r0, 0x56(r26)
    add r3, r5, r3
    extsh r3, r3
    add r0, r4, r0
    extsh r4, r0
    bl fn_800D61E4
    mr r4, r31
    mr r5, r30
    mr r6, r29
    mr r7, r28
    li r3, 0x0
    bl fn_800D5CB8
    bl fn_800D6728
    b @L_801091BC
@L_80108FAC:
    lha r3, 0x50(r26)
    lha r4, 0x52(r26)
    lha r5, 0x54(r26)
    lha r6, 0x56(r26)
    bl fn_8001EA98
    b @L_801091BC
@L_80108FC4:
    lha r3, 0x50(r26)
    mr r7, r28
    lha r4, 0x52(r26)
    lha r5, 0x54(r26)
    lha r6, 0x56(r26)
    bl fn_8001E644
    b @L_801091BC
@L_80108FE0:
    lha r0, 0x5c(r26)
    lis r4, 0x8081
    lha r3, 0x5e(r26)
    subi r8, r4, 0x7f7f
    rlwinm r4, r0, 0, 16, 31
    lha r0, 0x60(r26)
    mullw r7, r4, r31
    rlwinm r4, r3, 0, 16, 31
    rlwinm r0, r0, 0, 16, 31
    li r3, 0x7
    mullw r5, r4, r30
    mullw r0, r0, r29
    mulhw r4, r8, r7
    mulhw r6, r8, r5
    add r4, r4, r7
    srawi r7, r4, 7
    mulhw r4, r8, r0
    rlwinm r8, r7, 1, 31, 31
    add r5, r6, r5
    add r6, r7, r8
    srawi r5, r5, 7
    rlwinm r31, r6, 0, 24, 31
    add r0, r4, r0
    rlwinm r6, r5, 1, 31, 31
    srawi r0, r0, 7
    rlwinm r4, r0, 1, 31, 31
    add r5, r5, r6
    add r0, r0, r4
    rlwinm r25, r5, 0, 24, 31
    rlwinm r29, r0, 0, 24, 31
    bl fn_800D6A00
    lis r3, lbl_80314E08@ha
    addi r3, r3, lbl_80314E08@l
    bl fn_800D7820
    li r3, 0x2
    bl fn_800D67BC
    lha r3, 0x50(r26)
    lha r4, 0x52(r26)
    bl fn_800D61E4
    mr r4, r31
    mr r5, r25
    mr r6, r29
    mr r7, r28
    li r3, 0x0
    bl fn_800D5CB8
    lha r5, 0x50(r26)
    lha r3, 0x54(r26)
    lha r4, 0x52(r26)
    lha r0, 0x56(r26)
    add r3, r5, r3
    extsh r3, r3
    add r0, r4, r0
    extsh r4, r0
    bl fn_800D61E4
    mr r4, r31
    mr r5, r25
    mr r6, r29
    mr r7, r28
    li r3, 0x0
    bl fn_800D5CB8
    bl fn_800D6728
    b @L_801091BC
@L_801090D8:
    lha r0, 0x5c(r26)
    lis r4, 0x8081
    lha r3, 0x5e(r26)
    subi r7, r4, 0x7f7f
    rlwinm r4, r0, 0, 16, 31
    lha r0, 0x60(r26)
    mullw r6, r4, r31
    rlwinm r3, r3, 0, 16, 31
    rlwinm r0, r0, 0, 16, 31
    lfs f1, lbl_8047CE3C(r13)
    mullw r4, r3, r30
    mullw r0, r0, r29
    mulhw r3, r7, r6
    mulhw r5, r7, r4
    add r3, r3, r6
    srawi r6, r3, 7
    mulhw r3, r7, r0
    rlwinm r7, r6, 1, 31, 31
    add r4, r5, r4
    add r5, r6, r7
    srawi r4, r4, 7
    rlwinm r25, r5, 0, 24, 31
    add r0, r3, r0
    rlwinm r5, r4, 1, 31, 31
    srawi r0, r0, 7
    rlwinm r3, r0, 1, 31, 31
    add r4, r4, r5
    add r0, r0, r3
    rlwinm r29, r4, 0, 24, 31
    rlwinm r30, r0, 0, 24, 31
    bl fn_800D5648
    li r3, 0x1
    bl fn_800D6A00
    lis r3, lbl_80314E08@ha
    addi r3, r3, lbl_80314E08@l
    bl fn_800D7820
    li r3, 0x2
    bl fn_800D67BC
    lha r3, 0x50(r26)
    lha r4, 0x52(r26)
    bl fn_800D61E4
    mr r4, r25
    mr r5, r29
    mr r6, r30
    mr r7, r28
    li r3, 0x0
    bl fn_800D5CB8
    lha r3, 0x54(r26)
    lha r4, 0x56(r26)
    bl fn_800D61E4
    mr r4, r25
    mr r5, r29
    mr r6, r30
    mr r7, r28
    li r3, 0x0
    bl fn_800D5CB8
    bl fn_800D6728
@L_801091BC:
    lwz r8, 0x4c(r26)
    cmplwi r8, 0x0
    beq @L_801091E0
    lha r3, 0x50(r26)
    mr r7, r27
    lha r4, 0x52(r26)
    lha r5, 0x54(r26)
    lha r6, 0x56(r26)
    bl fn_800FBB34
@L_801091E0:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r27, r3
    li r31, 0x0
    li r30, 0x0
    b @L_801BB83C
@L_801BB4E4:
    rlwinm r0, r30, 3, 0, 28
    add r3, r27, r0
    lbz r0, 0x34(r3)
    cmplwi r0, 0x1
    bne @L_801BB838
    rlwinm r3, r30, 3, 0, 28
    rlwinm r0, r30, 3, 0, 28
    add r3, r27, r3
    li r26, 0x0
    lwz r29, 0x38(r3)
    add r3, r27, r0
    lbz r28, 0x35(r3)
    mr r3, r29
    bl fn_801B7C60
    cmpwi r3, 0x1
    beq @L_801BB52C
    li r26, 0x0
    b @L_801BB570
@L_801BB52C:
    mr r3, r29
    bl fn_801BB4C4
    cmpwi r3, 0x0
    beq @L_801BB540
    li r26, 0x1
@L_801BB540:
    mr r3, r29
    bl fn_801BAC8C
    cmpwi r3, 0x0
    beq @L_801BB554
    li r26, 0x1
@L_801BB554:
    mr r3, r29
    bl fn_801B9320
    cmpwi r3, 0x0
    beq @L_801BB568
    li r26, 0x1
@L_801BB568:
    mr r3, r29
    bl fn_801B9048
@L_801BB570:
    cmpwi r26, 0x0
    beq @L_801BB57C
    li r31, 0x1
@L_801BB57C:
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801BB7F4
    lbz r0, 0xd(r29)
    cmpwi r0, 0xff
    beq @L_801BB5A4
    bge @L_801BB838
    cmpwi r0, 0x0
    beq @L_801BB5D0
    b @L_801BB838
@L_801BB5A4:
    mr r3, r29
    mr r4, r28
    bl fn_801B750C
    rlwinm r4, r30, 3, 0, 28
    lwz r3, -31616(r13)
    lwz r0, -31612(r13)
    add r4, r27, r4
    li r31, 0x1
    stw r3, 0x34(r4)
    stw r0, 0x38(r4)
    b @L_801BB838
@L_801BB5D0:
    lbz r0, 0x35(r29)
    cmplwi r0, 0x7
    bne @L_801BB838
    lbz r0, 0x3d(r29)
    cmplwi r0, 0x7
    bne @L_801BB838
    lbz r0, 0xf(r29)
    cmplwi r0, 0x0
    bne @L_801BB838
    lbz r0, 0x10(r29)
    cmplwi r0, 0x0
    bne @L_801BB838
    lbz r0, 0x4c(r29)
    cmpwi r0, 0x3
    beq @L_801BB768
    bge @L_801BB620
    cmpwi r0, 0x1
    beq @L_801BB674
    bge @L_801BB6DC
    b @L_801BB838
@L_801BB620:
    cmpwi r0, 0x5
    beq @L_801BB62C
    b @L_801BB838
@L_801BB62C:
    rlwinm r0, r30, 3, 0, 28
    lwz r4, 0x4c(r29)
    lwz r5, 0x50(r29)
    add r6, r27, r0
    rlwinm r3, r30, 3, 0, 28
    rlwinm r0, r30, 3, 0, 28
    stw r4, 0x34(r6)
    add r3, r27, r3
    add r4, r27, r0
    stw r5, 0x38(r6)
    lwz r3, 0x38(r3)
    lbz r4, 0x35(r4)
    bl fn_801B7BD4
    mr r3, r29
    mr r4, r28
    bl fn_801B750C
    li r31, 0x1
    b @L_801BB838
@L_801BB674:
    cmpwi r30, 0x3
    bne @L_801BB694
    lbz r0, 0xe(r29)
    cmplwi r0, 0x0
    beq @L_801BB694
    lbz r0, 0x11(r29)
    cmplwi r0, 0x1
    beq @L_801BB838
@L_801BB694:
    rlwinm r0, r30, 3, 0, 28
    lwz r4, 0x4c(r29)
    lwz r5, 0x50(r29)
    add r6, r27, r0
    rlwinm r3, r30, 3, 0, 28
    rlwinm r0, r30, 3, 0, 28
    stw r4, 0x34(r6)
    add r3, r27, r3
    add r4, r27, r0
    stw r5, 0x38(r6)
    lwz r3, 0x38(r3)
    lbz r4, 0x35(r4)
    bl fn_801B7BD4
    mr r3, r29
    mr r4, r28
    bl fn_801B750C
    li r31, 0x1
    b @L_801BB838
@L_801BB6DC:
    lwz r0, 0x74(r27)
    cmplwi r0, 0x0
    beq @L_801BB6F8
    lwz r3, 0x74(r27)
    lwz r0, 0x74(r29)
    cmplw r3, r0
    bne @L_801BB838
@L_801BB6F8:
    lbz r0, 0x1e(r27)
    cmplwi r0, 0xff
    beq @L_801BB720
    lbz r0, 0x1e(r29)
    cmplwi r0, 0xff
    beq @L_801BB720
    lbz r3, 0x1e(r27)
    lbz r0, 0x1e(r29)
    cmplw r3, r0
    bne @L_801BB838
@L_801BB720:
    rlwinm r4, r30, 3, 0, 28
    lwz r3, 0x4c(r29)
    lwz r0, 0x50(r29)
    add r4, r27, r4
    stw r3, 0x34(r4)
    stw r0, 0x38(r4)
    lwz r0, 0x74(r29)
    stw r0, 0x74(r27)
    lbz r0, 0x1e(r27)
    cmplwi r0, 0xff
    bne @L_801BB754
    lbz r0, 0x1e(r29)
    stb r0, 0x1e(r27)
@L_801BB754:
    mr r3, r29
    mr r4, r28
    bl fn_801B750C
    li r31, 0x1
    b @L_801BB838
@L_801BB768:
    lbz r0, 0x78(r27)
    cmplwi r0, 0xff
    beq @L_801BB784
    lbz r3, 0x78(r27)
    lbz r0, 0x78(r29)
    cmplw r3, r0
    bne @L_801BB838
@L_801BB784:
    lbz r0, 0x1f(r27)
    cmplwi r0, 0xff
    beq @L_801BB7AC
    lbz r0, 0x1f(r29)
    cmplwi r0, 0xff
    beq @L_801BB7AC
    lbz r3, 0x1f(r27)
    lbz r0, 0x1f(r29)
    cmplw r3, r0
    bne @L_801BB838
@L_801BB7AC:
    rlwinm r4, r30, 3, 0, 28
    lwz r3, 0x4c(r29)
    lwz r0, 0x50(r29)
    add r4, r27, r4
    stw r3, 0x34(r4)
    stw r0, 0x38(r4)
    lbz r0, 0x78(r29)
    stb r0, 0x78(r27)
    lbz r0, 0x1f(r27)
    cmplwi r0, 0xff
    bne @L_801BB7E0
    lbz r0, 0x1f(r29)
    stb r0, 0x1f(r27)
@L_801BB7E0:
    mr r3, r29
    mr r4, r28
    bl fn_801B750C
    li r31, 0x1
    b @L_801BB838
@L_801BB7F4:
    lbz r0, 0x19(r29)
    cmpwi r0, 0xff
    beq @L_801BB810
    bge @L_801BB838
    cmpwi r0, 0x0
    beq @L_801BB838
    b @L_801BB838
@L_801BB810:
    mr r3, r29
    mr r4, r28
    bl fn_801B750C
    rlwinm r4, r30, 3, 0, 28
    lwz r3, -31616(r13)
    lwz r0, -31612(r13)
    add r4, r27, r4
    li r31, 0x1
    stw r3, 0x34(r4)
    stw r0, 0x38(r4)
@L_801BB838:
    addi r30, r30, 0x1
@L_801BB83C:
    cmpwi r30, 0x4
    blt @L_801BB4E4
    li r30, 0x0
    b @L_801BBAA8
@L_801BB84C:
    rlwinm r0, r30, 3, 0, 28
    add r3, r27, r0
    lbz r0, 0x54(r3)
    cmplwi r0, 0x1
    bne @L_801BBAA4
    rlwinm r3, r30, 3, 0, 28
    rlwinm r0, r30, 3, 0, 28
    add r3, r27, r3
    lwz r28, 0x58(r3)
    add r3, r27, r0
    lbz r29, 0x55(r3)
    mr r26, r28
    mr r3, r28
    bl fn_801B7C60
    cmpwi r3, 0x1
    bne @L_801BB8B8
    mr r3, r26
    bl fn_801BB4C4
    cmpwi r3, 0x0
    mr r3, r26
    bl fn_801BAC8C
    cmpwi r3, 0x0
    mr r3, r26
    bl fn_801B9320
    cmpwi r3, 0x0
    mr r3, r26
    bl fn_801B9048
@L_801BB8B8:
    lbz r0, 0x19(r28)
    cmpwi r0, 0xff
    beq @L_801BB8D4
    bge @L_801BBAA4
    cmpwi r0, 0x0
    beq @L_801BB900
    b @L_801BBAA4
@L_801BB8D4:
    mr r3, r28
    mr r4, r29
    bl fn_801B750C
    rlwinm r4, r30, 3, 0, 28
    lwz r3, -31616(r13)
    lwz r0, -31612(r13)
    add r4, r27, r4
    li r31, 0x1
    stw r3, 0x54(r4)
    stw r0, 0x58(r4)
    b @L_801BBAA4
@L_801BB900:
    lbz r0, 0x55(r28)
    cmplwi r0, 0x7
    bne @L_801BBAA4
    lbz r0, 0x5d(r28)
    cmplwi r0, 0x7
    bne @L_801BBAA4
    lbz r0, 0x1b(r28)
    cmplwi r0, 0x0
    bne @L_801BBAA4
    lbz r0, 0x1c(r28)
    cmplwi r0, 0x0
    bne @L_801BBAA4
    lbz r0, 0x6c(r28)
    cmpwi r0, 0x3
    beq @L_801BBA58
    bge @L_801BB950
    cmpwi r0, 0x1
    beq @L_801BB9A0
    bge @L_801BBA08
    b @L_801BBAA4
@L_801BB950:
    cmpwi r0, 0x5
    beq @L_801BB95C
    b @L_801BBAA4
@L_801BB95C:
    rlwinm r0, r30, 3, 0, 28
    lwz r4, 0x6c(r28)
    lwz r5, 0x70(r28)
    add r6, r27, r0
    rlwinm r3, r30, 3, 0, 28
    rlwinm r0, r30, 3, 0, 28
    stw r4, 0x54(r6)
    add r3, r27, r3
    add r4, r27, r0
    stw r5, 0x58(r6)
    lwz r3, 0x58(r3)
    lbz r4, 0x55(r4)
    bl fn_801B7BD4
    mr r3, r28
    mr r4, r29
    bl fn_801B750C
    li r31, 0x1
@L_801BB9A0:
    cmpwi r30, 0x3
    bne @L_801BB9C0
    lbz r0, 0x1a(r28)
    cmplwi r0, 0x0
    beq @L_801BB9C0
    lbz r0, 0x1d(r28)
    cmplwi r0, 0x1
    beq @L_801BBAA4
@L_801BB9C0:
    rlwinm r0, r30, 3, 0, 28
    lwz r4, 0x6c(r28)
    lwz r5, 0x70(r28)
    add r6, r27, r0
    rlwinm r3, r30, 3, 0, 28
    rlwinm r0, r30, 3, 0, 28
    stw r4, 0x54(r6)
    add r3, r27, r3
    add r4, r27, r0
    stw r5, 0x58(r6)
    lwz r3, 0x58(r3)
    lbz r4, 0x55(r4)
    bl fn_801B7BD4
    mr r3, r28
    mr r4, r29
    bl fn_801B750C
    li r31, 0x1
    b @L_801BBAA4
@L_801BBA08:
    lwz r0, 0x74(r27)
    cmplwi r0, 0x0
    beq @L_801BBA24
    lwz r3, 0x74(r27)
    lwz r0, 0x74(r28)
    cmplw r3, r0
    bne @L_801BBAA4
@L_801BBA24:
    rlwinm r3, r30, 3, 0, 28
    lwz r5, 0x6c(r28)
    lwz r0, 0x70(r28)
    add r6, r27, r3
    mr r3, r28
    mr r4, r29
    stw r5, 0x54(r6)
    stw r0, 0x58(r6)
    lwz r0, 0x74(r28)
    stw r0, 0x74(r27)
    bl fn_801B750C
    li r31, 0x1
    b @L_801BBAA4
@L_801BBA58:
    lbz r0, 0x78(r27)
    cmplwi r0, 0xff
    beq @L_801BBA74
    lbz r3, 0x78(r27)
    lbz r0, 0x78(r28)
    cmplw r3, r0
    bne @L_801BBAA4
@L_801BBA74:
    rlwinm r3, r30, 3, 0, 28
    lwz r5, 0x6c(r28)
    lwz r0, 0x70(r28)
    add r6, r27, r3
    mr r3, r28
    mr r4, r29
    stw r5, 0x54(r6)
    stw r0, 0x58(r6)
    lbz r0, 0x78(r28)
    stb r0, 0x78(r27)
    bl fn_801B750C
    li r31, 0x1
@L_801BBAA4:
    addi r30, r30, 0x1
@L_801BBAA8:
    cmpwi r30, 0x4
    blt @L_801BB84C
    mr r3, r31
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

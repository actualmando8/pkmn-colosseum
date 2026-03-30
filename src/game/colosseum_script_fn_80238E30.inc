    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r6, 0x0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r27, r4
    mr r28, r5
    mr r26, r3
    li r4, 0x0
    li r5, 0x43
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r29, r3, 0, 16, 31
    mr r3, r26
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r30, r3, 0, 16, 31
    mr r3, r27
    bl fn_80205BE8
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r31, r3, 0, 16, 31
    mr r4, r30
    li r3, 0x0
    li r5, 0x2a
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80238F40
    mr r3, r26
    mr r4, r27
    mr r5, r29
    bl fn_801F8424
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80238F24
    mr r4, r31
    li r3, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_8012640C
    rlwinm r0, r3, 0, 16, 31
    b @L_80238F44
@L_80238F24:
    mr r4, r31
    li r3, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_8012640C
    rlwinm r0, r3, 0, 16, 31
    b @L_80238F44
@L_80238F40:
    li r0, 0x9
@L_80238F44:
    rlwinm r3, r28, 0, 16, 31
    rlwinm r0, r0, 0, 16, 31
    cmplw r3, r0
    beq @L_80239038
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r31, r3, 0, 16, 31
    mr r3, r26
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r30, r3, 0, 16, 31
    mr r3, r27
    bl fn_80205BE8
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r29, r3, 0, 16, 31
    mr r4, r30
    li r3, 0x0
    li r5, 0x2a
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80239024
    mr r3, r26
    mr r4, r27
    mr r5, r31
    bl fn_801F8424
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80239008
    mr r4, r29
    li r3, 0x0
    li r5, 0x16
    li r6, 0x1
    bl fn_8012640C
    rlwinm r0, r3, 0, 16, 31
    b @L_80239028
@L_80239008:
    mr r4, r29
    li r3, 0x0
    li r5, 0x16
    li r6, 0x1
    bl fn_8012640C
    rlwinm r0, r3, 0, 16, 31
    b @L_80239028
@L_80239024:
    li r0, 0x9
@L_80239028:
    rlwinm r3, r28, 0, 16, 31
    rlwinm r0, r0, 0, 16, 31
    cmplw r3, r0
    bne @L_80239040
@L_80239038:
    li r3, 0x1
    b @L_80239044
@L_80239040:
    li r3, 0x0
@L_80239044:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

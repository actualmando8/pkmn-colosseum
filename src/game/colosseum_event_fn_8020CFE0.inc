    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r25, r4
    mr r29, r3
    mr r26, r5
    mr r27, r6
    mr r28, r8
    mr r31, r9
    mr r3, r25
    bl fn_80204A10
    rlwinm r0, r3, 0, 24, 31
    li r3, 0x0
    subfic r0, r0, 0x1
    cntlzw r0, r0
    rlwinm r30, r0, 27, 5, 31
    bl fn_801F18DC
    rlwinm r0, r3, 0, 24, 31
    mr r3, r29
    subfic r0, r0, 0x1
    cntlzw r0, r0
    rlwinm r29, r0, 27, 5, 31
    bl fn_801F8000
    cmplwi r3, 0x0
    bne @L_8020D054
    rlwinm. r0, r30, 0, 24, 31
    bne @L_8020D054
    li r29, 0x1
@L_8020D054:
    mr r3, r25
    bl fn_80205B8C
    li r4, 0x0
    li r5, 0x77
    li r6, 0x0
    bl fn_8012640C
    rlwinm. r0, r31, 0, 24, 31
    mr r31, r3
    bne @L_8020D150
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020D0C4
    rlwinm. r0, r28, 0, 16, 31
    bne @L_8020D0A8
    mr r4, r31
    li r3, 0x14
    bl fn_80132A38
    mr r4, r31
    li r3, 0x16
    bl fn_80132A38
    b @L_8020D1E8
@L_8020D0A8:
    mr r4, r31
    li r3, 0x15
    bl fn_80132A38
    mr r4, r31
    li r3, 0x17
    bl fn_80132A38
    b @L_8020D1E8
@L_8020D0C4:
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020D110
    rlwinm. r0, r28, 0, 16, 31
    bne @L_8020D0F4
    mr r4, r31
    li r3, 0x15
    bl fn_80132A38
    mr r4, r31
    li r3, 0x17
    bl fn_80132A38
    b @L_8020D1E8
@L_8020D0F4:
    mr r4, r31
    li r3, 0x14
    bl fn_80132A38
    mr r4, r31
    li r3, 0x16
    bl fn_80132A38
    b @L_8020D1E8
@L_8020D110:
    rlwinm. r0, r28, 0, 16, 31
    bne @L_8020D134
    mr r4, r31
    li r3, 0x14
    bl fn_80132A38
    mr r4, r31
    li r3, 0x16
    bl fn_80132A38
    b @L_8020D1E8
@L_8020D134:
    mr r4, r31
    li r3, 0x15
    bl fn_80132A38
    mr r4, r31
    li r3, 0x17
    bl fn_80132A38
    b @L_8020D1E8
@L_8020D150:
    cmplwi r0, 0x1
    bne @L_8020D1E8
    rlwinm r0, r26, 0, 16, 31
    cmplwi r0, 0x1
    bgt @L_8020D1A0
    rlwinm r0, r27, 0, 16, 31
    cmplwi r0, 0x1
    ble @L_8020D1A0
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020D184
    li r3, 0x7674
    b @L_8020D1E4
@L_8020D184:
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020D198
    li r3, 0x7679
    b @L_8020D1E4
@L_8020D198:
    li r3, 0x7671
    b @L_8020D1E4
@L_8020D1A0:
    mr r4, r31
    li r3, 0x14
    bl fn_80132A38
    mr r4, r31
    li r3, 0x16
    bl fn_80132A38
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020D1CC
    li r3, 0x7673
    b @L_8020D1E4
@L_8020D1CC:
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020D1E0
    li r3, 0x7678
    b @L_8020D1E4
@L_8020D1E0:
    li r3, 0x7670
@L_8020D1E4:
    bl fn_802624CC
@L_8020D1E8:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

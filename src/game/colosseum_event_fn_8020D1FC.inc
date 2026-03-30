    nofralloc
    stwu r1, -0xb60(r1)
    mflr r0
    stw r0, 0xb64(r1)
    stmw r18, 0xb28(r1)
    bl fn_8020D920
    bl fn_8020D814
    rlwinm r18, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0xd
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    bl fn_8020E0F8
    mr r0, r3
    li r3, 0x0
    mr r23, r0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r19, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r28, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x17
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r27, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x18
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r31, r3, 0, 16, 31
    mr r3, r18
    mr r5, r19
    li r4, 0x0
    bl fn_801F02AC
    mr r25, r3
    bl fn_801F7404
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8020D2C4
    li r3, 0x0
    b @L_8020D684
@L_8020D2C4:
    subfic r5, r18, 0x4
    mr r3, r25
    subic r0, r5, 0x1
    li r4, 0x0
    subfe r18, r0, r5
    li r5, 0x5
    li r6, 0x0
    bl fn_801F76B8
    rlwinm r0, r18, 0, 16, 31
    rlwinm r30, r3, 0, 16, 31
    mullw r29, r0, r28
    li r24, 0x0
    b @L_8020D5EC
@L_8020D2F8:
    mr r3, r25
    mr r6, r24
    li r4, 0x0
    li r5, 0x7
    bl fn_801F76B8
    add r0, r24, r29
    mr r4, r3
    rlwinm r20, r0, 0, 24, 31
    mr r3, r23
    mr r26, r4
    mr r4, r20
    bl fn_8020E068
    mr r0, r3
    mr r3, r23
    mr r18, r0
    mr r4, r20
    bl fn_8020E020
    mr r0, r3
    mr r3, r18
    mr r19, r0
    mr r4, r19
    bl fn_801FA4B4
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8020D5E8
    bl fn_8006B57C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020D380
    mr r3, r20
    bl fn_8006B0F8
    mr r4, r3
    addi r3, r1, 0xc
    bl fn_8012AC64
    b @L_8020D390
@L_8020D380:
    mr r3, r18
    mr r4, r19
    addi r5, r1, 0xc
    bl fn_801F9CBC
@L_8020D390:
    mr r3, r18
    bl fn_801F8FD8
    mr r0, r3
    mr r3, r26
    mr r7, r0
    mr r5, r18
    mr r6, r19
    addi r4, r1, 0xc
    bl fn_801FA6D8
    mr r3, r26
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    mr r0, r3
    mr r3, r26
    mr r22, r0
    bl fn_801FA634
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8020D5E8
    mr r3, r26
    mr r4, r27
    mr r5, r31
    bl fn_801F99C8
    li r21, 0x0
    li r19, 0x0
    b @L_8020D4E8
@L_8020D3FC:
    extsb r0, r21
    cmpw r0, r31
    bge @L_8020D4F4
    cmpw r0, r27
    bge @L_8020D4F4
    cmpwi r0, 0x6
    bge @L_8020D4F4
    mr r3, r22
    mr r5, r19
    li r4, 0x3
    bl fn_8012A5B0
    mr r18, r3
    bl fn_801233F4
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8020D4E4
    mr r3, r26
    mr r4, r18
    bl fn_801F9930
    cmplwi r3, 0x0
    bne @L_8020D4E4
    mr r3, r26
    extsb r6, r21
    li r4, 0x0
    li r5, 0x45
    bl fn_801FB1C0
    mr r0, r3
    li r3, 0x0
    mr r20, r0
    bl fn_801F4804
    mr r5, r3
    mr r3, r20
    mr r4, r18
    bl fn_80206AEC
    li r3, 0x0
    li r4, 0x0
    li r5, 0x27
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020D4E0
    li r3, 0x0
    li r4, 0x0
    li r5, 0x2e
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020D4E0
    mr r3, r26
    bl fn_801FB8F8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020D4E0
    mr r3, r20
    li r4, 0x3
    bl fn_802032E4
@L_8020D4E0:
    addi r21, r21, 0x1
@L_8020D4E4:
    addi r19, r19, 0x1
@L_8020D4E8:
    rlwinm r0, r19, 0, 16, 31
    cmplwi r0, 0x6
    blt @L_8020D3FC
@L_8020D4F4:
    li r20, 0x0
    b @L_8020D5DC
@L_8020D4FC:
    extsb r0, r21
    cmpw r0, r27
    bge @L_8020D5E8
    cmpwi r0, 0x6
    bge @L_8020D5E8
    mr r3, r22
    mr r5, r20
    li r4, 0x3
    bl fn_8012A5B0
    mr r18, r3
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8020D5D8
    mr r3, r26
    mr r4, r18
    bl fn_801F9930
    cmplwi r3, 0x0
    bne @L_8020D5D8
    mr r3, r26
    extsb r6, r21
    li r4, 0x0
    li r5, 0x45
    bl fn_801FB1C0
    mr r19, r3
    li r3, 0x0
    bl fn_801F4804
    mr r5, r3
    mr r3, r19
    mr r4, r18
    bl fn_80206AEC
    li r3, 0x0
    li r4, 0x0
    li r5, 0x27
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020D5D4
    li r3, 0x0
    li r4, 0x0
    li r5, 0x2e
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020D5D4
    mr r3, r26
    bl fn_801FB8F8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020D5D4
    mr r3, r19
    li r4, 0x3
    bl fn_802032E4
@L_8020D5D4:
    addi r21, r21, 0x1
@L_8020D5D8:
    addi r20, r20, 0x1
@L_8020D5DC:
    rlwinm r0, r20, 0, 16, 31
    cmplwi r0, 0x6
    blt @L_8020D4FC
@L_8020D5E8:
    addi r24, r24, 0x1
@L_8020D5EC:
    rlwinm r0, r24, 0, 16, 31
    cmplw r0, r28
    blt @L_8020D2F8
    mr r3, r25
    bl fn_801F7388
    rlwinm r18, r3, 0, 24, 31
    li r20, 0x0
    b @L_8020D674
@L_8020D60C:
    mr r3, r25
    mr r4, r20
    bl fn_801F7258
    cmplwi r3, 0x0
    beq @L_8020D670
    li r4, 0x0
    li r5, 0x4c
    li r6, 0x0
    bl fn_801FB1C0
    mr. r19, r3
    beq @L_8020D670
    mr r3, r30
    mr r4, r18
    mr r5, r20
    addi r6, r1, 0x9
    addi r7, r1, 0x8
    bl fn_801F72B0
    lbz r4, 0x9(r1)
    mr r3, r19
    lbz r5, 0x8(r1)
    bl fn_801C3FBC
    bl fn_801C3430
    mr r3, r19
    li r4, 0x1
    bl fn_801DA4E8
@L_8020D670:
    addi r20, r20, 0x1
@L_8020D674:
    rlwinm r0, r20, 0, 16, 31
    cmplw r0, r28
    blt @L_8020D60C
    li r3, 0x1
@L_8020D684:
    lmw r18, 0xb28(r1)
    lwz r0, 0xb64(r1)
    mtlr r0
    addi r1, r1, 0xb60
    blr

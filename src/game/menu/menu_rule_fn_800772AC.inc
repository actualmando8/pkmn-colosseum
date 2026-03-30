    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r28, r3
    mr r29, r4
    cntlzw r0, r28
    li r30, 0x0
    rlwinm r31, r0, 27, 5, 31
@L_800772D0:
    cmpwi r31, 0x0
    li r27, 0x0
    bne @L_800772F8
    mr r3, r28
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_800772FC
@L_800772F8:
    li r27, 0x1
@L_800772FC:
    cmpwi r27, 0x0
    beq @L_8007730C
    li r0, 0x1
    b @L_8007749C
@L_8007730C:
    cmpwi r30, 0x1
    beq @L_80077358
    bge @L_80077324
    cmpwi r30, 0x0
    bge @L_80077330
    b @L_80077480
@L_80077324:
    cmpwi r30, 0x3
    bge @L_80077480
    b @L_80077380
@L_80077330:
    mr r3, r28
    bl fn_8011F4A8
    rlwinm r5, r3, 0, 24, 31
    lha r0, 0x0(r29)
    srawi r4, r5, 31
    rlwinm r3, r0, 1, 31, 31
    subfc r0, r0, r5
    adde r0, r4, r3
    rlwinm r0, r0, 0, 24, 31
    b @L_8007749C
@L_80077358:
    mr r3, r28
    bl fn_8011F4A8
    lha r0, 0x2(r29)
    rlwinm r5, r3, 0, 24, 31
    rlwinm r3, r5, 1, 31, 31
    srawi r4, r0, 31
    subfc r0, r5, r0
    adde r0, r4, r3
    rlwinm r0, r0, 0, 24, 31
    b @L_8007749C
@L_80077380:
    mr r3, r28
    bl fn_8011F1A0
    mr r26, r3
    bl fn_8006B420
    rlwinm r0, r26, 0, 16, 31
    mr r27, r3
    cmpwi r0, 0xaf
    beq @L_800773B8
    bge @L_800773C0
    cmpwi r0, 0x0
    beq @L_800773B0
    b @L_800773C0
@L_800773B0:
    li r3, 0x1
    b @L_800773C8
@L_800773B8:
    li r3, 0x0
    b @L_800773C8
@L_800773C0:
    mr r3, r26
    bl fn_80142984
@L_800773C8:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800773DC
    li r0, 0x0
    b @L_8007749C
@L_800773DC:
    lwz r0, 0x8(r27)
    cmpwi r0, 0x1
    beq @L_8007740C
    bge @L_800773F8
    cmpwi r0, 0x0
    bge @L_80077404
    b @L_80077478
@L_800773F8:
    cmpwi r0, 0x3
    bge @L_80077478
    b @L_80077420
@L_80077404:
    li r0, 0x1
    b @L_8007749C
@L_8007740C:
    rlwinm r0, r26, 0, 16, 31
    cntlzw r0, r0
    rlwinm r0, r0, 27, 5, 31
    rlwinm r0, r0, 0, 24, 31
    b @L_8007749C
@L_80077420:
    lis r3, -32721
    lwz r0, -32504(r13)
    addi r5, r3, -7080
    li r4, 0x0
    rlwinm r3, r26, 0, 16, 31
    mtctr r0
    cmplwi r0, 0x0
    ble @L_80077470
@L_80077440:
    lhz r0, 0x0(r5)
    cmplw r3, r0
    bne @L_80077464
    addi r0, r4, 0x18
    lbzx r0, r27, r0
    cntlzw r0, r0
    rlwinm r0, r0, 27, 5, 31
    rlwinm r0, r0, 0, 24, 31
    b @L_8007749C
@L_80077464:
    addi r5, r5, 0x2
    addi r4, r4, 0x1
    bdnz @L_80077440
@L_80077470:
    li r0, 0x1
    b @L_8007749C
@L_80077478:
    li r0, 0x0
    b @L_8007749C
@L_80077480:
    lis r3, -32729
    lis r5, -32729
    addi r3, r3, -30136
    li r4, 0xfb
    addi r5, r5, -30120
    bl fn_80196E10
    li r0, 0x0
@L_8007749C:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800774B0
    li r3, 0x0
    b @L_800774C0
@L_800774B0:
    addi r30, r30, 0x1
    cmpwi r30, 0x3
    blt @L_800772D0
    li r3, 0x1
@L_800774C0:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

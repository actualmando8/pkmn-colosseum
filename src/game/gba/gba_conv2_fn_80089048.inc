    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    mr r29, r4
    mr r30, r5
    cmplwi r30, 0x0
    beq @L_80089094
    mr r3, r30
    bl fn_80123FBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80089094
    li r3, 0x0
    b @L_80089360
@L_80089094:
    lwz r11, 0x0(r29)
    cmplwi r30, 0x0
    lwz r12, 0x4(r29)
    lwz r31, 0x8(r29)
    rlwinm r5, r11, 0, 16, 23
    rlwinm r3, r12, 0, 16, 23
    rlwinm r10, r11, 0, 8, 15
    rlwinm r0, r31, 0, 16, 23
    rlwinm r7, r12, 0, 8, 15
    rlwinm r4, r31, 0, 8, 15
    rlwinm r9, r11, 24, 0, 7
    rlwinm r8, r5, 8, 0, 23
    rlwinm r6, r12, 24, 0, 7
    rlwinm r5, r3, 8, 0, 23
    rlwinm r3, r31, 24, 0, 7
    rlwinm r0, r0, 8, 0, 23
    rlwinm r10, r10, 24, 8, 31
    or r8, r9, r8
    rlwinm r7, r7, 24, 8, 31
    or r5, r6, r5
    rlwinm r4, r4, 24, 8, 31
    or r0, r3, r0
    rlwinm r9, r11, 8, 24, 31
    or r8, r10, r8
    rlwinm r6, r12, 8, 24, 31
    or r5, r7, r5
    rlwinm r3, r31, 8, 24, 31
    or r0, r4, r0
    or r7, r9, r8
    or r4, r6, r5
    stw r7, 0x0(r28)
    or r0, r3, r0
    lhz r31, 0xe(r29)
    stw r4, 0x4(r28)
    stw r0, 0x8(r28)
    beq @L_80089138
    mr r3, r30
    bl fn_8011F5C8
    rlwinm r0, r3, 0, 16, 31
    rlwinm r0, r0, 16, 0, 15
    or r31, r31, r0
@L_80089138:
    rlwinm r0, r31, 0, 16, 23
    rlwinm r4, r31, 0, 8, 15
    rlwinm r3, r31, 24, 0, 7
    lhz r5, 0xe(r29)
    rlwinm r0, r0, 8, 0, 23
    rlwinm r4, r4, 24, 8, 31
    or r0, r3, r0
    rlwinm r3, r31, 8, 24, 31
    or r0, r4, r0
    cmplwi r5, 0x32
    or r0, r3, r0
    addi r31, r28, 0x10
    stw r0, 0xc(r28)
    li r0, 0x0
    beq @L_8008917C
    cmplwi r5, 0x1e
    bne @L_80089180
@L_8008917C:
    li r0, 0x1
@L_80089180:
    cmpwi r0, 0x0
    bne @L_800891A0
    lis r3, -32729
    lis r5, -32729
    addi r3, r3, -2712
    li r4, 0xb7
    addi r5, r5, -2700
    bl fn_80196E10
@L_800891A0:
    mr r10, r29
    lhz r3, 0xe(r29)
    cmpwi r3, 0x0
    ble @L_80089314
    rlwinm r0, r3, 30, 2, 31
    cmplwi r0, 0x0
    mtctr r0
    beq @L_800892CC
@L_800891C0:
    lhz r9, 0x12(r10)
    lhz r11, 0x10(r10)
    addi r10, r10, 0x4
    rlwinm r0, r9, 16, 0, 15
    lhz r9, 0x12(r10)
    or r11, r11, r0
    rlwinm r7, r11, 0, 8, 15
    rlwinm r0, r9, 16, 0, 15
    rlwinm r4, r11, 0, 16, 23
    rlwinm r8, r11, 8, 24, 31
    rlwinm r5, r11, 24, 0, 7
    lhz r11, 0x10(r10)
    rlwinm r4, r4, 8, 0, 23
    addi r10, r10, 0x4
    or r11, r11, r0
    rlwinm r6, r7, 24, 8, 31
    or r0, r5, r4
    lhz r9, 0x12(r10)
    or r0, r6, r0
    rlwinm r4, r11, 0, 16, 23
    or r0, r8, r0
    rlwinm r7, r11, 0, 8, 15
    stw r0, 0x0(r31)
    rlwinm r8, r11, 8, 24, 31
    rlwinm r5, r11, 24, 0, 7
    rlwinm r4, r4, 8, 0, 23
    lhz r11, 0x10(r10)
    rlwinm r0, r9, 16, 0, 15
    addi r10, r10, 0x4
    rlwinm r6, r7, 24, 8, 31
    or r11, r11, r0
    or r0, r5, r4
    or r0, r6, r0
    lhz r9, 0x12(r10)
    or r0, r8, r0
    rlwinm r7, r11, 0, 8, 15
    rlwinm r4, r11, 0, 16, 23
    addi r31, r31, 0x4
    stw r0, 0x0(r31)
    rlwinm r8, r11, 8, 24, 31
    rlwinm r5, r11, 24, 0, 7
    lhz r11, 0x10(r10)
    rlwinm r0, r9, 16, 0, 15
    rlwinm r4, r4, 8, 0, 23
    or r11, r11, r0
    rlwinm r6, r7, 24, 8, 31
    or r0, r5, r4
    addi r31, r31, 0x4
    rlwinm r4, r11, 0, 16, 23
    rlwinm r7, r11, 0, 8, 15
    or r0, r6, r0
    rlwinm r5, r11, 24, 0, 7
    or r0, r8, r0
    rlwinm r4, r4, 8, 0, 23
    stw r0, 0x0(r31)
    rlwinm r6, r7, 24, 8, 31
    or r0, r5, r4
    rlwinm r8, r11, 8, 24, 31
    or r0, r6, r0
    addi r31, r31, 0x4
    or r0, r8, r0
    addi r10, r10, 0x4
    stw r0, 0x0(r31)
    addi r31, r31, 0x4
    bdnz @L_800891C0
    andi. r3, r3, 0x3
    beq @L_80089314
@L_800892CC:
    mtctr r3
@L_800892D0:
    lhz r9, 0x12(r10)
    lhz r11, 0x10(r10)
    addi r10, r10, 0x4
    rlwinm r0, r9, 16, 0, 15
    or r11, r11, r0
    rlwinm r4, r11, 0, 16, 23
    rlwinm r7, r11, 0, 8, 15
    rlwinm r5, r11, 24, 0, 7
    rlwinm r4, r4, 8, 0, 23
    rlwinm r8, r11, 8, 24, 31
    rlwinm r6, r7, 24, 8, 31
    or r0, r5, r4
    or r0, r6, r0
    or r0, r8, r0
    stw r0, 0x0(r31)
    addi r31, r31, 0x4
    bdnz @L_800892D0
@L_80089314:
    cmplwi r30, 0x0
    beq @L_8008935C
    mr r3, r30
    mr r4, r31
    bl fn_8008AE18
    addi r3, r31, 0x64
    li r4, 0x0
    li r5, 0xc
    bl memset
    addi r28, r31, 0x64
    li r29, 0x0
@L_80089340:
    mr r3, r29
    bl fn_80265F14
    stb r3, 0x0(r28)
    addi r28, r28, 0x1
    addi r29, r29, 0x1
    cmpwi r29, 0xb
    blt @L_80089340
@L_8008935C:
    li r3, 0x1
@L_80089360:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

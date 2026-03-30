    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r29, r3
    mr r30, r4
    mr r31, r5
    cmpwi r29, 0x0
    blt @L_8009331C
    cmpwi r29, 0x3
    ble @L_80093324
@L_8009331C:
    li r0, 0x0
    b @L_800933F4
@L_80093324:
    lis r3, -32704
    rlwinm r26, r29, 2, 0, 29
    addi r27, r3, -19672
    lwzx r0, r27, r26
    cmplwi r0, 0x0
    beq @L_80093344
    li r0, 0x1
    b @L_800933F4
@L_80093344:
    li r3, 0x44a0
    li r4, 0x20
    bl fn_800E2C04
    mr r28, r3
    rlwinm r0, r28, 0, 16, 31
    cmplwi r0, 0x0
    bne @L_80093374
    lis r3, -32729
    li r4, 0x1dd
    addi r3, r3, -2648
    addi r5, r2, -29880
    bl fn_80196E10
@L_80093374:
    mr r3, r28
    bl fn_800E27B0
    mr r28, r3
    li r4, 0x0
    li r5, 0x4490
    bl memset
    stwx r28, r27, r26
    lis r3, -32759
    addi r5, r3, 15108
    li r0, 0x0
    lwzx r26, r27, r26
    mr r3, r29
    stw r0, 0x4340(r26)
    addi r4, r26, 0x20
    stw r29, 0x4338(r26)
    bl fn_800716C8
    mr r3, r26
    bl fn_8009F77C
    addi r3, r26, 0x18
    bl fn_8009F9C8
    lis r3, -32759
    mr r5, r26
    addi r4, r3, 14324
    addi r6, r26, 0x4338
    addi r3, r26, 0x20
    li r7, 0x4000
    li r8, 0x8
    li r9, 0x0
    bl fn_800A19CC
    addi r3, r26, 0x20
    bl fn_800A1F94
    li r0, 0x1
@L_800933F4:
    cmpwi r0, 0x0
    bne @L_80093404
    li r3, 0x0
    b @L_800934D0
@L_80093404:
    lis r3, -32704
    rlwinm r0, r29, 2, 0, 29
    addi r4, r3, -19672
    mr r3, r30
    lwzx r27, r4, r0
    li r26, 0x0
    bl strlen
    cmplwi r31, 0x0
    mr r29, r3
    beq @L_80093438
    mr r3, r31
    bl strlen
    b @L_8009343C
@L_80093438:
    li r3, 0x0
@L_8009343C:
    cmplwi r29, 0x7f
    bge @L_8009344C
    cmplwi r3, 0x7f
    blt @L_80093454
@L_8009344C:
    li r26, 0x0
    b @L_800934CC
@L_80093454:
    mr r3, r27
    bl fn_8009F7B4
    lwz r0, 0x4340(r27)
    cmpwi r0, 0x0
    bne @L_800934A8
    li r26, 0x1
    lis r3, 0x3
    stw r26, 0x4340(r27)
    addi r0, r3, 0x1
    mr r4, r30
    addi r3, r27, 0x4344
    stw r0, 0x433c(r27)
    bl fn_800CA968
    cmplwi r31, 0x0
    beq @L_800934A0
    mr r4, r31
    addi r3, r27, 0x43c4
    bl fn_800CA968
    b @L_800934A8
@L_800934A0:
    li r0, 0x0
    stb r0, 0x43c4(r27)
@L_800934A8:
    mr r3, r27
    bl fn_8009F890
    addi r3, r27, 0x20
    li r4, 0x8
    bl fn_800A257C
    cmpwi r26, 0x0
    beq @L_800934CC
    addi r3, r27, 0x18
    bl fn_8009FABC
@L_800934CC:
    mr r3, r26
@L_800934D0:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

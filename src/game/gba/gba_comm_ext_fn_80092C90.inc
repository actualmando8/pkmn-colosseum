    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r29, r3
    mr r30, r4
    mr r31, r5
    cmpwi r29, 0x0
    blt @L_80092CBC
    cmpwi r29, 0x3
    ble @L_80092CC4
@L_80092CBC:
    li r0, 0x0
    b @L_80092D94
@L_80092CC4:
    lis r3, -32704
    rlwinm r26, r29, 2, 0, 29
    addi r27, r3, -19672
    lwzx r0, r27, r26
    cmplwi r0, 0x0
    beq @L_80092CE4
    li r0, 0x1
    b @L_80092D94
@L_80092CE4:
    li r3, 0x44a0
    li r4, 0x20
    bl fn_800E2C04
    mr r28, r3
    rlwinm r0, r28, 0, 16, 31
    cmplwi r0, 0x0
    bne @L_80092D14
    lis r3, -32729
    li r4, 0x1dd
    addi r3, r3, -2648
    addi r5, r2, -29880
    bl fn_80196E10
@L_80092D14:
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
@L_80092D94:
    cmpwi r0, 0x0
    bne @L_80092DA4
    li r3, 0x0
    b @L_80092E24
@L_80092DA4:
    lis r3, -32704
    rlwinm r0, r29, 2, 0, 29
    addi r3, r3, -19672
    li r26, 0x0
    lwzx r27, r3, r0
    mr r3, r27
    bl fn_8009F7B4
    lwz r0, 0x4340(r27)
    cmpwi r0, 0x0
    bne @L_80092DFC
    mr r4, r30
    mr r5, r31
    addi r3, r27, 0x4344
    bl fn_80089048
    mr r26, r3
    cmpwi r26, 0x0
    beq @L_80092DFC
    li r0, 0xc
    lis r3, 0x3
    stw r0, 0x4340(r27)
    addi r0, r3, 0xc
    stw r0, 0x433c(r27)
@L_80092DFC:
    mr r3, r27
    bl fn_8009F890
    addi r3, r27, 0x20
    li r4, 0x8
    bl fn_800A257C
    cmpwi r26, 0x0
    beq @L_80092E20
    addi r3, r27, 0x18
    bl fn_8009FABC
@L_80092E20:
    mr r3, r26
@L_80092E24:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

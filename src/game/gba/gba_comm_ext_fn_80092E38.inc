    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r30, r3
    mr r31, r4
    cmpwi r30, 0x0
    blt @L_80092E60
    cmpwi r30, 0x3
    ble @L_80092E68
@L_80092E60:
    li r0, 0x0
    b @L_80092F38
@L_80092E68:
    lis r3, -32704
    rlwinm r27, r30, 2, 0, 29
    addi r28, r3, -19672
    lwzx r0, r28, r27
    cmplwi r0, 0x0
    beq @L_80092E88
    li r0, 0x1
    b @L_80092F38
@L_80092E88:
    li r3, 0x44a0
    li r4, 0x20
    bl fn_800E2C04
    mr r29, r3
    rlwinm r0, r29, 0, 16, 31
    cmplwi r0, 0x0
    bne @L_80092EB8
    lis r3, -32729
    li r4, 0x1dd
    addi r3, r3, -2648
    addi r5, r2, -29880
    bl fn_80196E10
@L_80092EB8:
    mr r3, r29
    bl fn_800E27B0
    mr r29, r3
    li r4, 0x0
    li r5, 0x4490
    bl memset
    stwx r29, r28, r27
    lis r3, -32759
    addi r5, r3, 15108
    li r0, 0x0
    lwzx r27, r28, r27
    mr r3, r30
    stw r0, 0x4340(r27)
    addi r4, r27, 0x20
    stw r30, 0x4338(r27)
    bl fn_800716C8
    mr r3, r27
    bl fn_8009F77C
    addi r3, r27, 0x18
    bl fn_8009F9C8
    lis r3, -32759
    mr r5, r27
    addi r4, r3, 14324
    addi r6, r27, 0x4338
    addi r3, r27, 0x20
    li r7, 0x4000
    li r8, 0x8
    li r9, 0x0
    bl fn_800A19CC
    addi r3, r27, 0x20
    bl fn_800A1F94
    li r0, 0x1
@L_80092F38:
    cmpwi r0, 0x0
    bne @L_80092F48
    li r3, 0x0
    b @L_80092FB4
@L_80092F48:
    lis r3, -32704
    rlwinm r0, r30, 2, 0, 29
    addi r3, r3, -19672
    li r27, 0x0
    lwzx r28, r3, r0
    mr r3, r28
    bl fn_8009F7B4
    lwz r0, 0x4340(r28)
    cmpwi r0, 0x0
    bne @L_80092F8C
    li r0, 0xb
    lis r3, 0x3
    stw r0, 0x4340(r28)
    addi r0, r3, 0xb
    li r27, 0x1
    stw r0, 0x433c(r28)
    stw r31, 0x4344(r28)
@L_80092F8C:
    mr r3, r28
    bl fn_8009F890
    addi r3, r28, 0x20
    li r4, 0x8
    bl fn_800A257C
    cmpwi r27, 0x0
    beq @L_80092FB0
    addi r3, r28, 0x18
    bl fn_8009FABC
@L_80092FB0:
    mr r3, r27
@L_80092FB4:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

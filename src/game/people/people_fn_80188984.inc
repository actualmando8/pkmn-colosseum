    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    li r31, 0x0
    b @L_801889E0
    @L_801889A8:
    mr r3, r31
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801889DC
    lwz r0, 0x28(r3)
    cmplw r0, r27
    bne @L_801889DC
    lwz r0, 0x2c(r3)
    cmplw r0, r28
    bne @L_801889DC
    lwz r31, 0x4(r3)
    b @L_80188A4C
    @L_801889DC:
    addi r31, r31, 0x1
    @L_801889E0:
    bl fn_8018FDB4
    cmpw r31, r3
    blt @L_801889A8
    li r30, 0x0
    b @L_80188A3C
    @L_801889F4:
    mr r3, r30
    bl fn_8018FD88
    mr r31, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80188A38
    lwz r0, 0x2c(r31)
    cmplw r0, r28
    bne @L_80188A38
    lis r3, lbl_80273FD8@ha
    mr r4, r27
    addi r3, r3, lbl_80273FD8@l
    mr r5, r28
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r31, 0x4(r31)
    b @L_80188A4C
    @L_80188A38:
    addi r30, r30, 0x1
    @L_80188A3C:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_801889F4
    li r31, 0x0
    @L_80188A4C:
    li r30, 0x0
    b @L_80188A80
    @L_80188A54:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80188A7C
    lwz r0, 0x4(r3)
    cmplw r0, r31
    bne @L_80188A7C
    mr r30, r3
    b @L_80188A90
    @L_80188A7C:
    addi r30, r30, 0x1
    @L_80188A80:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_80188A54
    li r30, 0x0
    @L_80188A90:
    cmplwi r30, 0x0
    bne @L_80188AA0
    li r3, 0x0
    b @L_80188AE0
    @L_80188AA0:
    clrlwi r31, r29, 24
    @L_80188AA4:
    lfs f1, 0xc(r30)
    lfs f0, 0xb4(r30)
    fcmpu cr0, f1, f0
    bne @L_80188ACC
    lfs f1, 0x10(r30)
    lfs f0, 0xb8(r30)
    fcmpu cr0, f1, f0
    bne @L_80188ACC
    li r3, 0x0
    b @L_80188AE0
    @L_80188ACC:
    cmplwi r31, 0x0
    beq @L_80188ADC
    bl fn_800F0308
    b @L_80188AA4
    @L_80188ADC:
    li r3, 0x1
    @L_80188AE0:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

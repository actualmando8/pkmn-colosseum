    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    beq @L_8013B140
    lbz r0, 0x4c(r3)
    cmplwi r0, 0x0
    beq @L_8013B128
    lwz r0, 0x48(r3)
    addi r27, r3, 0x4
    lbz r30, 0x4e(r3)
    lbz r31, 0x4f(r3)
    clrlwi r28, r0, 16
    bl fn_800B8DF4
    bl fn_800B856C
    li r29, 0x0
    b @L_8013B118
    @L_8013B0E8:
    lwz r3, 0x0(r27)
    cmplwi r3, 0x0
    beq @L_8013B110
    cmplwi r31, 0x0
    bne @L_8013B100
    bl fn_800E5A74
    @L_8013B100:
    cmplwi r30, 0x0
    beq @L_8013B110
    lwz r3, 0x0(r27)
    bl fn_800E638C
    @L_8013B110:
    addi r29, r29, 0x1
    addi r27, r27, 0x4
    @L_8013B118:
    clrlwi r0, r29, 16
    cmplw r0, r28
    blt @L_8013B0E8
    b @L_8013B138
    @L_8013B128:
    lwz r3, 0x0(r3)
    cmplwi r3, 0x0
    beq @L_8013B138
    bl fn_801684F0
    @L_8013B138:
    li r3, 0x1
    b @L_8013B144
    @L_8013B140:
    li r3, 0x0
    @L_8013B144:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

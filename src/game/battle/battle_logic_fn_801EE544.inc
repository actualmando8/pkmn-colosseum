    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    clrlwi. r3, r3, 16
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mulli r0, r3, 0x38
    mr r29, r4
    li r31, 0x0
    lwz r5, -30900(r13)
    add r4, r5, r0
    beq @L_801EE578
    cmplwi r3, 0x60
    ble @L_801EE57C
@L_801EE578:
    li r4, 0x0
@L_801EE57C:
    cmplwi r4, 0x0
    beq @L_801EE5FC
    lhz r0, 0x1e(r4)
    lwz r3, -30908(r13)
    slwi r0, r0, 3
    add r30, r3, r0
@L_801EE594:
    lwz r0, 0x4(r30)
    cmplwi r0, 0x0
    beq @L_801EE5F4
    lhz r3, 0x2(r30)
    cmplwi r3, 0x0
    beq @L_801EE5D8
    bl fn_801906A0
    lbz r0, 0x0(r30)
    clrlwi r3, r3, 24
    cmplw r3, r0
    bne @L_801EE5EC
    cmplwi r29, 0x0
    lwz r31, 0x4(r30)
    beq @L_801EE5EC
    lbz r0, 0x1(r30)
    stb r0, 0x0(r29)
    b @L_801EE5EC
@L_801EE5D8:
    cmplwi r29, 0x0
    mr r31, r0
    beq @L_801EE5EC
    lbz r0, 0x1(r30)
    stb r0, 0x0(r29)
@L_801EE5EC:
    addi r30, r30, 0x8
    b @L_801EE594
@L_801EE5F4:
    mr r3, r31
    b @L_801EE600
@L_801EE5FC:
    li r3, 0x0
@L_801EE600:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

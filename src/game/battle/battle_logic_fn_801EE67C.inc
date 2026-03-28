    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    clrlwi. r30, r3, 16
    mr r27, r3
    mr r28, r4
    mulli r29, r30, 0x38
    lwz r0, -30900(r13)
    add r31, r0, r29
    beq @L_801EE6B0
    cmplwi r30, 0x60
    ble @L_801EE6B4
@L_801EE6B0:
    li r31, 0x0
@L_801EE6B4:
    cmplwi r31, 0x0
    beq @L_801EE73C
    li r3, 0x0
    bl fn_801EF1E4
    cmplwi r3, 0x0
    beq @L_801EE6F0
    mulli r4, r30, 0x18
    addi r5, r4, 0x4a4
    lhax r0, r3, r5
    cmpwi r0, 0x0
    bge @L_801EE6F0
    lwz r4, 0x0(r3)
    addi r0, r4, 0x1
    stw r0, 0x0(r3)
    sthx r4, r3, r5
@L_801EE6F0:
    lwz r3, -30900(r13)
    clrlwi. r0, r27, 16
    add r3, r3, r29
    beq @L_801EE708
    cmplwi r0, 0x60
    ble @L_801EE70C
@L_801EE708:
    li r3, 0x0
@L_801EE70C:
    cmplwi r3, 0x0
    beq @L_801EE728
    lhz r3, 0xc(r3)
    cmplwi r3, 0x0
    beq @L_801EE728
    li r4, 0x1
    bl fn_8019075C
@L_801EE728:
    lhz r3, 0x1c(r31)
    cmplwi r3, 0x0
    beq @L_801EE73C
    clrlwi r4, r28, 16
    bl fn_8019075C
@L_801EE73C:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

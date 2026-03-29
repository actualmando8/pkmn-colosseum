    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x28(r1)
    stw r31, 0x24(r1)
    stw r30, 0x20(r1)
    stw r29, 0x1c(r1)
    mr r29, r3
    bl OSDisableInterrupts
    lwz r4, 0x2cc(r29)
    addi r31, r3, 0x0
    subi r0, r4, 0x1
    stw r0, 0x2cc(r29)
    mr r30, r4
    lwz r0, 0x2cc(r29)
    cmpwi r0, 0x0
    bge @L_800A1FE0
    li r0, 0x0
    stw r0, 0x2cc(r29)
    b @L_800A21F4
    @L_800A1FE0:
    bne @L_800A21F4
    lhz r0, 0x2c8(r29)
    cmpwi r0, 0x4
    beq @L_800A20A4
    bge @L_800A21E0
    cmpwi r0, 0x1
    beq @L_800A2000
    b @L_800A21E0
    @L_800A2000:
    lwz r0, 0x2d4(r29)
    lwz r3, 0x2f4(r29)
    b @L_800A202C
    @L_800A200C:
    lwz r4, 0x0(r3)
    cmplwi r4, 0x0
    beq @L_800A2028
    lwz r4, 0x2d0(r4)
    cmpw r4, r0
    bge @L_800A2028
    mr r0, r4
    @L_800A2028:
    lwz r3, 0x10(r3)
    @L_800A202C:
    cmplwi r3, 0x0
    bne @L_800A200C
    stw r0, 0x2d0(r29)
    lis r3, RunQueue_803FB898@ha
    addi r0, r3, RunQueue_803FB898@l
    lwz r3, 0x2d0(r29)
    slwi r3, r3, 3
    add r0, r0, r3
    stw r0, 0x2dc(r29)
    lwz r4, 0x2dc(r29)
    lwz r3, 0x4(r4)
    cmplwi r3, 0x0
    bne @L_800A2068
    stw r29, 0x0(r4)
    b @L_800A206C
    @L_800A2068:
    stw r29, 0x2e0(r3)
    @L_800A206C:
    stw r3, 0x2e4(r29)
    li r0, 0x0
    li r3, 0x1
    stw r0, 0x2e0(r29)
    lwz r4, 0x2dc(r29)
    stw r29, 0x4(r4)
    lwz r0, 0x2d0(r29)
    lwz r4, RunQueueBits_8047A760(r13)
    subfic r0, r0, 0x1f
    slw r0, r3, r0
    or r0, r4, r0
    stw r0, RunQueueBits_8047A760(r13)
    stw r3, RunQueueHint_8047A764(r13)
    b @L_800A21E0
    @L_800A20A4:
    lwz r4, 0x2e0(r29)
    lwz r5, 0x2e4(r29)
    cmplwi r4, 0x0
    bne @L_800A20C0
    lwz r3, 0x2dc(r29)
    stw r5, 0x4(r3)
    b @L_800A20C4
    @L_800A20C0:
    stw r5, 0x2e4(r4)
    @L_800A20C4:
    cmplwi r5, 0x0
    bne @L_800A20D8
    lwz r3, 0x2dc(r29)
    stw r4, 0x0(r3)
    b @L_800A20DC
    @L_800A20D8:
    stw r4, 0x2e0(r5)
    @L_800A20DC:
    lwz r0, 0x2d4(r29)
    lwz r3, 0x2f4(r29)
    b @L_800A2108
    @L_800A20E8:
    lwz r4, 0x0(r3)
    cmplwi r4, 0x0
    beq @L_800A2104
    lwz r4, 0x2d0(r4)
    cmpw r4, r0
    bge @L_800A2104
    mr r0, r4
    @L_800A2104:
    lwz r3, 0x10(r3)
    @L_800A2108:
    cmplwi r3, 0x0
    bne @L_800A20E8
    stw r0, 0x2d0(r29)
    lwz r4, 0x2dc(r29)
    lwz r5, 0x0(r4)
    b @L_800A2124
    @L_800A2120:
    lwz r5, 0x2e0(r5)
    @L_800A2124:
    cmplwi r5, 0x0
    beq @L_800A213C
    lwz r3, 0x2d0(r5)
    lwz r0, 0x2d0(r29)
    cmpw r3, r0
    ble @L_800A2120
    @L_800A213C:
    cmplwi r5, 0x0
    bne @L_800A2174
    lwz r3, 0x4(r4)
    cmplwi r3, 0x0
    bne @L_800A2158
    stw r29, 0x0(r4)
    b @L_800A215C
    @L_800A2158:
    stw r29, 0x2e0(r3)
    @L_800A215C:
    stw r3, 0x2e4(r29)
    li r0, 0x0
    stw r0, 0x2e0(r29)
    lwz r3, 0x2dc(r29)
    stw r29, 0x4(r3)
    b @L_800A219C
    @L_800A2174:
    stw r5, 0x2e0(r29)
    lwz r3, 0x2e4(r5)
    stw r29, 0x2e4(r5)
    cmplwi r3, 0x0
    stw r3, 0x2e4(r29)
    bne @L_800A2198
    lwz r3, 0x2dc(r29)
    stw r29, 0x0(r3)
    b @L_800A219C
    @L_800A2198:
    stw r29, 0x2e0(r3)
    @L_800A219C:
    lwz r3, 0x2f0(r29)
    cmplwi r3, 0x0
    beq @L_800A21E0
    lwz r29, 0x8(r3)
    @L_800A21AC:
    lwz r0, 0x2cc(r29)
    cmpwi r0, 0x0
    bgt @L_800A21E0
    mr r3, r29
    bl fn_800A14EC
    lwz r0, 0x2d0(r29)
    addi r4, r3, 0x0
    cmpw r0, r4
    beq @L_800A21E0
    mr r3, r29
    bl fn_800A1528
    mr. r29, r3
    bne @L_800A21AC
    @L_800A21E0:
    lwz r0, RunQueueHint_8047A764(r13)
    cmpwi r0, 0x0
    beq @L_800A21F4
    li r3, 0x0
    bl SelectThread
    @L_800A21F4:
    mr r3, r31
    bl OSRestoreInterrupts
    mr r3, r30
    lwz r0, 0x2c(r1)
    lwz r31, 0x24(r1)
    lwz r30, 0x20(r1)
    lwz r29, 0x1c(r1)
    addi r1, r1, 0x28
    mtlr r0
    blr

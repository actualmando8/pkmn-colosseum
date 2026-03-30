    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x30(r1)
    stmw r27, 0x1c(r1)
    mr. r27, r3
    addi r28, r4, 0x0
    addi r31, r5, 0x0
    blt @L_800B30A0
    cmpwi r27, 0x2
    blt @L_800B30A8
    @L_800B30A0:
    li r3, -0x80
    b @L_800B31E0
    @L_800B30A8:
    lis r3, 0x8000
    lbz r0, 0x30e3(r3)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_800B30C0
    li r3, -0x3
    b @L_800B31E0
    @L_800B30C0:
    mulli r4, r27, 0x110
    lis r3, lbl_803FC620@ha
    addi r0, r3, lbl_803FC620@l
    add r30, r0, r4
    bl OSDisableInterrupts
    addi r29, r3, 0x0
    addi r3, r27, 0x0
    bl fn_800CF3C8
    cmpwi r3, -0x1
    bne @L_800B30F0
    li r30, -0x3
    b @L_800B31D4
    @L_800B30F0:
    cmpwi r3, 0x0
    bne @L_800B3100
    li r30, -0x1
    b @L_800B31D4
    @L_800B3100:
    lwz r0, 0x0(r30)
    cmpwi r0, 0x0
    beq @L_800B3148
    lwz r0, 0x24(r30)
    cmpwi r0, 0x1
    bge @L_800B3120
    li r30, -0x1
    b @L_800B31D4
    @L_800B3120:
    cmplwi r28, 0x0
    beq @L_800B3130
    lhz r0, 0x8(r30)
    stw r0, 0x0(r28)
    @L_800B3130:
    cmplwi r31, 0x0
    beq @L_800B3140
    lwz r0, 0xc(r30)
    stw r0, 0x0(r31)
    @L_800B3140:
    li r30, 0x0
    b @L_800B31D4
    @L_800B3148:
    mr r3, r27
    bl fn_800993A8
    rlwinm. r0, r3, 0, 28, 28
    beq @L_800B3160
    li r30, -0x2
    b @L_800B31D4
    @L_800B3160:
    addi r3, r27, 0x0
    addi r5, r1, 0x14
    li r4, 0x0
    bl fn_80099400
    cmpwi r3, 0x0
    bne @L_800B3180
    li r30, -0x1
    b @L_800B31D4
    @L_800B3180:
    lwz r3, 0x14(r1)
    bl fn_800B2FAC
    cmpwi r3, 0x0
    beq @L_800B31D0
    cmplwi r28, 0x0
    beq @L_800B31A4
    lwz r0, 0x14(r1)
    rlwinm r0, r0, 0, 24, 29
    stw r0, 0x0(r28)
    @L_800B31A4:
    cmplwi r31, 0x0
    beq @L_800B31C8
    lwz r4, 0x14(r1)
    lis r3, lbl_80312960@ha
    addi r0, r3, lbl_80312960@l
    rlwinm r3, r4, 23, 27, 29
    add r3, r0, r3
    lwz r0, 0x0(r3)
    stw r0, 0x0(r31)
    @L_800B31C8:
    li r30, 0x0
    b @L_800B31D4
    @L_800B31D0:
    li r30, -0x2
    @L_800B31D4:
    mr r3, r29
    bl OSRestoreInterrupts
    mr r3, r30
    @L_800B31E0:
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    addi r1, r1, 0x30
    mtlr r0
    blr

    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r23, 0x2c(r1)
    li r3, 0x28a0
    li r4, 0x20
    bl fn_800E2C04
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_8003F070
    bl fn_800E27B0
    b @L_8003F074
    @L_8003F070:
    li r3, 0x0
    @L_8003F074:
    mr r30, r3
    li r3, 0x320
    li r4, 0x20
    bl fn_800E2C04
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_8003F098
    bl fn_800E27B0
    b @L_8003F09C
    @L_8003F098:
    li r3, 0x0
    @L_8003F09C:
    mr r29, r3
    li r31, 0x0
    b @L_8003F178
    @L_8003F0A8:
    clrlwi r25, r31, 16
    lwz r3, lbl_8047A4E4(r13)
    slwi r0, r25, 1
    lhzx r3, r3, r0
    sthx r3, r29, r0
    mr r27, r3
    lwz r26, lbl_8047A4E0(r13)
    cmplwi r26, 0x0
    beq @L_8003F138
    cmplwi r3, 0x8000
    blt @L_8003F0D8
    clrlwi r27, r3, 18
    @L_8003F0D8:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r26
    clrlwi r4, r27, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r27
    li r3, 0x0
    bl fn_8025FDDC
    mr r28, r3
    mr r4, r27
    li r3, 0x0
    bl fn_8025FD34
    mr r27, r3
    mr r3, r26
    mr r4, r28
    bl fn_8011DFE0
    mr r3, r26
    mr r4, r27
    bl fn_8011DF90
    lwz r3, lbl_8047A4E0(r13)
    b @L_8003F13C
    @L_8003F138:
    li r3, 0x0
    @L_8003F13C:
    cmplwi r3, 0x0
    beq @L_8003F154
    bl fn_8011F5C8
    bl fn_8011E778
    bl fn_8011E760
    b @L_8003F158
    @L_8003F154:
    li r3, 0x0
    @L_8003F158:
    mulli r0, r25, 0x1a
    mr r5, r3
    li r4, 0x18
    add r25, r30, r0
    addi r3, r25, 0x2
    bl fn_800F96E4
    sth r31, 0x0(r25)
    addi r31, r31, 0x1
    @L_8003F178:
    lhz r24, lbl_8047A4E8(r13)
    clrlwi r0, r31, 16
    cmplw r0, r24
    blt @L_8003F0A8
    srwi r0, r24, 31
    add r0, r0, r24
    srawi r27, r0, 1
    b @L_8003F22C
    @L_8003F198:
    mulli r25, r27, 0x1a
    mr r28, r27
    b @L_8003F218
    @L_8003F1A4:
    subf r31, r27, r28
    mulli r0, r31, 0x1a
    add r26, r30, r0
    b @L_8003F1EC
    @L_8003F1B4:
    mr r4, r26
    addi r3, r1, 0x8
    li r5, 0x1a
    bl memcpy
    mr r3, r26
    mr r4, r23
    li r5, 0x1a
    bl memcpy
    mr r3, r23
    addi r4, r1, 0x8
    li r5, 0x1a
    bl memcpy
    subf r26, r25, r26
    subf r31, r27, r31
    @L_8003F1EC:
    cmpwi r31, 0x0
    blt @L_8003F214
    add r0, r31, r27
    addi r3, r26, 0x2
    mulli r0, r0, 0x1a
    add r23, r30, r0
    addi r4, r23, 0x2
    bl fn_800F9EE4
    cmpwi r3, 0x0
    bge @L_8003F1B4
    @L_8003F214:
    addi r28, r28, 0x1
    @L_8003F218:
    cmpw r28, r24
    blt @L_8003F1A4
    srwi r0, r27, 31
    add r0, r0, r27
    srawi r27, r0, 1
    @L_8003F22C:
    cmpwi r27, 0x0
    bgt @L_8003F198
    li r5, 0x0
    b @L_8003F260
    @L_8003F23C:
    clrlwi r0, r5, 16
    lwz r3, lbl_8047A4E4(r13)
    mulli r4, r0, 0x1a
    addi r5, r5, 0x1
    slwi r0, r0, 1
    lhzx r4, r30, r4
    slwi r4, r4, 1
    lhzx r4, r29, r4
    sthx r4, r3, r0
    @L_8003F260:
    lhz r0, lbl_8047A4E8(r13)
    clrlwi r3, r5, 16
    cmplw r3, r0
    blt @L_8003F23C
    cmplwi r30, 0x0
    beq @L_8003F29C
    mr r3, r30
    bl fn_800E202C
    mr r27, r3
    clrlwi r0, r27, 16
    cmplwi r0, 0x0
    beq @L_8003F29C
    bl fn_800E24B0
    mr r3, r27
    bl fn_800E209C
    @L_8003F29C:
    cmplwi r29, 0x0
    beq @L_8003F2C8
    mr r3, r29
    bl fn_800E202C
    mr r27, r3
    clrlwi r0, r27, 16
    cmplwi r0, 0x0
    beq @L_8003F2C8
    bl fn_800E24B0
    mr r3, r27
    bl fn_800E209C
    @L_8003F2C8:
    lmw r23, 0x2c(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr

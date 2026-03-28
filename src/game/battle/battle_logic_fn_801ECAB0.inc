    nofralloc
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x4(r1)
    stwu r1, -0x58(r1)
    stmw r20, 0x28(r1)
    addi r31, r4, 0x0
    beq @L_801ECAD4
    cmplwi r31, 0x0
    bne @L_801ECADC
@L_801ECAD4:
    li r3, 0x0
    b @L_801ECF00
@L_801ECADC:
    lwz r0, 0x0(r31)
    cmpwi r5, 0x1
    addi r4, r31, 0x50
    add r27, r31, r0
    addi r27, r27, 0x50
    bne @L_801ECB0C
    lwz r0, 0x4(r31)
    addi r30, r3, 0x0
    li r26, 0x1
    slwi r0, r0, 1
    add r29, r3, r0
    b @L_801ECB18
@L_801ECB0C:
    addi r30, r3, 0x0
    addi r29, r3, 0x2
    li r26, 0x2
@L_801ECB18:
    lwz r0, 0x0(r31)
    cmplwi r0, 0x0
    bne @L_801ECC74
    addi r3, r1, 0x14
    bl fn_801ECFA4
    lis r25, 0x1
    lha r21, 0x48(r31)
    lis r23, 0x8000
    lha r20, 0x4a(r31)
    slwi r27, r26, 1
    subi r26, r25, 0x1
    subi r24, r23, 0x1
    li r22, 0x0
    b @L_801ECC64
@L_801ECB50:
    addi r3, r1, 0x14
    bl fn_801ECF14
    lbz r5, 0x1c(r1)
    extsh r6, r20
    lbz r0, 0x1d(r1)
    extsh r4, r21
    slwi r5, r5, 2
    add r5, r31, r5
    lha r7, 0xa(r5)
    slw r0, r3, r0
    lha r3, 0x8(r5)
    slwi r5, r0, 11
    mullw r6, r7, r6
    mullw r4, r3, r4
    srawi r3, r6, 31
    srawi r0, r4, 31
    addc r4, r6, r4
    adde r3, r3, r0
    srawi r0, r5, 31
    addc r4, r4, r5
    adde r3, r3, r0
    li r5, 0x5
    bl fn_800C4C50
    and r0, r4, r26
    clrlwi r0, r0, 16
    cmplwi r0, 0x8000
    li r6, 0x0
    and r7, r3, r6
    ble @L_801ECBD0
    addc r4, r4, r25
    adde r3, r3, r6
    b @L_801ECBF0
@L_801ECBD0:
    bne @L_801ECBF0
    and r0, r4, r25
    xor r5, r0, r6
    xor r0, r7, r6
    or. r0, r5, r0
    beq @L_801ECBF0
    addc r4, r4, r25
    adde r3, r3, r6
@L_801ECBF0:
    li r0, 0x0
    xoris r6, r0, 0x8000
    xoris r5, r3, 0x8000
    subfc r0, r4, r24
    subfe r5, r5, r6
    subfe r5, r6, r6
    neg. r5, r5
    beq @L_801ECC18
    subi r4, r23, 0x1
    li r3, 0x0
@L_801ECC18:
    li r0, -0x1
    xoris r5, r0, 0x8000
    xoris r6, r3, 0x8000
    subfc r0, r23, r4
    subfe r5, r5, r6
    subfe r5, r6, r6
    neg. r5, r5
    beq @L_801ECC40
    lis r4, 0x8000
    li r3, -0x1
@L_801ECC40:
    li r5, 0x10
    bl fn_800C4C98
    sth r4, 0x0(r29)
    mr r20, r21
    addi r21, r4, 0x0
    sth r4, 0x0(r30)
    add r29, r29, r27
    add r30, r30, r27
    addi r22, r22, 0x1
@L_801ECC64:
    lwz r0, 0x4(r31)
    cmplw r22, r0
    blt @L_801ECB50
    b @L_801ECEFC
@L_801ECC74:
    addi r3, r1, 0x14
    bl fn_801ECFA4
    lis r24, 0x1
    lha r22, 0x48(r31)
    lis r25, 0x8000
    lha r20, 0x4a(r31)
    slwi r28, r26, 1
    subi r23, r24, 0x1
    subi r26, r25, 0x1
    li r21, 0x0
    b @L_801ECDAC
@L_801ECCA0:
    addi r3, r1, 0x14
    bl fn_801ECF14
    lbz r5, 0x1c(r1)
    extsh r6, r20
    lbz r0, 0x1d(r1)
    extsh r4, r22
    slwi r5, r5, 2
    add r5, r31, r5
    lha r7, 0xa(r5)
    slw r0, r3, r0
    lha r3, 0x8(r5)
    slwi r5, r0, 11
    mullw r6, r7, r6
    mullw r4, r3, r4
    srawi r3, r6, 31
    srawi r0, r4, 31
    addc r4, r6, r4
    adde r3, r3, r0
    srawi r0, r5, 31
    addc r4, r4, r5
    adde r3, r3, r0
    li r5, 0x5
    bl fn_800C4C50
    and r0, r4, r23
    clrlwi r0, r0, 16
    cmplwi r0, 0x8000
    li r6, 0x0
    and r7, r3, r6
    ble @L_801ECD20
    addc r4, r4, r24
    adde r3, r3, r6
    b @L_801ECD40
@L_801ECD20:
    bne @L_801ECD40
    and r0, r4, r24
    xor r5, r0, r6
    xor r0, r7, r6
    or. r0, r5, r0
    beq @L_801ECD40
    addc r4, r4, r24
    adde r3, r3, r6
@L_801ECD40:
    li r7, 0x0
    xoris r6, r7, 0x8000
    xoris r5, r3, 0x8000
    subfc r0, r4, r26
    subfe r5, r5, r6
    subfe r5, r6, r6
    neg. r5, r5
    beq @L_801ECD68
    subi r4, r25, 0x1
    addi r3, r7, 0x0
@L_801ECD68:
    li r7, -0x1
    xoris r6, r3, 0x8000
    xoris r5, r7, 0x8000
    subfc r0, r25, r4
    subfe r5, r5, r6
    subfe r5, r6, r6
    neg. r5, r5
    beq @L_801ECD90
    lis r4, 0x8000
    addi r3, r7, 0x0
@L_801ECD90:
    li r5, 0x10
    bl fn_800C4C98
    sth r4, 0x0(r29)
    addi r20, r22, 0x0
    addi r22, r4, 0x0
    add r29, r29, r28
    addi r21, r21, 0x1
@L_801ECDAC:
    lwz r0, 0x4(r31)
    cmplw r21, r0
    blt @L_801ECCA0
    addi r3, r1, 0x14
    addi r4, r27, 0x0
    bl fn_801ECFA4
    lis r26, 0x1
    lha r23, 0x4c(r31)
    lis r27, 0x8000
    lha r21, 0x4e(r31)
    subi r25, r26, 0x1
    subi r29, r27, 0x1
    li r22, 0x0
    b @L_801ECEF0
@L_801ECDE4:
    addi r3, r1, 0x14
    bl fn_801ECF14
    lbz r5, 0x1c(r1)
    extsh r6, r21
    lbz r0, 0x1d(r1)
    extsh r4, r23
    slwi r5, r5, 2
    add r5, r31, r5
    lha r7, 0x2a(r5)
    slw r0, r3, r0
    lha r3, 0x28(r5)
    slwi r5, r0, 11
    mullw r6, r7, r6
    mullw r4, r3, r4
    srawi r3, r6, 31
    srawi r0, r4, 31
    addc r4, r6, r4
    adde r3, r3, r0
    srawi r0, r5, 31
    addc r4, r4, r5
    adde r3, r3, r0
    li r5, 0x5
    bl fn_800C4C50
    and r0, r4, r25
    clrlwi r0, r0, 16
    cmplwi r0, 0x8000
    li r6, 0x0
    and r7, r3, r6
    ble @L_801ECE64
    addc r4, r4, r26
    adde r3, r3, r6
    b @L_801ECE84
@L_801ECE64:
    bne @L_801ECE84
    and r0, r4, r26
    xor r5, r0, r6
    xor r0, r7, r6
    or. r0, r5, r0
    beq @L_801ECE84
    addc r4, r4, r26
    adde r3, r3, r6
@L_801ECE84:
    li r7, 0x0
    xoris r6, r7, 0x8000
    xoris r5, r3, 0x8000
    subfc r0, r4, r29
    subfe r5, r5, r6
    subfe r5, r6, r6
    neg. r5, r5
    beq @L_801ECEAC
    subi r4, r27, 0x1
    addi r3, r7, 0x0
@L_801ECEAC:
    li r7, -0x1
    xoris r6, r3, 0x8000
    xoris r5, r7, 0x8000
    subfc r0, r27, r4
    subfe r5, r5, r6
    subfe r5, r6, r6
    neg. r5, r5
    beq @L_801ECED4
    lis r4, 0x8000
    addi r3, r7, 0x0
@L_801ECED4:
    li r5, 0x10
    bl fn_800C4C98
    sth r4, 0x0(r30)
    addi r21, r23, 0x0
    addi r23, r4, 0x0
    add r30, r30, r28
    addi r22, r22, 0x1
@L_801ECEF0:
    lwz r0, 0x4(r31)
    cmplw r22, r0
    blt @L_801ECDE4
@L_801ECEFC:
    lwz r3, 0x4(r31)
@L_801ECF00:
    lmw r20, 0x28(r1)
    lwz r0, 0x5c(r1)
    addi r1, r1, 0x58
    mtlr r0
    blr

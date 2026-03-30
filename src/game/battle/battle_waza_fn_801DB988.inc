    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r27, r3
    lwz r31, 0x24(r3)
    bl fn_800D3088
    lwz r0, 0x0(r27)
    mr r30, r3
    li r29, 0x0
    li r28, 0x0
    add r0, r0, r30
    li r6, 0x2
    stw r0, 0x0(r27)
    lwz r26, 0x3c(r27)
    lhz r4, 0x2c(r27)
    lhz r5, 0x2e(r27)
    mr r3, r26
    bl fn_801DA74C
    lwz r0, 0x0(r27)
    cmpw r3, r0
    ble @L_801DB9E4
    li r28, 0x1
@L_801DB9E4:
    lbz r0, 0x16(r26)
    cmplwi r0, 0x0
    bne @L_801DBAD4
    lbz r0, 0x17(r26)
    cmplwi r0, 0x0
    beq @L_801DBAD4
    lhz r4, 0x2c(r27)
    mr r3, r26
    lhz r5, 0x2e(r27)
    li r6, 0x1
    bl fn_801DA74C
    lwz r0, 0x0(r27)
    cmpw r3, r0
    bge @L_801DBAD4
    li r3, -0x1
    b @L_801DBAFC
    b @L_801DBAD4
@L_801DBA28:
    lwz r0, 0x6c(r31)
    cmpwi r0, 0x1
    beq @L_801DBA90
    bge @L_801DBAD0
    cmpwi r0, 0x0
    bge @L_801DBA48
    b @L_801DBAD0
    b @L_801DBAD0
@L_801DBA48:
    lwz r3, 0x70(r31)
    lwz r0, 0x0(r27)
    cmpw r3, r0
    bgt @L_801DBA84
    mr r3, r31
    bl fn_801D84F4
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801DBA70
    addi r28, r28, 0x1
    b @L_801DBAD0
@L_801DBA70:
    lis r3, -32728
    addi r3, r3, -26384
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_801DBAD0
@L_801DBA84:
    stw r3, 0x74(r31)
    addi r29, r29, 0x1
    b @L_801DBAD0
@L_801DBA90:
    mr r3, r31
    mr r4, r30
    bl fn_801D81CC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801DBACC
    mr r3, r31
    li r4, 0x0
    bl fn_801D7E58
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801DBAD0
    lis r3, -32728
    addi r3, r3, -26328
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_801DBAD0
@L_801DBACC:
    addi r28, r28, 0x1
@L_801DBAD0:
    lwz r31, 0xa8(r31)
@L_801DBAD4:
    cmplwi r31, 0x0
    bne @L_801DBA28
    add. r0, r28, r29
    bne @L_801DBAEC
    li r3, 0x0
    b @L_801DBAFC
@L_801DBAEC:
    lbz r0, 0x15(r27)
    extsb r0, r0
    cntlzw r0, r0
    rlwinm r3, r0, 27, 5, 31
@L_801DBAFC:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

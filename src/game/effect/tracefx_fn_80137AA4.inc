    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr. r29, r3
    beq @L_80137CEC
    lhz r28, 0x20(r29)
    lhz r27, 0x22(r29)
    lhz r3, 0x24(r29)
    lhz r4, 0x26(r29)
    bl fn_800F9318
    lwz r0, 0x14(r29)
    mr r30, r3
    cmplwi r0, 0x0
    bne @L_80137AE8
    li r3, 0x0
    b @L_80137D00
@L_80137AE8:
    cmplwi r27, 0x0
    beq @L_80137AF8
    cmplwi r28, 0x0
    bne @L_80137B00
@L_80137AF8:
    li r3, 0x0
    b @L_80137D00
@L_80137B00:
    cmplwi r30, 0x0
    bne @L_80137B10
    li r3, 0x0
    b @L_80137D00
@L_80137B10:
    lhz r4, 0x28(r29)
    bl fn_800EE150
    cmplwi r3, 0x0
    bne @L_80137B28
    li r3, 0x0
    b @L_80137D00
@L_80137B28:
    bl fn_800EE828
    lhz r4, 0x2a(r29)
    mr r3, r30
    bl fn_800EE150
    cmplwi r3, 0x0
    bne @L_80137B48
    li r3, 0x0
    b @L_80137D00
@L_80137B48:
    bl fn_800EE828
    rlwinm r0, r28, 31, 17, 31
    cmpw r27, r0
    ble @L_80137B60
    sth r0, 0x22(r29)
    mr r27, r0
@L_80137B60:
    clrlslwi r28, r27, 16, 5
    rlwinm r30, r27, 0, 16, 31
    mr r3, r28
    bl fn_800E3534
    rlwinm. r0, r3, 0, 16, 31
    bne @L_80137B80
    li r3, 0x0
    b @L_80137D00
@L_80137B80:
    sth r3, 0xc(r29)
    bl fn_800E27B0
    mr r31, r3
    mr r5, r28
    stw r31, 0x4(r29)
    li r4, 0x0
    stw r31, 0x0(r29)
    bl memset
    subi r6, r30, 0x1
    mr r0, r30
    rlwinm r3, r6, 5, 0, 26
    li r7, 0x0
    add r5, r31, r3
    b @L_80137C20
@L_80137BB8:
    rlwinm. r3, r7, 0, 16, 31
    bne @L_80137BD0
    clrlslwi r3, r7, 16, 5
    addi r3, r3, 0x1c
    stwx r5, r31, r3
    b @L_80137BE8
@L_80137BD0:
    subi r4, r3, 0x1
    clrlslwi r3, r7, 16, 5
    rlwinm r4, r4, 5, 0, 26
    add r4, r31, r4
    addi r3, r3, 0x1c
    stwx r4, r31, r3
@L_80137BE8:
    rlwinm r3, r7, 0, 16, 31
    cmpw r3, r6
    bne @L_80137C04
    clrlslwi r3, r7, 16, 5
    addi r3, r3, 0x18
    stwx r31, r31, r3
    b @L_80137C1C
@L_80137C04:
    addi r4, r3, 0x1
    clrlslwi r3, r7, 16, 5
    rlwinm r4, r4, 5, 0, 26
    add r4, r31, r4
    addi r3, r3, 0x18
    stwx r4, r31, r3
@L_80137C1C:
    addi r7, r7, 0x1
@L_80137C20:
    rlwinm r3, r7, 0, 16, 31
    cmplw r3, r0
    blt @L_80137BB8
    rlwinm r3, r30, 4, 0, 27
    bl fn_800E3534
    rlwinm. r0, r3, 0, 16, 31
    bne @L_80137C54
    lhz r3, 0xc(r29)
    bl fn_800E24B0
    lhz r3, 0xc(r29)
    bl fn_800E209C
    li r3, 0x0
    b @L_80137D00
@L_80137C54:
    sth r3, 0xe(r29)
    bl fn_800E27B0
    stw r3, 0x8(r29)
    li r5, 0x0
    lis r3, 0x4330
    lfd f3, -25960(r2)
    sth r5, 0x1c(r29)
    lfs f4, -25968(r2)
    sth r5, 0x1e(r29)
    lfd f2, -25952(r2)
    lhz r4, 0x22(r29)
    stw r3, 0x8(r1)
    subi r0, r4, 0x1
    lwz r6, 0x8(r29)
    xoris r0, r0, 0x8000
    lfs f0, -25964(r2)
    stw r0, 0xc(r1)
    lfd f1, 0x8(r1)
    fsubs f1, f1, f3
    fdivs f3, f4, f1
    b @L_80137CD8
@L_80137CA8:
    rlwinm r0, r5, 0, 16, 31
    stw r3, 0x8(r1)
    addi r5, r5, 0x1
    stw r0, 0xc(r1)
    lfd f1, 0x8(r1)
    fsubs f1, f1, f2
    fmuls f1, f1, f3
    stfs f1, 0x0(r6)
    stfs f0, 0x4(r6)
    stfs f1, 0x8(r6)
    stfs f4, 0xc(r6)
    addi r6, r6, 0x10
@L_80137CD8:
    rlwinm r0, r5, 0, 16, 31
    cmplw r0, r4
    blt @L_80137CA8
    li r3, 0x1
    b @L_80137D00
@L_80137CEC:
    lis r3, -32729
    addi r3, r3, 11016
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
@L_80137D00:
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

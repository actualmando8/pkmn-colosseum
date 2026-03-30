    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r29, r3
    mr r30, r4
    mr r25, r5
    lis r5, -32729
    li r24, 0x0
    addi r27, r5, 8704
    li r31, 0x0
    bl fn_800F9318
    mr r26, r3
    mr r5, r25
    addi r4, r26, 0x60
    bl fn_80191F64
    mr r3, r26
    addi r4, r27, 0xac
    bl fn_80191ECC
    mr r26, r3
    cmplwi r26, 0x0
    bne @L_80114B40
    li r3, 0x0
    b @L_80114C94
@L_80114B40:
    bl fn_80115BD8
    mr r4, r30
    bl fn_80115584
    lwz r0, 0x0(r26)
    cmplwi r0, 0x0
    beq @L_80114B90
    rlwinm r0, r30, 0, 1, 15
    li r25, 0x0
    ori r28, r0, 0x1000
    b @L_80114B80
@L_80114B68:
    mr r4, r29
    or r5, r28, r24
    li r6, 0x0
    bl fn_800F9378
    addi r25, r25, 0x4
    addi r24, r24, 0x1
@L_80114B80:
    lwz r3, 0x0(r26)
    lwzx r3, r3, r25
    cmplwi r3, 0x0
    bne @L_80114B68
@L_80114B90:
    lwz r0, 0x8(r26)
    cmplwi r0, 0x0
    beq @L_80114C08
    rlwinm r0, r30, 0, 1, 15
    li r24, 0x0
    ori r25, r0, 0x1600
    lis r3, -32751
    addi r28, r3, 20736
    b @L_80114BF8
@L_80114BB4:
    bl fn_800DCE4C
    cmplwi r3, 0x0
    bne @L_80114BE0
    mr r4, r31
    addi r3, r27, 0x298
    crxor 6, 6, 6
    bl fn_800DD970
    addi r3, r27, 0x2d0
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_80114BF0
@L_80114BE0:
    mr r4, r29
    mr r6, r28
    or r5, r25, r31
    bl fn_800F9378
@L_80114BF0:
    addi r24, r24, 0x4
    addi r31, r31, 0x1
@L_80114BF8:
    lwz r3, 0x8(r26)
    lwzx r3, r3, r24
    cmplwi r3, 0x0
    bne @L_80114BB4
@L_80114C08:
    cmplwi r26, 0x0
    rlwinm r24, r30, 0, 1, 15
    ori r25, r24, 0x1800
    bne @L_80114C28
    addi r3, r27, 0x1cc
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_80114C5C
@L_80114C28:
    lwz r3, 0x4(r26)
    bl fn_800D27FC
    cmplwi r3, 0x0
    bne @L_80114C48
    addi r3, r27, 0x200
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_80114C5C
@L_80114C48:
    lis r5, -32751
    mr r4, r29
    addi r6, r5, 20700
    mr r5, r25
    bl fn_800F9378
@L_80114C5C:
    lwz r3, 0xc(r26)
    cmplwi r3, 0x0
    beq @L_80114C84
    lwz r3, 0x0(r3)
    cmplwi r3, 0x0
    beq @L_80114C84
    mr r4, r29
    ori r5, r24, 0x1a00
    li r6, 0x0
    bl fn_800F9378
@L_80114C84:
    mr r3, r29
    mr r4, r30
    bl fn_801134E4
    mr r3, r26
@L_80114C94:
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

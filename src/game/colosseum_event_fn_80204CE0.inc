    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r21, 0x14(r1)
    mr r22, r4
    mr r23, r5
    mr r24, r6
    lbz r29, 0x4b(r1)
    mr r26, r8
    mr r21, r3
    mr r25, r7
    mr r27, r9
    mr r28, r10
    rlwinm r30, r8, 0, 16, 31
    li r4, 0x0
    li r5, 0xe5
    li r6, 0x0
    bl fn_8012640C
    mr. r31, r3
    bne @L_80204D38
    li r3, 0x0
    b @L_80204DD0
@L_80204D38:
    mr r4, r30
    mr r5, r27
    mr r6, r28
    bl fn_8020A398
    mr r3, r31
    mr r7, r29
    li r4, 0x0
    li r5, 0x21
    li r6, 0x0
    bl fn_80142B24
    mr r3, r21
    li r4, 0x0
    li r5, 0xfe
    li r6, 0x0
    bl fn_8012640C
    mr. r31, r3
    bne @L_80204D84
    li r31, 0x0
    b @L_80204DBC
@L_80204D84:
    mr r4, r22
    mr r5, r21
    mr r6, r23
    mr r7, r24
    mr r8, r25
    bl fn_801F11CC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80204DB8
    mr r3, r31
    mr r4, r26
    bl fn_8020D878
    b @L_80204DBC
@L_80204DB8:
    li r31, 0x0
@L_80204DBC:
    cmplwi r31, 0x0
    bne @L_80204DCC
    li r3, 0x0
    b @L_80204DD0
@L_80204DCC:
    mr r3, r31
@L_80204DD0:
    lmw r21, 0x14(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr

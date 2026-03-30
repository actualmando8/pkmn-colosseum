    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r22, 0x8(r1)
    mr r23, r4
    mr r24, r5
    mr r25, r6
    lbz r30, 0x3b(r1)
    mr r27, r8
    mr r22, r3
    mr r26, r7
    mr r28, r9
    mr r29, r10
    clrlwi r31, r8, 16
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80204FC4
    li r3, 0x0
    b @L_80205048
@L_80204FC4:
    mr r4, r29
    mr r5, r31
    mr r6, r28
    mr r7, r30
    bl fn_802099AC
    mr r3, r22
    li r4, 0x0
    li r5, 0xfe
    li r6, 0x0
    bl fn_8012640C
    mr. r31, r3
    bne @L_80204FFC
    li r31, 0x0
    b @L_80205034
@L_80204FFC:
    mr r4, r23
    mr r5, r22
    mr r6, r24
    mr r7, r25
    mr r8, r26
    bl fn_801F11CC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80205030
    mr r3, r31
    mr r4, r27
    bl fn_8020D878
    b @L_80205034
@L_80205030:
    li r31, 0x0
@L_80205034:
    cmplwi r31, 0x0
    bne @L_80205044
    li r3, 0x0
    b @L_80205048
@L_80205044:
    mr r3, r31
@L_80205048:
    lmw r22, 0x8(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

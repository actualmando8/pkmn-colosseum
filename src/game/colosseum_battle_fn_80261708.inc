    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r4, 0x0
    li r6, 0x0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r31, r5
    mr r28, r3
    li r5, 0xee
    bl fn_8012640C
    mr. r29, r3
    bne @L_80261740
    li r3, 0x1
    b @L_80261838
@L_80261740:
    bl fn_801DAC78
    rlwinm. r0, r3, 0, 16, 31
    bne @L_80261754
    li r3, 0x1
    b @L_80261838
@L_80261754:
    lbz r0, 0x0(r31)
    cmplwi r0, 0x0
    beq @L_80261814
    lwz r30, 0x8(r31)
    stw r28, 0x0(r30)
    sth r3, 0x4(r30)
    mr r3, r29
    bl fn_801DA354
    stb r3, 0x6(r30)
    mr r3, r29
    bl fn_801D9E1C
    stb r3, 0x7(r30)
    mr r3, r29
    bl fn_801DE164
    stw r3, 0x8(r30)
    cmplwi r29, 0x0
    lwz r9, 0x4(r31)
    bne @L_802617A4
    li r0, 0x0
    b @L_802617F4
@L_802617A4:
    li r7, 0x0
    li r3, 0x0
    mr r6, r7
    li r0, 0x2
@L_802617B4:
    mr r4, r6
    add r8, r9, r3
    mtctr r0
@L_802617C0:
    add r10, r8, r4
    lwz r5, 0x4(r10)
    cmplw r5, r29
    bne @L_802617D8
    addi r0, r10, 0x4
    b @L_802617F4
@L_802617D8:
    addi r4, r4, 0x4
    bdnz @L_802617C0
    addi r7, r7, 0x1
    addi r3, r3, 0x10
    cmpwi r7, 0x4
    blt @L_802617B4
    li r0, 0x0
@L_802617F4:
    stw r0, 0xc(r30)
    mr r3, r29
    bl fn_801DAC3C
    addi r4, r30, 0x10
    bl fn_800E9C6C
    lwz r3, 0x8(r31)
    addi r0, r3, 0x7c
    stw r0, 0x8(r31)
@L_80261814:
    lbz r0, 0x1(r31)
    cmplwi r0, 0x0
    beq @L_80261828
    mr r3, r29
    bl fn_801DB100
@L_80261828:
    lhz r4, 0x2(r31)
    li r3, 0x1
    addi r0, r4, 0x1
    sth r0, 0x2(r31)
@L_80261838:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

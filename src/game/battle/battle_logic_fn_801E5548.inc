    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x38(r1)
    stmw r27, 0x24(r1)
    mr. r27, r3
    addi r28, r4, 0x0
    addi r29, r5, 0x0
    addi r30, r6, 0x0
    addi r31, r7, 0x0
    beq @L_801E573C
    cmplwi r28, 0x0
    beq @L_801E5744
    cmplwi r29, 0x0
    beq @L_801E5744
    cmplwi r30, 0x0
    beq @L_801E5744
    cmplwi r31, 0x0
    beq @L_801E574C
    bl PPCMfhid2
    rlwinm. r0, r3, 0, 3, 3
    beq @L_801E576C
    lwz r0, -21100(r13)
    cmpwi r0, 0x0
    beq @L_801E5774
    stw r31, -21108(r13)
    li r4, 0x6bc
    lwz r3, -21108(r13)
    addi r0, r3, 0x1f
    clrrwi r3, r0, 5
    stw r3, -21104(r13)
    addi r0, r3, 0x6bc
    stw r0, -21108(r13)
    lwz r3, -21104(r13)
    bl fn_8009B388
    lwz r3, -21104(r13)
    li r4, 0x21
    li r0, 0x0
    stw r4, 0x6a4(r3)
    li r31, 0x0
    lwz r3, -21104(r13)
    sth r0, 0x698(r3)
    lwz r3, -21104(r13)
    stw r27, 0x69c(r3)
@L_801E55F4:
    lwz r4, -21104(r13)
    lwz r3, 0x69c(r4)
    addi r0, r3, 0x1
    stw r0, 0x69c(r4)
    lbz r0, 0x0(r3)
    cmplwi r0, 0xff
    bne @L_801E575C
    b @L_801E5620
@L_801E5614:
    lwz r3, 0x0(r4)
    addi r0, r3, 0x1
    stw r0, 0x0(r4)
@L_801E5620:
    lwz r3, -21104(r13)
    addi r4, r3, 0x69c
    lwz r3, 0x69c(r3)
    lbz r0, 0x0(r3)
    cmplwi r0, 0xff
    beq @L_801E5614
    addi r0, r3, 0x1
    stw r0, 0x0(r4)
    lbz r0, 0x0(r3)
    cmplwi r0, 0xd7
    bgt @L_801E567C
    cmplwi r0, 0xc4
    bne @L_801E5664
    bl fn_801E5DE4
    clrlwi. r0, r3, 24
    bne @L_801E5764
    b @L_801E5718
@L_801E5664:
    cmplwi r0, 0xc0
    bne @L_801E5754
    bl fn_801E57D0
    clrlwi. r0, r3, 24
    bne @L_801E5764
    b @L_801E5718
@L_801E567C:
    cmplwi r0, 0xd8
    blt @L_801E56DC
    cmplwi r0, 0xdf
    bgt @L_801E56DC
    cmplwi r0, 0xdd
    bne @L_801E569C
    bl fn_801E62D8
    b @L_801E5718
@L_801E569C:
    cmplwi r0, 0xdb
    bne @L_801E56B4
    bl fn_801E5A28
    clrlwi. r0, r3, 24
    bne @L_801E5764
    b @L_801E5718
@L_801E56B4:
    cmplwi r0, 0xda
    bne @L_801E56D0
    bl fn_801E590C
    clrlwi. r0, r3, 24
    bne @L_801E5764
    li r31, 0x1
    b @L_801E5718
@L_801E56D0:
    cmplwi r0, 0xd8
    beq @L_801E5718
    b @L_801E5754
@L_801E56DC:
    cmplwi r0, 0xe0
    blt @L_801E5718
    blt @L_801E56F0
    cmplwi r0, 0xef
    ble @L_801E56F8
@L_801E56F0:
    cmplwi r0, 0xfe
    bne @L_801E5754
@L_801E56F8:
    lwz r3, -21104(r13)
    lwz r4, 0x69c(r3)
    addi r5, r3, 0x69c
    lbz r3, 0x0(r4)
    lbz r0, 0x1(r4)
    rlwimi r0, r3, 8, 16, 23
    add r0, r4, r0
    stw r0, 0x0(r5)
@L_801E5718:
    clrlwi. r0, r31, 24
    beq @L_801E55F4
    bl fn_801E578C
    addi r3, r28, 0x0
    addi r4, r29, 0x0
    addi r5, r30, 0x0
    bl fn_801E6578
    li r3, 0x0
    b @L_801E5778
@L_801E573C:
    li r3, 0x19
    b @L_801E5778
@L_801E5744:
    li r3, 0x1b
    b @L_801E5778
@L_801E574C:
    li r3, 0x1a
    b @L_801E5778
@L_801E5754:
    li r3, 0xb
    b @L_801E5778
@L_801E575C:
    li r3, 0x3
    b @L_801E5778
@L_801E5764:
    clrlwi r3, r3, 24
    b @L_801E5778
@L_801E576C:
    li r3, 0x1c
    b @L_801E5778
@L_801E5774:
    li r3, 0x1d
@L_801E5778:
    lmw r27, 0x24(r1)
    lwz r0, 0x3c(r1)
    addi r1, r1, 0x38
    mtlr r0
    blr

    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r5, 0x0
    stw r0, 0x34(r1)
    stmw r26, 0x18(r1)
    mr r28, r3
    mr r29, r4
    sth r5, 0x8(r1)
    b @L_801F8804
    @L_801F87F0:
    lhz r3, 0x8(r1)
    clrlslwi r0, r0, 16, 2
    stwx r5, r29, r0
    addi r0, r3, 0x1
    sth r0, 0x8(r1)
    @L_801F8804:
    lhz r0, 0x8(r1)
    cmplwi r0, 0x6
    blt @L_801F87F0
    li r0, 0x0
    li r30, 0x0
    sth r0, 0x8(r1)
    @L_801F881C:
    cmplwi r28, 0x0
    bne @L_801F882C
    li r4, 0x0
    b @L_801F89DC
    @L_801F882C:
    addic. r0, r1, 0x8
    bne @L_801F89B8
    li r4, 0x0
    b @L_801F89DC
    b @L_801F89B8
    @L_801F8840:
    mr r3, r28
    li r4, 0x0
    li r5, 0x45
    bl fn_801FB1C0
    mr r31, r3
    bl fn_80206608
    clrlwi. r0, r3, 24
    bne @L_801F8868
    li r0, 0x1
    b @L_801F89A4
    @L_801F8868:
    cmplwi r28, 0x0
    bne @L_801F8878
    li r26, 0x0
    b @L_801F88F0
    @L_801F8878:
    cmplwi r31, 0x0
    bne @L_801F8888
    li r26, 0x0
    b @L_801F88F0
    @L_801F8888:
    li r27, 0x0
    b @L_801F88E0
    @L_801F8890:
    mr r3, r28
    mr r6, r27
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    mr r26, r3
    bl fn_80206780
    clrlwi. r0, r3, 24
    bne @L_801F88B8
    li r26, 0x0
    @L_801F88B8:
    cmplwi r26, 0x0
    beq @L_801F88DC
    mr r3, r31
    mr r4, r26
    bl fn_80204928
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F88DC
    b @L_801F88F0
    @L_801F88DC:
    addi r27, r27, 0x1
    @L_801F88E0:
    clrlwi r0, r27, 16
    cmplwi r0, 0x2
    blt @L_801F8890
    li r26, 0x0
    @L_801F88F0:
    cmplwi r26, 0x0
    beq @L_801F8900
    li r0, 0x2
    b @L_801F89A4
    @L_801F8900:
    cmplwi r28, 0x0
    bne @L_801F8910
    li r0, 0x0
    b @L_801F898C
    @L_801F8910:
    cmplwi r31, 0x0
    bne @L_801F8920
    li r0, 0x0
    b @L_801F898C
    @L_801F8920:
    li r27, 0x0
    b @L_801F897C
    @L_801F8928:
    mr r3, r28
    mr r6, r27
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    mr r26, r3
    bl fn_80206780
    clrlwi. r0, r3, 24
    bne @L_801F8950
    li r26, 0x0
    @L_801F8950:
    cmplwi r26, 0x0
    beq @L_801F8978
    mr r3, r26
    mr r4, r31
    bl fn_80204854
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F8978
    li r0, 0x1
    b @L_801F898C
    @L_801F8978:
    addi r27, r27, 0x1
    @L_801F897C:
    clrlwi r0, r27, 16
    cmplwi r0, 0x2
    blt @L_801F8928
    li r0, 0x0
    @L_801F898C:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_801F89A0
    li r0, 0x3
    b @L_801F89A4
    @L_801F89A0:
    li r0, 0x0
    @L_801F89A4:
    clrlwi. r0, r0, 24
    beq @L_801F89C4
    lhz r3, 0x8(r1)
    addi r0, r3, 0x1
    sth r0, 0x8(r1)
    @L_801F89B8:
    lhz r6, 0x8(r1)
    cmplwi r6, 0x6
    blt @L_801F8840
    @L_801F89C4:
    lhz r0, 0x8(r1)
    cmplwi r0, 0x6
    blt @L_801F89D8
    li r4, 0x0
    b @L_801F89DC
    @L_801F89D8:
    mr r4, r31
    @L_801F89DC:
    cmplwi r4, 0x0
    beq @L_801F8A00
    lhz r3, 0x8(r1)
    clrlslwi r0, r30, 16, 2
    stwx r4, r29, r0
    addi r30, r30, 0x1
    addi r0, r3, 0x1
    sth r0, 0x8(r1)
    b @L_801F881C
    @L_801F8A00:
    mr r3, r30
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

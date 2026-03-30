    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r22, 0x8(r1)
    mr r29, r3
    li r4, 0x0
    bl fn_801040D0
    mr r0, r3
    mr r3, r29
    mr r30, r0
    li r4, 0x1
    bl fn_801040D0
    mr r3, r29
    li r4, 0x2
    bl fn_801040D0
    lbz r0, 0x2(r29)
    mr r31, r3
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8006E0A8
    lbz r0, 0x1(r29)
    extsb r0, r0
    cmpwi r0, 0x3
    beq @L_8006E07C
    bge @L_8006E0A8
    cmpwi r0, 0x0
    beq @L_8006DC9C
    bge @L_8006DF50
    b @L_8006E0A8
    @L_8006DC9C:
    li r25, 0x0
    mr r26, r25
    lis r3, lbl_803B6D68@ha
    addi r23, r3, lbl_803B6D68@l
    @L_8006DCAC:
    mr r3, r30
    clrlwi r4, r26, 16
    bl fn_8012AC08
    mr r24, r3
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8006DCDC
    mr r0, r25
    addi r25, r25, 0x1
    slwi r0, r0, 2
    stwx r24, r23, r0
    @L_8006DCDC:
    addi r26, r26, 0x1
    cmpwi r26, 0x6
    blt @L_8006DCAC
    lis r4, lbl_803B6D68@ha
    slwi r0, r25, 2
    addi r5, r4, lbl_803B6D68@l
    li r6, 0x0
    stwx r6, r5, r0
    lis r3, fn_8006E128@ha
    addi r4, r3, fn_8006E128@l
    stw r6, 0x1c(r5)
    li r3, 0x0
    bl fn_8010B01C
    lis r3, lbl_8026858C@ha
    li r26, 0x0
    addi r27, r3, lbl_8026858C@l
    lis r3, 0x2aab
    subi r28, r3, 0x5555
    @L_8006DD24:
    mulhw r4, r28, r26
    mr r3, r30
    srwi r0, r4, 31
    add r0, r4, r0
    mulli r0, r0, 0x6
    subf r0, r0, r26
    clrlwi r4, r0, 16
    bl fn_8012AC08
    li r4, 0x0
    mr r25, r3
    bl fn_80076398
    clrlwi r0, r3, 24
    li r23, 0x0
    cntlzw r0, r0
    srwi r0, r0, 5
    clrlwi r24, r0, 24
    cmplwi r24, 0x0
    bne @L_8006DD80
    mr r3, r25
    bl fn_80076334
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8006DD84
    @L_8006DD80:
    li r23, 0x1
    @L_8006DD84:
    cmplwi r31, 0x0
    clrlwi r23, r23, 24
    beq @L_8006DE44
    cmplwi r25, 0x0
    beq @L_8006DDCC
    mr r3, r25
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8006DDCC
    mr r3, r25
    mr r4, r31
    bl fn_800772AC
    clrlwi r0, r3, 24
    cntlzw r0, r0
    srwi r0, r0, 5
    or r0, r23, r0
    clrlwi r23, r0, 24
    @L_8006DDCC:
    mr r3, r30
    mr r4, r25
    mr r5, r31
    li r6, 0x1
    bl fn_80076A8C
    clrlwi r0, r3, 24
    mr r3, r30
    cntlzw r0, r0
    mr r4, r25
    srwi r0, r0, 5
    mr r5, r31
    or r0, r23, r0
    li r6, 0x2
    clrlwi r23, r0, 24
    bl fn_80076A8C
    clrlwi r0, r3, 24
    mr r3, r30
    cntlzw r0, r0
    mr r4, r25
    srwi r0, r0, 5
    mr r5, r31
    or r0, r23, r0
    li r6, 0x3
    clrlwi r23, r0, 24
    bl fn_80076A8C
    clrlwi r0, r3, 24
    cntlzw r0, r0
    srwi r0, r0, 5
    or r0, r23, r0
    clrlwi r23, r0, 24
    @L_8006DE44:
    lhz r4, 0x0(r27)
    mr r3, r29
    bl fn_801046C8
    mr r22, r3
    cmplwi r22, 0x0
    beq @L_8006DE9C
    cmplwi r24, 0x0
    beq @L_8006DE7C
    li r3, 0x375
    bl fn_8005D858
    mr r4, r3
    mr r3, r22
    bl fn_80071318
    b @L_8006DE9C
    @L_8006DE7C:
    clrlwi r0, r23, 24
    cmplwi r0, 0x0
    beq @L_8006DE9C
    li r3, 0x25b
    bl fn_8005D858
    mr r4, r3
    mr r3, r22
    bl fn_80071318
    @L_8006DE9C:
    lhz r4, 0x2(r27)
    mr r3, r29
    bl fn_801046C8
    mr r24, r3
    cmplwi r24, 0x0
    beq @L_8006DEF4
    clrlwi r0, r23, 24
    li r22, 0x274
    cmplwi r0, 0x0
    beq @L_8006DEC8
    li r22, 0x25c
    @L_8006DEC8:
    mr r3, r25
    bl fn_8011FC74
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8006DEE0
    li r22, 0x341
    @L_8006DEE0:
    mr r3, r22
    bl fn_8005D858
    mr r4, r3
    mr r3, r24
    bl fn_80071318
    @L_8006DEF4:
    lhz r4, 0x4(r27)
    mr r3, r29
    bl fn_801046C8
    cmplwi r3, 0x0
    beq @L_8006DF10
    mr r4, r23
    bl fn_80109220
    @L_8006DF10:
    addi r27, r27, 0x8
    addi r26, r26, 0x1
    cmplwi r26, 0xc
    blt @L_8006DD24
    lis r3, lbl_80267EA8@ha
    lwz r24, 0x1c(r29)
    addi r3, r3, lbl_80267EA8@l
    lhz r23, 0xc(r3)
    b @L_8006DF44
    @L_8006DF34:
    mr r4, r23
    addi r3, r24, 0xc
    bl fn_80108518
    lwz r24, 0x0(r24)
    @L_8006DF44:
    cmplwi r24, 0x0
    bne @L_8006DF34
    b @L_8006E0A8
    @L_8006DF50:
    lis r3, lbl_8026858C@ha
    li r23, 0x0
    addi r27, r3, lbl_8026858C@l
    @L_8006DF5C:
    lhz r4, 0x6(r27)
    mr r3, r29
    bl fn_801046C8
    mr r28, r3
    cmplwi r28, 0x0
    beq @L_8006E068
    lis r4, 0x2aab
    mr r3, r30
    subi r0, r4, 0x5555
    mulhw r4, r0, r23
    srwi r0, r4, 31
    add r0, r4, r0
    mulli r0, r0, 0x6
    subf r0, r0, r23
    clrlwi r4, r0, 16
    bl fn_8012AC08
    mr r22, r3
    cmplwi r22, 0x0
    beq @L_8006E05C
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8006E05C
    mr r3, r22
    bl fn_8011F1A0
    li r24, 0x0
    mr r25, r3
    bl fn_80077C1C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8006DFF0
    mr r3, r25
    bl fn_80077C68
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8006DFF0
    li r24, 0x1
    @L_8006DFF0:
    cmplwi r31, 0x0
    clrlwi r24, r24, 24
    beq @L_8006E018
    mr r3, r30
    mr r4, r22
    mr r5, r31
    li r6, 0x2
    bl fn_80076A8C
    and r0, r24, r3
    clrlwi r24, r0, 24
    @L_8006E018:
    clrlwi r4, r25, 16
    mr r3, r28
    neg r0, r4
    or r0, r0, r4
    srwi r4, r0, 31
    bl fn_80109220
    clrlwi r0, r24, 24
    lwz r3, 0x64(r28)
    cmplwi r0, 0x0
    clrlwi r3, r3, 24
    beq @L_8006E04C
    li r0, -0x100
    b @L_8006E050
    @L_8006E04C:
    lis r0, 0xff00
    @L_8006E050:
    or r0, r3, r0
    stw r0, 0x64(r28)
    b @L_8006E068
    @L_8006E05C:
    mr r3, r28
    li r4, 0x0
    bl fn_80109220
    @L_8006E068:
    addi r27, r27, 0x8
    addi r23, r23, 0x1
    cmplwi r23, 0xc
    blt @L_8006DF5C
    b @L_8006E0A8
    @L_8006E07C:
    lis r3, lbl_80267EA8@ha
    lwz r22, 0x1c(r29)
    addi r3, r3, lbl_80267EA8@l
    lhz r23, 0x1a(r3)
    b @L_8006E0A0
    @L_8006E090:
    mr r4, r23
    addi r3, r22, 0xc
    bl fn_80108518
    lwz r22, 0x0(r22)
    @L_8006E0A0:
    cmplwi r22, 0x0
    bne @L_8006E090
    @L_8006E0A8:
    mr r3, r29
    li r4, 0x0
    li r5, 0x0
    bl fn_80070D84
    lmw r22, 0x8(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

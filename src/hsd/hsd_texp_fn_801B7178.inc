    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmpwi r5, 0x0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    addi r31, r1, 0x8
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r4
    stw r3, 0x8(r1)
    beq @L_801B7300
    b @L_801B72F0
@L_801B71A8:
    cmpwi r29, 0x7
    beq @L_801B71C0
    lwz r3, 0x0(r31)
    lwz r0, 0x0(r3)
    cmpw r29, r0
    bne @L_801B72E8
@L_801B71C0:
    lwz r3, 0x0(r31)
    lwz r0, 0x0(r3)
    cmpwi r0, 0x4
    beq @L_801B725C
    bge @L_801B72D8
    cmpwi r0, 0x1
    beq @L_801B71E0
    b @L_801B72D8
@L_801B71E0:
    lwz r3, 0x0(r31)
    lwz r4, 0x0(r31)
    cmplwi r3, 0x0
    lwz r30, 0x4(r4)
    bne @L_801B71FC
    li r0, 0x0
    b @L_801B7228
@L_801B71FC:
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    bne @L_801B7210
    li r0, 0x2
    b @L_801B7228
@L_801B7210:
    addis r0, r3, 0x1
    cmplwi r0, 0xfffe
    bne @L_801B7224
    li r0, 0x3
    b @L_801B7228
@L_801B7224:
    lwz r0, 0x0(r3)
@L_801B7228:
    cmpwi r0, 0x4
    beq @L_801B724C
    bge @L_801B7254
    cmpwi r0, 0x1
    beq @L_801B7240
    b @L_801B7254
@L_801B7240:
    li r4, 0x7c
    bl fn_80193AF0
    b @L_801B7254
@L_801B724C:
    li r4, 0x14
    bl fn_80193AF0
@L_801B7254:
    stw r30, 0x0(r31)
    b @L_801B72F0
@L_801B725C:
    lwz r3, 0x0(r31)
    lwz r4, 0x0(r31)
    cmplwi r3, 0x0
    lwz r30, 0x4(r4)
    bne @L_801B7278
    li r0, 0x0
    b @L_801B72A4
@L_801B7278:
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    bne @L_801B728C
    li r0, 0x2
    b @L_801B72A4
@L_801B728C:
    addis r0, r3, 0x1
    cmplwi r0, 0xfffe
    bne @L_801B72A0
    li r0, 0x3
    b @L_801B72A4
@L_801B72A0:
    lwz r0, 0x0(r3)
@L_801B72A4:
    cmpwi r0, 0x4
    beq @L_801B72C8
    bge @L_801B72D0
    cmpwi r0, 0x1
    beq @L_801B72BC
    b @L_801B72D0
@L_801B72BC:
    li r4, 0x7c
    bl fn_80193AF0
    b @L_801B72D0
@L_801B72C8:
    li r4, 0x14
    bl fn_80193AF0
@L_801B72D0:
    stw r30, 0x0(r31)
    b @L_801B72F0
@L_801B72D8:
    addi r3, r2, -22576
    li r4, 0xdc
    addi r5, r2, -22528
    bl fn_80196E10
@L_801B72E8:
    lwz r3, 0x0(r31)
    addi r31, r3, 0x4
@L_801B72F0:
    lwz r0, 0x0(r31)
    cmplwi r0, 0x0
    bne @L_801B71A8
    b @L_801B74EC
@L_801B7300:
    cmpwi r29, 0x7
    beq @L_801B7310
    cmpwi r29, 0x1
    bne @L_801B74E0
@L_801B7310:
    lwz r30, 0x8(r1)
    b @L_801B735C
@L_801B7318:
    lwz r0, 0x0(r30)
    cmpwi r0, 0x1
    beq @L_801B7328
    b @L_801B7358
@L_801B7328:
    lwz r0, 0x8(r30)
    cmpwi r0, 0x0
    bne @L_801B7358
    lwz r0, 0x8(r30)
    cmpwi r0, 0x0
    bne @L_801B7358
    mr r3, r30
    li r4, 0x1
    bl fn_801B750C
    mr r3, r30
    li r4, 0x5
    bl fn_801B750C
@L_801B7358:
    lwz r30, 0x4(r30)
@L_801B735C:
    cmplwi r30, 0x0
    bne @L_801B7318
    b @L_801B74E0
@L_801B7368:
    cmpwi r29, 0x7
    beq @L_801B7380
    lwz r3, 0x0(r31)
    lwz r0, 0x0(r3)
    cmpw r29, r0
    bne @L_801B74D8
@L_801B7380:
    lwz r3, 0x0(r31)
    lwz r0, 0x0(r3)
    cmpwi r0, 0x4
    beq @L_801B743C
    bge @L_801B74C8
    cmpwi r0, 0x1
    beq @L_801B73A0
    b @L_801B74C8
@L_801B73A0:
    lwz r3, 0x0(r31)
    lwz r0, 0x8(r3)
    cmpwi r0, 0x0
    bne @L_801B74D8
    lwz r3, 0x0(r31)
    lwz r0, 0x14(r3)
    cmpwi r0, 0x0
    bne @L_801B74D8
    lwz r3, 0x0(r31)
    lwz r4, 0x0(r31)
    cmplwi r3, 0x0
    lwz r30, 0x4(r4)
    bne @L_801B73DC
    li r0, 0x0
    b @L_801B7408
@L_801B73DC:
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    bne @L_801B73F0
    li r0, 0x2
    b @L_801B7408
@L_801B73F0:
    addis r0, r3, 0x1
    cmplwi r0, 0xfffe
    bne @L_801B7404
    li r0, 0x3
    b @L_801B7408
@L_801B7404:
    lwz r0, 0x0(r3)
@L_801B7408:
    cmpwi r0, 0x4
    beq @L_801B742C
    bge @L_801B7434
    cmpwi r0, 0x1
    beq @L_801B7420
    b @L_801B7434
@L_801B7420:
    li r4, 0x7c
    bl fn_80193AF0
    b @L_801B7434
@L_801B742C:
    li r4, 0x14
    bl fn_80193AF0
@L_801B7434:
    stw r30, 0x0(r31)
    b @L_801B74E0
@L_801B743C:
    lwz r3, 0x0(r31)
    lbz r0, 0x10(r3)
    cmplwi r0, 0x0
    bne @L_801B74D8
    lwz r3, 0x0(r31)
    lwz r4, 0x0(r31)
    cmplwi r3, 0x0
    lwz r30, 0x4(r4)
    bne @L_801B7468
    li r0, 0x0
    b @L_801B7494
@L_801B7468:
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    bne @L_801B747C
    li r0, 0x2
    b @L_801B7494
@L_801B747C:
    addis r0, r3, 0x1
    cmplwi r0, 0xfffe
    bne @L_801B7490
    li r0, 0x3
    b @L_801B7494
@L_801B7490:
    lwz r0, 0x0(r3)
@L_801B7494:
    cmpwi r0, 0x4
    beq @L_801B74B8
    bge @L_801B74C0
    cmpwi r0, 0x1
    beq @L_801B74AC
    b @L_801B74C0
@L_801B74AC:
    li r4, 0x7c
    bl fn_80193AF0
    b @L_801B74C0
@L_801B74B8:
    li r4, 0x14
    bl fn_80193AF0
@L_801B74C0:
    stw r30, 0x0(r31)
    b @L_801B74E0
@L_801B74C8:
    addi r3, r2, -22576
    li r4, 0x10c
    addi r5, r2, -22528
    bl fn_80196E10
@L_801B74D8:
    lwz r3, 0x0(r31)
    addi r31, r3, 0x4
@L_801B74E0:
    lwz r0, 0x0(r31)
    cmplwi r0, 0x0
    bne @L_801B7368
@L_801B74EC:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r3, 0x8(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

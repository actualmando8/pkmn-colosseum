    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr. r31, r5
    stw r30, 0x18(r1)
    mr r30, r4
    stw r29, 0x14(r1)
    mr r29, r3
    beq @L_800EE3A0
    lwz r5, 0x4(r29)
    li r0, 0x4
    addi r4, r5, 0xe4
    mtctr r0
    @L_800EE300:
    lwz r3, 0x4(r4)
    lhz r0, 0x2(r29)
    cmplw r3, r0
    bne @L_800EE328
    lis r3, lbl_80270F10@ha
    addi r3, r3, lbl_80270F10@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_800EE388
    @L_800EE328:
    addi r4, r4, 0xc
    bdnz @L_800EE300
    lwz r0, 0xe4(r5)
    addi r3, r5, 0xe4
    cmpwi r0, 0x0
    bne @L_800EE344
    b @L_800EE388
    @L_800EE344:
    lwzu r0, 0xc(r3)
    cmpwi r0, 0x0
    bne @L_800EE354
    b @L_800EE388
    @L_800EE354:
    lwzu r0, 0xc(r3)
    cmpwi r0, 0x0
    bne @L_800EE364
    b @L_800EE388
    @L_800EE364:
    lwzu r0, 0xc(r3)
    cmpwi r0, 0x0
    bne @L_800EE374
    b @L_800EE388
    @L_800EE374:
    lis r3, lbl_80270F44@ha
    addi r3, r3, lbl_80270F44@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    @L_800EE388:
    cmplwi r3, 0x0
    beq @L_800EE3A0
    stw r31, 0x0(r3)
    lhz r0, 0x2(r29)
    stw r0, 0x4(r3)
    stw r30, 0x8(r3)
    @L_800EE3A0:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

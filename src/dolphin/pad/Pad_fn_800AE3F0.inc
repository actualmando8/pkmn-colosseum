    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x8(r1)
    lwz r0, lbl_8047A93C(r13)
    cmplwi r0, 0x0
    bne @L_800AE420
    lwz r3, lbl_8047A930(r13)
    cmplwi r3, 0x0
    beq @L_800AE420
    stw r3, lbl_8047A93C(r13)
    lwz r0, 0x0(r3)
    stw r0, lbl_8047A930(r13)
    @L_800AE420:
    lwz r5, lbl_8047A93C(r13)
    cmplwi r5, 0x0
    beq @L_800AE4E0
    lwz r6, 0x18(r5)
    lwz r0, lbl_8047A948(r13)
    cmplw r6, r0
    bgt @L_800AE474
    lwz r3, 0x8(r5)
    cmplwi r3, 0x0
    bne @L_800AE458
    lwz r4, 0x10(r5)
    lwz r5, 0x14(r5)
    bl fn_800AC990
    b @L_800AE464
    @L_800AE458:
    lwz r4, 0x14(r5)
    lwz r5, 0x10(r5)
    bl fn_800AC990
    @L_800AE464:
    lwz r3, lbl_8047A93C(r13)
    lwz r0, 0x1c(r3)
    stw r0, lbl_8047A944(r13)
    b @L_800AE4A4
    @L_800AE474:
    lwz r3, 0x8(r5)
    cmplwi r3, 0x0
    bne @L_800AE494
    lwz r4, 0x10(r5)
    mr r6, r0
    lwz r5, 0x14(r5)
    bl fn_800AC990
    b @L_800AE4A4
    @L_800AE494:
    lwz r4, 0x14(r5)
    mr r6, r0
    lwz r5, 0x10(r5)
    bl fn_800AC990
    @L_800AE4A4:
    lwz r3, lbl_8047A93C(r13)
    lwz r4, lbl_8047A948(r13)
    lwz r0, 0x18(r3)
    subf r0, r4, r0
    stw r0, 0x18(r3)
    lwz r4, lbl_8047A93C(r13)
    lwz r0, lbl_8047A948(r13)
    lwz r3, 0x10(r4)
    add r0, r3, r0
    stw r0, 0x10(r4)
    lwz r4, lbl_8047A93C(r13)
    lwz r0, lbl_8047A948(r13)
    lwz r3, 0x14(r4)
    add r0, r3, r0
    stw r0, 0x14(r4)
    @L_800AE4E0:
    lwz r0, 0xc(r1)
    addi r1, r1, 0x8
    mtlr r0
    blr

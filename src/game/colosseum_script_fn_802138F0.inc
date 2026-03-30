    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lwz r4, lbl_8047B610(r13)
    lwz r5, 0x2(r4)
    lbz r3, 0x1(r4)
    cmplwi r5, 0x0
    lhz r6, 0xa(r4)
    lwz r7, 0x6(r4)
    lwz r8, 0xc(r4)
    lwz r31, 0x10(r4)
    bne @L_8021392C
    li r4, 0x0
    b @L_80213930
@L_8021392C:
    lwz r4, 0x0(r5)
@L_80213930:
    cmplwi r7, 0x0
    bne @L_80213940
    li r5, 0x0
    b @L_80213944
@L_80213940:
    lbz r5, 0x0(r7)
@L_80213944:
    cmplwi r8, 0x0
    bne @L_80213954
    li r7, 0x0
    b @L_80213958
@L_80213954:
    lbz r7, 0x0(r8)
@L_80213958:
    bl fn_80135E44
    sth r3, 0x0(r31)
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x14
    stw r0, lbl_8047B610(r13)
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

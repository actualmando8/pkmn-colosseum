    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r3, lbl_80478C70(r13)
    bl fn_8009AC3C
    lwz r0, lbl_8047B270(r13)
    cmplwi r0, 0x0
    beq @L_8019C82C
    lwz r0, lbl_8047B274(r13)
    cmplwi r0, 0x0
    beq @L_8019C82C
    lwz r5, lbl_8047B270(r13)
    li r3, 0x0
    lwz r4, lbl_8047B274(r13)
    li r0, 0x0
    stw r5, lbl_8047B268(r13)
    stw r4, lbl_8047B26C(r13)
    stw r3, lbl_8047B270(r13)
    stw r0, lbl_8047B274(r13)
    @L_8019C82C:
    lwz r3, lbl_8047B268(r13)
    lwz r4, lbl_8047B26C(r13)
    bl fn_8009ABD0
    stw r3, lbl_80478C70(r13)
    lwz r3, lbl_80478C70(r13)
    bl fn_8009AB50
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, lbl_80466BC0@ha
    stw r0, 0x14(r1)
    addi r0, r4, lbl_80466BC0@l
    stw r31, 0xc(r1)
    mr r31, r0
    lbz r0, 0x19(r31)
    stw r3, lbl_8047B294(r13)
    cmplwi r0, 0x0
    beq @L_8019C744
    lwz r4, lbl_80478C78(r13)
    li r3, 0x2
    bl fn_800BCEF4
    b @L_8019C750
    @L_8019C744:
    lwz r3, lbl_8047B27C(r13)
    li r4, 0x0
    bl fn_800BCEF4
    @L_8019C750:
    lhz r4, 0x8(r31)
    lhz r0, 0x10(r31)
    lbz r3, 0x18(r31)
    subf r0, r0, r4
    srwi r4, r0, 31
    bl fn_800BD07C
    lwz r0, lbl_8047B290(r13)
    cmpwi r0, 0x0
    beq @L_8019C79C
    lwz r0, lbl_8047B290(r13)
    clrlwi. r0, r0, 31
    beq @L_8019C784
    bl fn_800B856C
    @L_8019C784:
    lwz r0, lbl_8047B290(r13)
    rlwinm. r0, r0, 0, 30, 30
    beq @L_8019C794
    bl fn_800BB29C
    @L_8019C794:
    li r0, 0x0
    stw r0, lbl_8047B290(r13)
    @L_8019C79C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

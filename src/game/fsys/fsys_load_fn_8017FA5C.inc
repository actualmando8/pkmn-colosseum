    nofralloc
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    li r30, 0x0
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    li r28, 0x0
    lwz r0, lbl_8047B1D0(r13)
    lwz r29, lbl_8047B1D0(r13)
    cmplwi r0, 0x0
    bne @L_8017FA98
    lis r3, lbl_80455048@ha
    addi r3, r3, lbl_80455048@l
    lwz r30, 0x24(r3)
    b @L_8017FAEC
    @L_8017FA98:
    li r28, 0x0
    lwz r31, 0x0(r29)
    @L_8017FAA0:
    lis r0, 0x8000
    addi r28, r28, 0x1
    cmplw r31, r0
    bgt @L_8017FAB8
    mr r3, r30
    b @L_8017FAF0
    @L_8017FAB8:
    cmplwi r31, 0x0
    beq @L_8017FAC8
    lwz r0, 0x4(r31)
    add r30, r30, r0
    @L_8017FAC8:
    lwz r0, lbl_8047B1D0(r13)
    cmplw r31, r0
    beq @L_8017FADC
    lwz r31, 0x0(r31)
    b @L_8017FAA0
    @L_8017FADC:
    lis r3, lbl_80455048@ha
    addi r3, r3, lbl_80455048@l
    lwz r0, 0x24(r3)
    add r30, r30, r0
    @L_8017FAEC:
    mr r3, r30
    @L_8017FAF0:
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    addi r1, r1, 0x20
    blr

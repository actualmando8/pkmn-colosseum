    nofralloc
    cmplwi r3, 0x0
    beq @L_801A6C94
    la r5, lbl_8047B2DC(r13)
    b @L_801A6C6C
    @L_801A6C44:
    lwz r0, 0x0(r5)
    cmplw r0, r3
    bne @L_801A6C64
    lwz r4, 0x8(r3)
    li r0, 0x0
    stw r4, 0x0(r5)
    stw r0, 0x8(r3)
    blr
    @L_801A6C64:
    lwz r4, 0x0(r5)
    addi r5, r4, 0x8
    @L_801A6C6C:
    lwz r0, 0x0(r5)
    cmplwi r0, 0x0
    bne @L_801A6C44
    blr
    @L_801A6C7C:
    lwz r4, lbl_8047B2DC(r13)
    li r0, 0x0
    lwz r3, lbl_8047B2DC(r13)
    lwz r4, 0x8(r4)
    stw r0, 0x8(r3)
    stw r4, lbl_8047B2DC(r13)
    @L_801A6C94:
    lwz r0, lbl_8047B2DC(r13)
    cmplwi r0, 0x0
    bne @L_801A6C7C
    blr

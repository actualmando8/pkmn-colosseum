    nofralloc
    lwz r4, lbl_80478B08(r13)
    lwz r0, 0x8(r4)
    cmplwi r0, 0x0
    bne @L_800FC1E8
    li r3, -0x1
    blr
@L_800FC1E8:
    mr r5, r0
    b @L_800FC234
@L_800FC1F0:
    cmplw r5, r3
    bne @L_800FC230
    lwz r3, 0xc(r5)
    cmplwi r3, 0x0
    beq @L_800FC210
    lwz r0, 0x8(r5)
    stw r0, 0x8(r3)
    b @L_800FC218
@L_800FC210:
    lwz r0, 0x8(r5)
    stw r0, 0x8(r4)
@L_800FC218:
    lwz r3, 0x8(r5)
    cmplwi r3, 0x0
    beq @L_800FC23C
    lwz r0, 0xc(r5)
    stw r0, 0xc(r3)
    b @L_800FC23C
@L_800FC230:
    lwz r5, 0x8(r5)
@L_800FC234:
    cmplwi r5, 0x0
    bne @L_800FC1F0
@L_800FC23C:
    li r3, 0x0
    blr

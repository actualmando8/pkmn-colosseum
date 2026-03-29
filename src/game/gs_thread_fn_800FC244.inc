    nofralloc
    lwz r4, lbl_80478B08(r13)
    lwz r0, 0x8(r4)
    cmplwi r0, 0x0
    bne @L_800FC268
    stw r3, 0x8(r4)
    li r0, 0x0
    stw r0, 0x8(r3)
    stw r0, 0xc(r3)
    blr
@L_800FC268:
    mr r4, r0
@L_800FC26C:
    cmplw r4, r3
    bne @L_800FC27C
    li r3, 0x0
    blr
@L_800FC27C:
    lwz r0, 0x8(r4)
    cmplwi r0, 0x0
    bne @L_800FC29C
    stw r3, 0x8(r4)
    li r0, 0x0
    stw r0, 0x8(r3)
    stw r4, 0xc(r3)
    blr
@L_800FC29C:
    mr r4, r0
    b @L_800FC26C

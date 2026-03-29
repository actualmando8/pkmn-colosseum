    nofralloc
    lwz r7, lbl_80478B08(r13)
    li r5, 0x0
    lhz r6, 0x0(r7)
    b @L_800FA410
@L_800FA3E0:
    clrlwi r0, r5, 24
    lwz r4, 0x20(r7)
    mulli r0, r0, 0x68
    addi r5, r5, 0x1
    add r4, r4, r0
    lbz r0, 0x0(r4)
    cmplwi r0, 0x0
    beq @L_800FA410
    lwz r0, 0x1c(r4)
    cmplw r0, r3
    bne @L_800FA410
    b @L_800FA420
@L_800FA410:
    clrlwi r0, r5, 24
    cmpw r0, r6
    blt @L_800FA3E0
    li r4, 0x0
@L_800FA420:
    cmplwi r4, 0x0
    beq @L_800FA43C
    lbz r0, 0x0(r4)
    cmplwi r0, 0x1
    bne @L_800FA43C
    li r3, 0x1
    blr
@L_800FA43C:
    li r3, 0x0
    blr

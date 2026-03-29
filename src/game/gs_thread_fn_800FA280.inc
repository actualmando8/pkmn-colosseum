    nofralloc
    cmplwi r3, 0x0
    bne @L_800FA290
    li r3, 0x0
    blr
@L_800FA290:
    lwz r4, lbl_80478B08(r13)
    srwi r5, r3, 20
    clrlwi r3, r3, 12
    lwz r9, 0x8(r4)
    b @L_800FA304
@L_800FA2A4:
    lhz r0, 0x0(r9)
    cmplw r0, r5
    bne @L_800FA300
    lhz r6, 0x4(r9)
    addi r8, r9, 0x10
    li r7, 0x0
    b @L_800FA2F8
@L_800FA2C0:
    add r0, r7, r6
    srwi r4, r0, 1
    slwi r10, r4, 3
    lwzx r0, r8, r10
    cmplw r0, r3
    bne @L_800FA2E8
    add r3, r8, r10
    lwz r0, 0x4(r3)
    add r3, r9, r0
    blr
@L_800FA2E8:
    bge @L_800FA2F4
    addi r7, r4, 0x1
    b @L_800FA2F8
@L_800FA2F4:
    mr r6, r4
@L_800FA2F8:
    cmplw r7, r6
    blt @L_800FA2C0
@L_800FA300:
    lwz r9, 0x8(r9)
@L_800FA304:
    cmplwi r9, 0x0
    bne @L_800FA2A4
    li r3, 0x0
    blr

    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    cmplwi r3, 0x0
    bne @L_800FA330
    li r3, 0x0
    b @L_800FA3C0
@L_800FA330:
    bne @L_800FA33C
    li r3, 0x0
    b @L_800FA3BC
@L_800FA33C:
    lwz r4, lbl_80478B08(r13)
    srwi r5, r3, 20
    clrlwi r3, r3, 12
    lwz r9, 0x8(r4)
    b @L_800FA3B0
@L_800FA350:
    lhz r0, 0x0(r9)
    cmplw r0, r5
    bne @L_800FA3AC
    lhz r6, 0x4(r9)
    addi r8, r9, 0x10
    li r7, 0x0
    b @L_800FA3A4
@L_800FA36C:
    add r0, r7, r6
    srwi r4, r0, 1
    slwi r10, r4, 3
    lwzx r0, r8, r10
    cmplw r0, r3
    bne @L_800FA394
    add r3, r8, r10
    lwz r0, 0x4(r3)
    add r3, r9, r0
    b @L_800FA3BC
@L_800FA394:
    bge @L_800FA3A0
    addi r7, r4, 0x1
    b @L_800FA3A4
@L_800FA3A0:
    mr r6, r4
@L_800FA3A4:
    cmplw r7, r6
    blt @L_800FA36C
@L_800FA3AC:
    lwz r9, 0x8(r9)
@L_800FA3B0:
    cmplwi r9, 0x0
    bne @L_800FA350
    li r3, 0x0
@L_800FA3BC:
    bl fn_800FDFE4
@L_800FA3C0:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

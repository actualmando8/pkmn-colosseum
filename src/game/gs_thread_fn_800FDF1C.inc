    nofralloc
    lwz r8, lbl_80478B08(r13)
    li r11, 0x0
    li r7, 0x0
    lhz r9, 0x4(r8)
    mtctr r9
    cmpwi r9, 0x0
    ble @L_800FDF5C
@L_800FDF38:
    lwz r6, 0x24(r8)
    lhz r0, 0x20(r3)
    add r10, r6, r7
    lhz r6, 0x0(r10)
    cmplw r6, r0
    beq @L_800FDF5C
    addi r7, r7, 0x8
    addi r11, r11, 0x1
    bdnz @L_800FDF38
@L_800FDF5C:
    cmpw r11, r9
    bne @L_800FDF6C
    li r3, 0x0
    blr
@L_800FDF6C:
    lwz r6, 0x4(r10)
    clrlwi r4, r4, 16
    b @L_800FDFD4
@L_800FDF78:
    lhz r9, 0x0(r6)
    addi r7, r6, 0x10
    li r8, 0x0
    b @L_800FDFC4
@L_800FDF88:
    add r0, r8, r9
    srwi r10, r0, 1
    slwi r0, r10, 3
    add r3, r7, r0
    lhz r0, 0x0(r3)
    cmplw r0, r4
    bne @L_800FDFB4
    cmplwi r5, 0x0
    beqlr
    stw r6, 0x0(r5)
    blr
@L_800FDFB4:
    bge @L_800FDFC0
    addi r8, r10, 0x1
    b @L_800FDFC4
@L_800FDFC0:
    mr r9, r10
@L_800FDFC4:
    cmplw r8, r9
    blt @L_800FDF88
    blt @L_800FDFDC
    lwz r6, 0x8(r6)
@L_800FDFD4:
    cmplwi r6, 0x0
    bne @L_800FDF78
@L_800FDFDC:
    li r3, 0x0
    blr

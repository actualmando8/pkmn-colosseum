    nofralloc
    cmplwi r3, 0x0
    beq @L_8013E4CC
    lwz r6, 0x18(r3)
    cmplwi r6, 0x0
    beq @L_8013E4CC
    b @L_8013E4AC
    @L_8013E488:
    lwz r0, 0x1c(r3)
    subf r0, r5, r0
    stw r0, 0x1c(r3)
    lwz r6, 0x10(r6)
    cmplwi r6, 0x0
    stw r6, 0x18(r3)
    bne @L_8013E4AC
    li r3, 0x0
    blr
    @L_8013E4AC:
    lwz r0, 0x1c(r3)
    lwz r5, 0x8(r6)
    cmplw r0, r5
    bge @L_8013E488
    add r0, r0, r4
    stw r0, 0x1c(r3)
    li r3, 0x1
    blr
    @L_8013E4CC:
    li r3, 0x0
    blr

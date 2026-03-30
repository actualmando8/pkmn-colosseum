    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x0
    li r6, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r31, r5
    mr r30, r4
    li r5, 0x9
    mr r4, r31
    bl fn_8011BEB4
    clrlwi r0, r30, 16
    clrlwi r3, r3, 16
    cmplwi r0, 0x1f
    bne @L_8025C608
    cmplwi r3, 0x92
    beq @L_8025C600
    cmplwi r3, 0x95
    beq @L_8025C600
    cmplwi r3, 0x98
    beq @L_8025C600
    cmplwi r3, 0xcf
    bne @L_8025C608
@L_8025C600:
    li r3, 0x1
    b @L_8025C660
@L_8025C608:
    clrlwi r0, r30, 16
    cmplwi r0, 0x20
    bne @L_8025C624
    cmplwi r3, 0x93
    bne @L_8025C624
    li r3, 0x1
    b @L_8025C660
@L_8025C624:
    clrlwi r0, r30, 16
    cmplwi r0, 0x21
    bne @L_8025C64C
    clrlwi r0, r31, 16
    cmplwi r0, 0x39
    beq @L_8025C644
    cmplwi r0, 0xfa
    bne @L_8025C64C
@L_8025C644:
    li r3, 0x1
    b @L_8025C660
@L_8025C64C:
    cmplwi r3, 0x5e
    bne @L_8025C65C
    li r3, 0x1
    b @L_8025C660
@L_8025C65C:
    li r3, 0x0
@L_8025C660:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

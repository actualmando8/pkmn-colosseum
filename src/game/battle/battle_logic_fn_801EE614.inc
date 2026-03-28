    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    clrlwi. r3, r3, 16
    stw r0, 0x14(r1)
    mulli r0, r3, 0x38
    lwz r4, -30900(r13)
    add r4, r4, r0
    beq @L_801EE63C
    cmplwi r3, 0x60
    ble @L_801EE640
@L_801EE63C:
    li r4, 0x0
@L_801EE640:
    cmplwi r4, 0x0
    beq @L_801EE668
    lhz r3, 0x1c(r4)
    cmplwi r3, 0x0
    beq @L_801EE660
    bl fn_801906A0
    extsb r3, r3
    b @L_801EE66C
@L_801EE660:
    li r3, 0x0
    b @L_801EE66C
@L_801EE668:
    li r3, 0x0
@L_801EE66C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

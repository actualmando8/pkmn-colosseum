    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    clrlwi. r3, r3, 16
    stw r0, 0x14(r1)
    mulli r0, r3, 0x38
    lwz r4, -30900(r13)
    add r4, r4, r0
    beq @L_801EEAF8
    cmplwi r3, 0x60
    ble @L_801EEAFC
@L_801EEAF8:
    li r4, 0x0
@L_801EEAFC:
    cmplwi r4, 0x0
    beq @L_801EEB20
    lhz r3, 0x12(r4)
    cmplwi r3, 0x0
    beq @L_801EEB18
    bl fn_801902E0
    b @L_801EEB24
@L_801EEB18:
    li r3, 0x0
    b @L_801EEB24
@L_801EEB20:
    li r3, 0x0
@L_801EEB24:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

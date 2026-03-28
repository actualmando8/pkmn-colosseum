    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, -32713
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    li r30, 0x0
    addi r31, r3, 21056
    b @L_801EE41C
@L_801EE3B8:
    clrlslwi r0, r30, 16, 1
    lwz r3, -30900(r13)
    lhzx r4, r31, r0
    mulli r0, r4, 0x38
    cmplwi r4, 0x0
    add r3, r3, r0
    beq @L_801EE3DC
    cmplwi r4, 0x60
    ble @L_801EE3E0
@L_801EE3DC:
    li r3, 0x0
@L_801EE3E0:
    cmplwi r3, 0x0
    beq @L_801EE404
    lhz r3, 0x14(r3)
    cmplwi r3, 0x0
    beq @L_801EE3FC
    bl fn_801902E0
    b @L_801EE408
@L_801EE3FC:
    li r3, 0x0
    b @L_801EE408
@L_801EE404:
    li r3, 0x0
@L_801EE408:
    clrlwi. r0, r3, 24
    bne @L_801EE418
    li r3, 0x0
    b @L_801EE42C
@L_801EE418:
    addi r30, r30, 0x1
@L_801EE41C:
    clrlwi r0, r30, 16
    cmplwi r0, 0x30
    blt @L_801EE3B8
    li r3, 0x1
@L_801EE42C:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

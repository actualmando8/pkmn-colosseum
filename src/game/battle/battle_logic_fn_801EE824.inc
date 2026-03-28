    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    clrlwi. r3, r3, 16
    stw r0, 0x14(r1)
    mulli r0, r3, 0x38
    lwz r5, -30900(r13)
    add r5, r5, r0
    beq @L_801EE84C
    cmplwi r3, 0x60
    ble @L_801EE850
@L_801EE84C:
    li r5, 0x0
@L_801EE850:
    cmplwi r5, 0x0
    beq @L_801EE880
    clrlslwi r0, r4, 16, 1
    add r3, r5, r0
    lhz r3, 0x24(r3)
    cmplwi r3, 0x0
    beq @L_801EE878
    bl fn_801906A0
    extsb r3, r3
    b @L_801EE884
@L_801EE878:
    li r3, 0x0
    b @L_801EE884
@L_801EE880:
    li r3, 0x0
@L_801EE884:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

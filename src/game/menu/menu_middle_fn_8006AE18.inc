    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r3, 0x8ae
    bl fn_801906A0
    cmplwi r3, 0x0
    beq @L_8006AED8
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    addis r3, r3, 0x1
    lbz r0, -0x3678(r3)
    cmplwi r0, 0x0
    beq @L_8006AE58
    subi r3, r3, 0x4cd8
    b @L_8006AE5C
    @L_8006AE58:
    li r3, 0x0
    @L_8006AE5C:
    cmplwi r3, 0x0
    beq @L_8006AED8
    lhz r0, 0x0(r3)
    cmpwi r0, 0x3
    beq @L_8006AEAC
    bge @L_8006AE84
    cmpwi r0, 0x1
    beq @L_8006AE9C
    bge @L_8006AEA4
    b @L_8006AEC4
    @L_8006AE84:
    cmpwi r0, 0x309
    beq @L_8006AEB4
    bge @L_8006AEC4
    cmpwi r0, 0x308
    bge @L_8006AEBC
    b @L_8006AEC4
    @L_8006AE9C:
    li r3, 0x0
    b @L_8006AEDC
    @L_8006AEA4:
    li r3, 0x1
    b @L_8006AEDC
    @L_8006AEAC:
    li r3, 0x2
    b @L_8006AEDC
    @L_8006AEB4:
    li r3, 0x3
    b @L_8006AEDC
    @L_8006AEBC:
    li r3, 0x4
    b @L_8006AEDC
    @L_8006AEC4:
    lis r3, lbl_80267DE8@ha
    li r4, 0x1c2
    addi r3, r3, lbl_80267DE8@l
    la r5, lbl_8047C040(r2)
    bl fn_80196E10
    @L_8006AED8:
    li r3, 0x0
    @L_8006AEDC:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

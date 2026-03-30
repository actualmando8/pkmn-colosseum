    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    beq @L_801AFEE4
    cmplwi r4, 0x0
    bne @L_801AFE90
    b @L_801AFEE4
    @L_801AFE90:
    mr r31, r3
    mr r30, r4
    b @L_801AFED4
    @L_801AFE9C:
    cmplwi r31, 0x0
    beq @L_801AFECC
    cmplwi r30, 0x0
    beq @L_801AFECC
    lwz r0, 0x18(r31)
    cmplwi r0, 0x0
    beq @L_801AFEC0
    lwz r3, 0x18(r31)
    bl fn_801C25E4
    @L_801AFEC0:
    lwz r3, 0x4(r30)
    bl fn_801C2670
    stw r3, 0x18(r31)
    @L_801AFECC:
    lwz r31, 0x0(r31)
    lwz r30, 0x0(r30)
    @L_801AFED4:
    cmplwi r31, 0x0
    beq @L_801AFEE4
    cmplwi r30, 0x0
    bne @L_801AFE9C
    @L_801AFEE4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
